#pragma once
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "Wall.h"
#include "HealthPickup.h"

using ENEMY_LIST = std::list<ENEMY_PTR>;
using WALL_LIST = std::list<WALL_PTR>;
using PICKUP_LIST = std::list<HEALTHPICKUP_PTR>;

class SceneManager : public GameObject
{
public:
	SceneManager(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label = ID_UNKNOWN, POINT2f const & position = { 0, 0 });
	~SceneManager();

	// Implement inherited private implementation
	void input_impl() override;
	void draw_impl(Canvas & c) override;
	void update_impl(float const & dt) override;

	void EntityDestroyed(GAMEOBJECT_PTR entity);

private:
	PLAYER_PTR player;
	int level;

	ENEMY_LIST enemyList;
	WALL_LIST wallList;
	PICKUP_LIST pickupList;

	bool gameOver;
	bool playerWon;

	void LoadLevel();
	void GameOver();
	void YouWin();
	void ClearLevel();
};

using SCENEMANAGER_PTR = std::shared_ptr<SceneManager>;

