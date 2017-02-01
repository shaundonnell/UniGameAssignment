#pragma once
#include "Character.h"

class Enemy : public Character
{
public:
	Enemy(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label = ID_UNKNOWN, POINT2f const & position = { 0, 0 });
	~Enemy();

	// Implement inherited private implementation
	void input_impl() override;
	void draw_impl(Canvas & c) override;
	void update_impl(float const & dt) override;

	void OnCollision(GAMEOBJECT_PTR other) override;

	void SetFireTarget(GAMEOBJECT_PTR target);

	float fireRate;
	float lastFire;

private:
	GAMEOBJECT_PTR fireTarget;
};

using ENEMY_PTR = std::shared_ptr<Enemy>;

