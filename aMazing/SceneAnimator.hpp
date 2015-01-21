#pragma once

#include<vector>
#include<unordered_map>
#include<assimp/scene.h>
#include<memory>
#include<assimp/Importer.hpp>
#include"SceneAnimNode.hpp"
#include"AnimEvaluator.hpp"
class SceneAnimator
{
public:
	/** Constructor for a given scene.
	*
	* The object keeps a reference to the scene during its lifetime, but
	* ownership stays at the caller.
	* @param pScene The scene to animate.
	* @param pAnimIndex [optional] Index of the animation to play. Assumed to
	*  be 0 if not given.
	*/
	SceneAnimator(const aiScene* pScene,size_t pAnimIndex = 0)
	{
		mScene = pScene;
		mCurrentAnimIndex = -1;
		mAnimEvaluator = nullptr;
		mRootNode = nullptr;

		// build the nodes-for-bones table
		for (unsigned int i = 0; i < pScene->mNumMeshes; ++i)
		{
			const auto mesh = pScene->mMeshes[i];
			for (unsigned int n = 0; n < mesh->mNumBones; ++n)
			{
				const auto bone = mesh->mBones[n];
				mBoneNodesByName[bone->mName.data] = pScene->mRootNode->FindNode(bone->mName);
			}
		}

		// set default animation also creates the node tree for this animation
		setAnimIndex(pAnimIndex);
	}

	~SceneAnimator()
	{
	}
	/** Sets the animation to use for playback. This also recreates the internal
	* mapping structures, which might take a few cycles.
	* @param pAnimIndex Index of the animation in the scene's animation array
	*/
	void setAnimIndex(size_t animIndex)
	{
		// if animIndex is cached,then do nothing.
		if (animIndex == mCurrentAnimIndex)
			return;

		//clear up data
		mNodesByName.clear();

		mCurrentAnimIndex = animIndex;

		// create the internal node tree. Do this even in case of invalid animation index
		// so that the transformation matrices are properly set up to mimic the current scene
		mRootNode.reset(CreateNodeTree(mScene->mRootNode, nullptr));

		// invalid anim index
		if (mCurrentAnimIndex >= mScene->mNumAnimations)
			return;

		// create an evaluator for this animation
		mAnimEvaluator.reset(new AnimEvaluator(mScene->mAnimations[mCurrentAnimIndex]));
	}

	/** Calculates the node transformations for the scene. Call this to get
	* uptodate results before calling one of the getters.
	* @param pTime Current time. Can be an arbitrary range.
	*/
	void calculate(double pTime,std::size_t fps)
	{
		// invalid anim
		if (!mAnimEvaluator)
			return;

		// calculate current local transformations
		mAnimEvaluator->Evaluate(pTime,fps);

		// and update all node transformations with the results
		updateTransforms(mRootNode.get(), mAnimEvaluator->GetTransformations());
	}

	/** Retrieves the most recent local transformation matrix for the given node.
	*
	* The returned matrix is in the node's parent's local space, just like the
	* original node's transformation matrix. If the node is not animated, the
	* node's original transformation is returned so that you can safely use or
	* assign it to the node itsself. If there is no node with the given name,
	* the identity matrix is returned. All transformations are updated whenever
	* Calculate() is called.
	* @param pNodeName Name of the node
	* @return A reference to the node's most recently calculated local
	*   transformation matrix.
	*/
	const aiMatrix4x4& getLocalTransform(const aiNode* node) const
	{
		auto it = mNodesByName.find(node);
		if (it == mNodesByName.end())
			return mIdentityMatrix;
		return it->second->localTransform;
	}

	/** Retrieves the most recent global transformation matrix for the given node.
	*
	* The returned matrix is in world space, which is the same coordinate space
	* as the transformation of the scene's root node. If the node is not animated,
	* the node's original transformation is returned so that you can safely use or
	* assign it to the node itsself. If there is no node with the given name, the
	* identity matrix is returned. All transformations are updated whenever
	* Calculate() is called.
	* @param pNodeName Name of the node
	* @return A reference to the node's most recently calculated global
	*   transformation matrix.
	*/
	const aiMatrix4x4& getGlobalTransform(const aiNode* node) const
	{
		auto it = mNodesByName.find(node);
		if (it == mNodesByName.end())
			return mIdentityMatrix;

		return it->second->globalTransform;
	}

