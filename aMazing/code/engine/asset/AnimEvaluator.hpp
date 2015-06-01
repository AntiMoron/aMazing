#pragma once

#include <assimp/anim.h>
#include <vector>
#include <tuple>

namespace aMazing
{
	/** Calculates transformations for a given timestamp from a set of animation tracks. Not directly useful,
	* better use the AnimPlayer class.
	*/
	class AnimEvaluator
	{
	public:
		explicit AnimEvaluator() = delete;
		/** Constructor on a given animation. The animation is fixed throughout the lifetime of
		* the object.
		* @param pAnim The animation to calculate poses for. Ownership of the animation object stays
		*   at the caller, the evaluator just keeps a reference to it as long as it persists.
		* !!!For component from assimp should not use shared_ptr.
		*/
		explicit AnimEvaluator(const aiAnimation* pAnim)
		{
			mAnim = pAnim;
			mLastTime = 0.0;
			mLastPositions.resize(pAnim->mNumChannels, std::make_tuple(0, 0, 0));
		}

		/** Evaluates the animation tracks for a given time stamp. The calculated pose can be retrieved as a
		* array of transformation matrices afterwards by calling GetTransformations().
		* @param pTime The time for which you want to evaluate the animation, in seconds. Will be mapped into the animation cycle, so
		*   it can be an arbitrary value. Best use with ever-increasing time stamps.
		*/
		void Evaluate(double pTime, std::size_t fps)
		{
			// extract ticks per second. Assume default value if not given
			double ticksPerSecond = mAnim->mTicksPerSecond != 0.0 ? mAnim->mTicksPerSecond : double(fps);
			// every following time calculation happens in ticks
			pTime *= ticksPerSecond;

			// map into anim's duration
			double time = 0.0f;
			if (mAnim->mDuration > 0.0)
				time = fmod(pTime, mAnim->mDuration);

			if (mTransforms.size() != mAnim->mNumChannels)
				mTransforms.resize(mAnim->mNumChannels);

			// calculate the transformations for each animation channel
			for (unsigned int a = 0; a < mAnim->mNumChannels; a++)
			{
				const aiNodeAnim* channel = mAnim->mChannels[a];

				// ******** Position *****
				aiVector3D presentPosition(0, 0, 0);
				if (channel->mNumPositionKeys > 0)
				{
					// Look for present frame number. Search from last position if time is after the last time, else from beginning
					// Should be much quicker than always looking from start for the average use case.
					unsigned int frame = (time >= mLastTime) ? std::get<0>(mLastPositions[a]) : 0;
					while (frame < channel->mNumPositionKeys - 1)
					{
						if (time < channel->mPositionKeys[frame + 1].mTime)
							break;
						frame++;
					}

					// interpolate between this frame's value and next frame's value
					unsigned int nextFrame = (frame + 1) % channel->mNumPositionKeys;
					const aiVectorKey& key = channel->mPositionKeys[frame];
					const aiVectorKey& nextKey = channel->mPositionKeys[nextFrame];
					double diffTime = nextKey.mTime - key.mTime;
					if (diffTime < 0.0)
						diffTime += mAnim->mDuration;
					if (diffTime > 0)
					{
						float factor = float((time - key.mTime) / diffTime);
						presentPosition = key.mValue + (nextKey.mValue - key.mValue) * factor;
					}
					else
					{
						presentPosition = key.mValue;
					}
					std::get<0>(mLastPositions[a]) = frame;
				}

				// ******** Rotation *********
				aiQuaternion presentRotation(1, 0, 0, 0);
				if (channel->mNumRotationKeys > 0)
				{
					unsigned int frame = (time >= mLastTime) ? std::get<1>(mLastPositions[a]) : 0;
					while (frame < channel->mNumRotationKeys - 1)
					{
						if (time < channel->mRotationKeys[frame + 1].mTime)
							break;
						frame++;
					}

					// interpolate between this frame's value and next frame's value
					unsigned int nextFrame = (frame + 1) % channel->mNumRotationKeys;
					const aiQuatKey& key = channel->mRotationKeys[frame];
					const aiQuatKey& nextKey = channel->mRotationKeys[nextFrame];
					double diffTime = nextKey.mTime - key.mTime;
					if (diffTime < 0.0)
						diffTime += mAnim->mDuration;
					if (diffTime > 0)
					{
						float factor = float((time - key.mTime) / diffTime);
						aiQuaternion::Interpolate(presentRotation, key.mValue, nextKey.mValue, factor);
					}
					else
					{
						presentRotation = key.mValue;
					}

					std::get<1>(mLastPositions[a]) = frame;
				}

				// ******** Scaling **********
				aiVector3D presentScaling(1, 1, 1);
				if (channel->mNumScalingKeys > 0)
				{
					unsigned int frame = (time >= mLastTime) ? std::get<2>(mLastPositions[a]) : 0;
					while (frame < channel->mNumScalingKeys - 1)
					{
						if (time < channel->mScalingKeys[frame + 1].mTime)
							break;
						frame++;
					}

					// TODO: (thom) interpolation maybe? This time maybe even logarithmic, not linear
					presentScaling = channel->mScalingKeys[frame].mValue;
					std::get<2>(mLastPositions[a]) = frame;
				}

				// build a transformation matrix from it
				aiMatrix4x4& mat = mTransforms[a];
				mat = aiMatrix4x4(presentRotation.GetMatrix());
				mat.a1 *= presentScaling.x; mat.b1 *= presentScaling.x; mat.c1 *= presentScaling.x;
				mat.a2 *= presentScaling.y; mat.b2 *= presentScaling.y; mat.c2 *= presentScaling.y;
				mat.a3 *= presentScaling.z; mat.b3 *= presentScaling.z; mat.c3 *= presentScaling.z;
				mat.a4 = presentPosition.x; mat.b4 = presentPosition.y; mat.c4 = presentPosition.z;
			}

			mLastTime = time;
		}

		/** Returns the transform matrices calculated at the last Evaluate() call.
		* The array matches the mChannels array of
		* the aiAnimation. */
		const std::vector<aiMatrix4x4>& GetTransformations() const
		{
			return mTransforms;
		}

	protected:
		/** The animation we're working on */
		const aiAnimation* mAnim;

		/** At which frame the last evaluation happened for each channel.
		* Useful to quickly find the corresponding frame for slightly increased time stamps
		*/
		double mLastTime;
		std::vector<std::tuple<unsigned int, unsigned int, unsigned int> > mLastPositions;

		/** The array to store the transformations results of the evaluation */
		std::vector<aiMatrix4x4> mTransforms;
	};
}
