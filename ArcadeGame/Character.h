#pragma once
#include "GameObject.h"
#include "WinCanvas.h"
#include "Projectile.h"

class Character : public GameObject
{
public:
	Character(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label = ID_UNKNOWN, POINT2f const & position = { 0, 0 });
	~Character();

	// Implement inherited private implementation
	void input_impl() override;
	void draw_impl(Canvas & c) override;
	void update_impl(float const & dt) override;

	int GetHealth();

protected:
	COLORREF color;
	int health;
	float invincibleTime;
	bool isHit;
	float hitTime;

	void SpawnProjectile(VECTOR2f direction, long color, ProjectileType type, float speed);
};

