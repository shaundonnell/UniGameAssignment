#pragma once
#include "Canvas.h"
#include "BaseClasses.h"
#include "Character.h"

class Player : public Character
{
private:
	// Implement inherited private implementation
	void input_impl() override;
	void draw_impl(Canvas & c) override;
	void update_impl(float const & dt) override;

	float speed;
	float maxSpeed;
	VECTOR2f direction;
	float fireRate;
	float timeSinceLastFire;

	void OnCollision(GAMEOBJECT_PTR other) override;

public:
	Player(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label = ID_UNKNOWN, POINT2f const & position = { 0, 0 });
	~Player();

};

using PLAYER_PTR = std::shared_ptr<Player>;

