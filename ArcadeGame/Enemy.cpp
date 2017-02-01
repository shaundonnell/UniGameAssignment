#include "Enemy.h"
#include "WinCanvas.h"
#include "Projectile.h"


Enemy::Enemy(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label, POINT2f const & position)
	: Character(width, height, ui, scene, label, position)
{
	color = RGB(255, 0, 0); // Red
	health = 3;

	// Randomise fire rate between 0.5f and 1.5f for variety
	float modifier = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	fireRate = 1.0f + (modifier * 0.5f - 0.25f);

	lastFire = 0;
	fireTarget = nullptr;
}


Enemy::~Enemy()
{
}

void Enemy::input_impl()
{
	Character::input_impl(); // Call Super function
}

void Enemy::draw_impl(Canvas & c)
{

	Character::draw_impl(c); // Call Super function
}

void Enemy::update_impl(float const & dt)
{
	Character::update_impl(dt); // Call Super function

	lastFire += dt;

	if(lastFire > fireRate && fireTarget != nullptr && !fireTarget->IsDestroyed())
	{
		lastFire = 0;
		VECTOR2f direction = fireTarget->GetPosition() + (GetPosition() * -1);
		direction = direction / direction.length();
		SpawnProjectile(direction, color, ENEMY, 500);
	}
}

void Enemy::OnCollision(GAMEOBJECT_PTR other)
{
	// Try to cast to a projectile
	PROJECTILE_PTR projectile = std::dynamic_pointer_cast<Projectile>(other);

	// If it is a projectile and we are able to be hit again
	if(projectile && projectile->GetType() == PLAYER && !isHit)
	{
		projectile->Destroy();

		isHit = true;
		hitTime = 0;

		health -= 1;

		if (health <= 0)
		{
			Destroy();
		}
	}
}

void Enemy::SetFireTarget(GAMEOBJECT_PTR target)
{
	fireTarget = target;
}
