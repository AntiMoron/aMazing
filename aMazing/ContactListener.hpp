#pragma once

#include"Box2D/Box2D.h"

class ContactListener : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact)
	{
		if (contact && contact->IsTouching())
		{
			printf("hahahaha");
			b2Fixture* A = contact->GetFixtureA();
			b2Fixture* B = contact->GetFixtureB();
			
			if (A && B)
			{
				A->GetBody();
			}
		}
	}

	void EndContact(b2Contact* contact)
	{
		;
	}
};