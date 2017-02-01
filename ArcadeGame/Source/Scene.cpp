/* ********************************************************************************* *
 * *  COPYRIGHT NOTICE                                                             * *
 * *  ----------------                                                             * *
 * *  (C)[2012] - [2015] Deakin University                                         * *
 * *  All rights reserved.                                                         * *
 * *  All information contained herein is, and remains the property of Deakin      * *
 * *  University and the authors(Tim Wilkin, Michael Hobbs and Shaun Bangay).      * *
 * *  Dissemination of this information or reproduction of this material is        * *
 * *  strictly forbidden unless prior written permission is obtained from Deakin   * *
 * *  University.The right to create derivative works from this material is        * *
 * *  hereby granted to students enrolled in SIT153, but only for the purposes of  * *
 * *  assessment while an enrolled student at Deakin University.                   * *
 * *                                                                               * *
 * ********************************************************************************* */

#include "Scene.h"
#include "../GameObject.h"
#include "../SceneManager.h"
#include <complex>

Scene::Scene(POINT2 const & s, POINT2 const & e)
	: m_ul(s),
	  m_br(e),
	  m_game_entities(),
	m_game_entities_to_remove()
{}

Scene::~Scene()
{
	Clear();
}

int Scene::GetWidth()
{
	return (int)(m_br.x - m_ul.x);
}
int Scene::GetHeight()
{
	return (int)(m_br.y - m_ul.y);
}

void Scene::OnInput()
{
	for (auto c : m_game_entities)
		c.second->OnInput();
}

void Scene::OnUpdate(float const & dt)
{
	// Update any sprite object that requires it
	for (auto d : m_game_entities)
		d.second->OnUpdate(dt);

	for (auto uid : m_game_entities_to_remove)
		RemoveEntity(uid);

	// resolve collisions
	RunCollisionTests();

	// do any other update actions (such as remove dead sprites)
}

void Scene::OnDraw(Canvas & c)
{
	for (auto s : m_game_entities)
	{
		if (s.second == manager)
			continue; // Skip for now so the scene manager draws last

		s.second->OnDraw(c);
	}

	manager->OnDraw(c);
}

bool Scene::Load(std::string const & filename)
{
	/*
	 * Add functionality here to load a set of sprites from the given file
	 */


	return false; // if failed to load
}
void Scene::AddEntity(GAMEOBJECT_PTR pEntity)
{
	m_game_entities.insert( std::make_pair(pEntity->GetUID(), pEntity) );
}

void Scene::MarkObjectForRemoval(UID_TYPE const & uid)
{
	m_game_entities_to_remove.push_back(uid);
}

void Scene::SetSceneManager(SCENEMANAGER_PTR _manager)
{
	manager = _manager;
}

GAMEOBJECT_PTR Scene::RemoveEntity(UID_TYPE const & uid)
{
	GAMEOBJECT_PTR p = nullptr;
	auto iter = m_game_entities.find(uid);
	if (iter != m_game_entities.end())
	{
		p = Find(uid);
		m_game_entities.erase(iter);
		manager->EntityDestroyed(p);
	}

	return p;
}


GAMEOBJECT_PTR Scene::Find(UID_TYPE const & uid)
{
	auto iter = m_game_entities.find(uid);
	if (iter != m_game_entities.end())
		return iter->second;
	else
		return nullptr;
}



void Scene::Clear()
{
	m_game_entities.clear();
}