	/** Calculates the bone matrices for the given mesh.
	*
	* Each bone matrix transforms from mesh space in bind pose to mesh space in
	* skinned pose, it does not contain the mesh's world matrix. Thus the usual
	* matrix chain for using in the vertex shader is
	* @code
	* boneMatrix * worldMatrix * viewMatrix * projMatrix
	* @endcode
	* @param pNode The node carrying the mesh.
	* @param pMeshIndex Index of the mesh in the node's mesh array. The NODE's
	*   mesh array, not  the scene's mesh array! Leave out to use the first mesh
	*   of the node, which is usually also the only one.
	* @return A reference to a vector of bone matrices. Stays stable till the
	*   next call to GetBoneMatrices();
	*/
	const std::vector<aiMatrix4x4>& getBoneMatrices(const aiNode* pNode,
		size_t pMeshIndex = 0)
	{
		if (pMeshIndex >= pNode->mNumMeshes)
		{
			throw;
		}
		size_t meshIndex = pNode->mMeshes[pMeshIndex];
		if (meshIndex < mScene->mNumMeshes)
		{
			throw;
		}
		const auto mesh = mScene->mMeshes[meshIndex];

		// resize array and initialise it with identity matrices
		mTransforms.resize(mesh->mNumBones, aiMatrix4x4());

		// calculate the mesh's inverse global transform
		aiMatrix4x4 globalInverseMeshTransform = getGlobalTransform(pNode);
		globalInverseMeshTransform.Inverse();

		// Bone matrices transform from mesh coordinates in bind pose to mesh coordinates in skinned pose
		// Therefore the formula is offsetMatrix * currentGlobalTransform * inverseCurrentMeshTransform
		for (size_t a = 0; a < mesh->mNumBones; ++a)
		{
			const aiBone* bone = mesh->mBones[a];
			const aiMatrix4x4& currentGlobalTransform = getGlobalTransform(mBoneNodesByName[bone->mName.data]);
			mTransforms[a] = globalInverseMeshTransform * currentGlobalTransform * bone->mOffsetMatrix;
		}

		// and return the result
		return mTransforms;
	}


	/** @brief Get the current animation index
	*/
	size_t currentAnimIndex() const 
	{
		return mCurrentAnimIndex;
	}

	/** @brief Get the current animation or NULL
	*/
	aiAnimation* currentAnim() const 
	{
		return  mCurrentAnimIndex < mScene->mNumAnimations ? mScene->mAnimations[mCurrentAnimIndex] : nullptr;
	}

protected:

	/** Recursively creates an internal node structure matching the
	*  current scene and animation.
	*/
	SceneAnimNode* CreateNodeTree(aiNode* pNode, SceneAnimNode* pParent)
	{
		SceneAnimNode* internalNode = new SceneAnimNode(pNode->mName.data);
		internalNode->parent.reset(pParent);
		mNodesByName[pNode] = internalNode;

		// copy its transformation
		internalNode->localTransform = pNode->mTransformation;
		calculateGlobalTransform(internalNode);

		// find the index of the animation track affecting this node, if any
		if (mCurrentAnimIndex < mScene->mNumAnimations)
		{
			internalNode->channelIndex = -1;
			const aiAnimation* currentAnim = mScene->mAnimations[mCurrentAnimIndex];
			for (unsigned int a = 0; a < currentAnim->mNumChannels; a++)
			{
				if (currentAnim->mChannels[a]->mNodeName.data == internalNode->name)
				{
					internalNode->channelIndex = a;
					break;
				}
			}
		}

		// continue for all child nodes and assign the created internal nodes as our children
		for (unsigned int a = 0; a < pNode->mNumChildren; a++)
		{
			SceneAnimNode* childNode = CreateNodeTree(pNode->mChildren[a], internalNode);
			internalNode->children.push_back(std::unique_ptr<SceneAnimNode>(childNode));
		}

		return internalNode;
	}

	/** Recursively updates the internal node transformations from the
	*  given matrix array
	*/
	void updateTransforms(SceneAnimNode* pNode, const std::vector<aiMatrix4x4>& pTransforms)
	{
		// update node local transform
		if (pNode->channelIndex != -1)
		{
			assert(pNode->channelIndex < pTransforms.size());
			pNode->localTransform = pTransforms[pNode->channelIndex];
		}

		// update global transform as well
		calculateGlobalTransform(pNode);

		// continue for all children
		for (auto it = pNode->children.begin(); it != pNode->children.end(); ++it)
		{
			updateTransforms(it->get(), pTransforms);
		}
	}

	/** Calculates the global transformation matrix for the given internal node */
	void calculateGlobalTransform(SceneAnimNode* pInternalNode)
	{
		// concatenate all parent transforms to get the global transform for this node
		pInternalNode->globalTransform = pInternalNode->localTransform;
		auto node = pInternalNode->parent.get();
		while (node)
		{
			pInternalNode->globalTransform = node->localTransform * pInternalNode->globalTransform;
			node = node->parent.get();
		}
	}

protected:
	/** The scene we're operating on */
	const aiScene* mScene;

	/** Current animation index */
	size_t mCurrentAnimIndex;

	/** The AnimEvaluator we use to calculate the current pose for the current animation */
	std::unique_ptr<AnimEvaluator> mAnimEvaluator;

	/** Root node of the internal scene structure */
	std::unique_ptr<SceneAnimNode> mRootNode;

	/** Name to node map to quickly find nodes by their name */
	typedef std::unordered_map<const aiNode*, SceneAnimNode*> NodeMap;
	NodeMap mNodesByName;

	/** Name to node map to quickly find nodes for given bones by their name */
	typedef std::unordered_map<const char*, const aiNode*> BoneMap;
	BoneMap mBoneNodesByName;

	/** Array to return transformations results inside. */
	std::vector<aiMatrix4x4> mTransforms;

	/** Identity matrix to return a reference to in case of error */
	aiMatrix4x4 mIdentityMatrix;
};