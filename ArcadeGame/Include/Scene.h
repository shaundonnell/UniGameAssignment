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

#ifndef SCENE_H
#define SCENE_H
/*
 *		The SCENE object is provided as a starting point for your
 *		implementation of an object that can hold a number of Sprite
 *		objects. Note the use of LIST container, which can hold
 *		SPRITE_PTRs. These are actually std::shared_ptr<Sprite> objects.
 *
 *		You should consider adding methods that would enable managed access
 *		to stored sprites
 */

#include <list>
#include <map>
#include <string>

#include "BaseClasses.h"

class GameObject;
class SceneManager;

using GAMEOBJECT_PTR = std::shared_ptr<GameObject>;

using ENTITY_LIST = std::map<UID_TYPE, GAMEOBJECT_PTR>;
using ENTITY_UID_LIST = std::list<UID_TYPE>;

class Scene
{
	private:

		ENTITY_LIST		m_game_entities;
		ENTITY_UID_LIST	m_game_entities_to_remove;

		POINT2			m_ul,
						m_br;

		void RunCollisionTests();
		bool CollisionCheck(GAMEOBJECT_PTR go1, GAMEOBJECT_PTR go2);

		std::shared_ptr<SceneManager> manager;

	public:

		Scene(POINT2 const & s, POINT2 const & e);
		~Scene();

		void OnUpdate(float const & dt);
		void OnDraw(Canvas & c);
		void OnInput();

		bool Load(std::string const & filename);
		void Clear();

		GAMEOBJECT_PTR Find(UID_TYPE const & uid);

		void AddEntity(GAMEOBJECT_PTR pEntity);

		GAMEOBJECT_PTR RemoveEntity(UID_TYPE const & uid);
		void MarkObjectForRemoval(UID_TYPE const & uid);

		void SetSceneManager(std::shared_ptr<SceneManager> _manager);

		int GetWidth();
		int GetHeight();
};

#endif