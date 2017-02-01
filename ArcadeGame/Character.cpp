#include "Character.h"
#include "Projectile.h"



Character::Character(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label, POINT2f const & position)
	: GameObject(width, height, ui, scene, label, position)
{
	hitTime = 0;
	isHit = false;
	invincibleTime = 0.1f;
}


Character::~Character()
{
}

void Character::input_impl()
{
}

void Character::draw_impl(Canvas & c)
{
	if(isHit)
	{
		// Flash white when hit
		c.DrawSolidRect(*this, RGB(255, 255, 255));
	}
	else
	{
		c.DrawSolidRect(*this, color);
	}
}

void Character::update_impl(float const & dt)
{
	// Calculate velocity and position
	m_velocity = m_velocity + (m_acceleration * dt);
	m_position = m_position + (m_velocity * dt);

	// Move the character
	VECTOR2f d = br - ul;
	ul = m_position;
	br.x = m_position.x + d.x;
	br.y = m_position.y + d.y;

	// Update hit time
	if (hitTime > invincibleTime)
	{
		isHit = false;
	}
	else
	{
		hitTime += dt;
	}
}

int Character::GetHealth()
{
	return health;
}

void Character::SpawnProjectile(VECTOR2f direction, long color, ProjectileType type, float speed)
{
	auto projectile = Instantiate<Projectile>(10, 10);
	projectile->SetPosition(GetPosition() + VECTOR2f(5, 5) + (direction * 20));
	projectile->setColor(color);
	projectile->SetType(type);

	VECTOR2f normalisedDirection = direction / direction.length();

	projectile->SetVelocity(normalisedDirection * speed);
}