void Scene::RunCollisionTests()
{

	for (auto e_i : m_game_entities)
	{
		/*
		 * Test for a collision between this Sprite and boundary
		 */

		GAMEOBJECT_PTR go = e_i.second;

		if(go->IsStatic())
			continue; // We don't need to check for collision on object that don't move

		if (!go->SkipCollisions()) // Skip kenimatic game objects
		{

			if (go->GetPosition().x <= 0)
			{
				go->SetPosition(VECTOR2f(1, go->GetPosition().y));
				go->SetVelocity(VECTOR2f(go->GetVelocity().x * -1, go->GetVelocity().y));
			}

			if (go->GetPosition().y <= 0)
			{
				go->SetPosition(VECTOR2f(go->GetPosition().x, 1));
				go->SetVelocity(VECTOR2f(go->GetVelocity().x, go->GetVelocity().y * -1));
			}
			if (go->GetPosition().x + go->GetWidth() >= m_br.x)
			{
				go->SetPosition(VECTOR2f(m_br.x - go->GetWidth() - 1, go->GetPosition().y));
				go->SetVelocity(VECTOR2f(go->GetVelocity().x * -1, go->GetVelocity().y));
			}

			if (go->GetPosition().y + go->GetHeight() >= m_br.y)
			{
				go->SetPosition(VECTOR2f(go->GetPosition().x, m_br.y - go->GetHeight() - 1));
				go->SetVelocity(VECTOR2f(go->GetVelocity().x, go->GetVelocity().y * -1));
			}
		}

		/*
		 * Test for a collision between this Sprite and each other Sprite
		 */

		for (auto e_j : m_game_entities)
		{
			if (e_i.first == e_j.first) // don't test a Sprite against itself
				continue;


			GAMEOBJECT_PTR go2 = e_j.second;

			PROJECTILE_PTR proj1 = std::dynamic_pointer_cast<Projectile>(go);
			PROJECTILE_PTR proj2 = std::dynamic_pointer_cast<Projectile>(go2);

			if(proj1 && proj2)
				continue; // Projectiles don't interact with eachother

			if (CollisionCheck(go, go2))
			{
				if (!go->SkipCollisions() && !go2->SkipCollisions()) // Don't calculate collisons if skipping
				{

					VECTOR2f direction = go->GetCentre() - go2->GetCentre();

					int moveBackAmount = 3;

					// Normalise
					//direction = direction / direction.length();
					if (std::abs(direction.x) > std::abs(direction.y))
					{
						// Reverse X velocity
						int xDirection = direction.x > 0 ? 1 : -1;

						if (!go->IsStatic()) // Don't move static objects
						{
							go->SetPosition(go->GetPosition() + VECTOR2f(xDirection * moveBackAmount, 0));
							go->SetVelocity(VECTOR2f(std::abs(go->GetVelocity().x) * xDirection, go->GetVelocity().y));
						}

						if (!go2->IsStatic()) // Don't move static objects
						{
							go2->SetPosition(go2->GetPosition() + VECTOR2f(xDirection * moveBackAmount * -1, 0));
							go2->SetVelocity(VECTOR2f(std::abs(go2->GetVelocity().x) * xDirection * -1, go2->GetVelocity().y));
						}
					}
					else
					{
						// Reverse Y velocity
						int yDirection = direction.y > 0 ? 1 : -1;

						if (!go->IsStatic()) // Don't move static objects
						{
							go->SetPosition(go->GetPosition() + VECTOR2f(0, yDirection * moveBackAmount));
							go->SetVelocity(VECTOR2f(go->GetVelocity().x, std::abs(go->GetVelocity().y) * yDirection));
						}

						if (!go2->IsStatic()) // Don't move static objects
						{
							go2->SetPosition(go2->GetPosition() + VECTOR2f(0, yDirection * moveBackAmount * -1));
							go2->SetVelocity(VECTOR2f(go2->GetVelocity().x, std::abs(go2->GetVelocity().y) * yDirection * -1));
						}
					}
				}

				// Call on collision event methods
				go->OnCollision(go2);
				go2->OnCollision(go);

			}


		}
	}
}

// Reference: http://lazyfoo.net/SDL_tutorials/lesson17/
bool Scene::CollisionCheck(GAMEOBJECT_PTR go1, GAMEOBJECT_PTR go2)
{
	int goLeft = go1->GetPosition().x;
	int goRight = go1->GetPosition().x + go1->GetWidth();
	int goTop = go1->GetPosition().y;
	int goBottom = go1->GetPosition().y + go1->GetHeight();

	int go2Left = go2->GetPosition().x;
	int go2Right = go2->GetPosition().x + go2->GetWidth();
	int go2Top = go2->GetPosition().y;
	int go2Bottom = go2->GetPosition().y + go2->GetHeight();

	if(goBottom <= go2Top)
	{
		return false;
	}

	if(goTop >= go2Bottom)
	{
		return false;
	}

	if(goRight <= go2Left)
	{
		return false;
	}

	if(goLeft >= go2Right)
	{
		return false;
	}

	return true;
}

