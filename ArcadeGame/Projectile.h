#pragma once
#include "GameObject.h"

enum ProjectileType
{
	PLAYER,
	ENEMY
};

class Projectile : public GameObject
{
public:
	Projectile(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label = ID_UNKNOWN, POINT2f const & position = { 0, 0 });
	~Projectile();

	ProjectileType GetType();
	void SetType(ProjectileType _type);

	long GetColor();
	void setColor(long color);

private:
	// Implement inherited private implementation
	void input_impl() override;
	void draw_impl(Canvas & c) override;
	void update_impl(float const & dt) override;

	ProjectileType type;
	long color;
};

using PROJECTILE_PTR = std::shared_ptr<Projectile>;

