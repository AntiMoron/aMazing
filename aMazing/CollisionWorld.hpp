#pragma once
#include<cstdio>
#include<memory>
#include"CollisionHeader.h"
#include"ContactListener.hpp"
class CollisionWorld
{
public:
	CollisionWorld()
	{
		world = nullptr;
		cameraBody = nullptr;
	}
	~CollisionWorld(){}

	HRESULT Initialize()
	{
		world = new b2World(b2Vec2(0.0f, -10.0f), true);
		ContactListener listener;
		world->SetContactListener(&listener);
		setUpCamera(0.0f, 0.0f, 0.1);
		addBox(0.0f, 0.0f, 100.0f, 100.0f);
		return S_OK;
	}

	void Shutdown()
	{
		//clear up Blocks' bodies.
		for (auto& body : mazeBlocks)
		{
			world->DestroyBody(body);
		}
		mazeBlocks.clear();
		//clear up Spheres' bodies
		for (auto& body : mazeSpheres)
		{
			world->DestroyBody(body);
		}
		mazeSpheres.clear();
		//clear up Camera's body
		if(cameraBody != nullptr)
		{
			world->DestroyBody(cameraBody);
			cameraBody = nullptr;
		}
		if (world != nullptr)
		{
			delete world;
			world = nullptr;
		}
	}

	void updateCameraPos(float centerX,float centerZ,float rotation)
	{
		cameraBody->SetTransform(b2Vec2(centerX, centerZ), rotation * 0.01745329251994);
	}

	bool setUpCamera(float centerX,float centerZ,float radius)
	{
		b2BodyDef mazeBlockDef = {};
		mazeBlockDef.type = b2_kinematicBody;
		mazeBlockDef.position.Set(centerX, centerZ);
		mazeBlockDef.angle = 0.0f;
		b2Body* dynamicBody = world->CreateBody(&mazeBlockDef);

		b2CircleShape sphereShape;
		sphereShape.m_p.Set(0, 0);
		sphereShape.m_radius = radius;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &sphereShape;
		fixtureDef.density = 1.0f;
		dynamicBody->CreateFixture(&fixtureDef);

		dynamicBody->SetUserData(dynamicBody);
		cameraBody = dynamicBody;
		printf("%d %f %f\n", world->GetBodyCount(), dynamicBody->GetPosition().x, dynamicBody->GetPosition().y);
		return 0;
	}

	bool addSphere(float centerX,float centerZ,float radius)
	{
		b2BodyDef mazeBlockDef = {};
		mazeBlockDef.type = b2_staticBody;
		mazeBlockDef.position.Set(centerX, centerZ);
		mazeBlockDef.angle = 0.0f;
		b2Body* dynamicBody = world->CreateBody(&mazeBlockDef);

		b2CircleShape sphereShape;
		sphereShape.m_p.Set(0, 0);
		sphereShape.m_radius = radius;
		b2FixtureDef fixtureDef = {};
		fixtureDef.shape = &sphereShape;
		fixtureDef.density = 1.0f;
		dynamicBody->CreateFixture(&fixtureDef);

		dynamicBody->SetUserData(dynamicBody);
		mazeSpheres.emplace_back(dynamicBody);
		return true;
	}

	bool addBox(float centerX,float centerZ,float extX,float extZ)
	{
		b2BodyDef mazeBlockDef = {};
		mazeBlockDef.type = b2_staticBody;
		mazeBlockDef.position.Set(centerX, centerZ);
		mazeBlockDef.angle = 0.0f;
		b2Body* staticBody = world->CreateBody(&mazeBlockDef);

		b2PolygonShape boxShape;
		boxShape.SetAsBox(extX, extZ);
		b2FixtureDef fixtureDef = {};
		fixtureDef.shape = &boxShape;
		fixtureDef.density = 1.0f;
		staticBody->CreateFixture(&fixtureDef);

		staticBody->SetUserData(staticBody);
		mazeBlocks.emplace_back(staticBody);
		return true;
	}

	//returns the index of un static body.
	//if index == 0 is camera
	//else is mazeSpheres[index - 1]
	b2Vec2 getNewState()
	{
		;
		for (auto& block : mazeBlocks)
		{
			if (block->GetFixtureList()->GetShape()->TestPoint(block->GetTransform(), cameraBody->GetPosition()));
//				printf("_");
		}
		for (b2ContactEdge* edge = cameraBody->GetContactList();
			edge != nullptr;
			edge = edge->next)
		{
			printf("The Collision is: ");
			if (edge->contact->IsTouching())
			{
				printf("%d\n",1);
			}
			else printf("0\n");
		}
		printf("%f %f\n", cameraBody->GetTransform().position.x, cameraBody->GetTransform().position.y);
		return b2Vec2(0.0f, 0.0f);
	}
private:
	b2World* world;
	b2Body* cameraBody;
	std::vector<b2Body*> mazeSpheres;
	std::vector<b2Body*> mazeBlocks;
};
