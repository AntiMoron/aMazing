#pragma once
#include<cstdio>
#include<vector>
#include<iostream>
#include<memory>
#include"CollisionIntersection.hpp"

namespace aMazing
{
	class CollisionWorld
	{
	public:
		CollisionWorld()
		{
		}
		~CollisionWorld(){}

		HRESULT Initialize()
		{
			camera.reset(new CollisionCircle);
			setUpCamera(0.0f, 0.0f, 0.0005f);
			return S_OK;
		}

		void updateCameraPos(float centerX, float centerZ, float rotation)
		{
			camera->setCenter(XMFLOAT2(centerX, centerZ));
		}

		bool setUpCamera(float centerX, float centerZ, float radius)
		{
			camera->setCenter(XMFLOAT2(centerX, centerZ));
			camera->setRadius(radius);
			return 0;
		}

		bool addSphere(float centerX, float centerZ, float radius)
		{
			return true;
		}

		bool addBox(float centerX, float centerZ, float extX, float extZ)
		{
			CollisionRectangle* box = new CollisionRectangle;
			std::array<XMFLOAT2, 2> data = {
				XMFLOAT2(centerX - extX, centerZ - extZ),
				XMFLOAT2(centerX + extX, centerZ + extZ) };
			box->setPoints(data);
			rectangles.push_back(std::unique_ptr<CollisionRectangle>(box));
			return true;
		}

		//returns the index of un static body.
		//if index == 0 is camera
		//else is mazeSpheres[index - 1]
		std::pair<float, float> getNewState()
		{
			contactHandler();
			return std::pair<float, float>(0.0f, 0.0f);
		}
	private:
		void CALLBACK contactHandler()
		{
			bool flag = false;
			for (auto& rec : rectangles)
			{
				if (IntersectRectangleCircle(rec.get(), camera.get()))
				{
					flag = true;
					//				printf("hit\n");
				}
			}
		}
		std::shared_ptr<CollisionCircle> camera;
		std::vector<std::unique_ptr<CollisionRectangle> > rectangles;
	};
}
