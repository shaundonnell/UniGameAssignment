#include "Player.h"
#include "WinCanvas.h"
#include "HealthPickup.h"


Player::Player(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label, POINT2f const & position)
	: Character(width, height, ui, scene, label, position)
{
	speed = 1000.0f;
	maxSpeed = 200.0f;
	fireRate = 0.3f;
	timeSinceLastFire = 0;
	health = 5;
	color = RGB(0, 255, 255); // Cyan
}

Player::~Player()
{}

void Player::input_impl()
{
	Character::input_impl(); // Call Super function

	direction = VECTOR2f(0, 0);

	// Calculate the direction to move based on user input
	if(m_pInput->isActive('w') || m_pInput->isActive('W'))
	{
		direction = direction + VECTOR2f(0, -1);
	}
	if (m_pInput->isActive('a') || m_pInput->isActive('A'))
	{
		direction = direction + VECTOR2f(-1, 0);
	}
	if (m_pInput->isActive('s') || m_pInput->isActive('S'))
	{
		direction = direction + VECTOR2f(0, 1);
	}
	if (m_pInput->isActive('d') || m_pInput->isActive('D'))
	{
		direction = direction + VECTOR2f(1, 0);
	}

	// If we have a direction and the currently velocity is below max speed
	if(direction.length() > 0 && GetVelocity().length() < maxSpeed)
	{
		// Normalise the direction
		direction = direction / direction.length();

		SetAcceleration(direction * speed);
	}
	else
	{
		if (GetVelocity().length() > 0)
		{
			// Add some damping in the opposite direction to velocity to slow the player down
			VECTOR2f velocityDirection = GetVelocity() / GetVelocity().length();
			SetAcceleration(velocityDirection * -speed);
		}
	}

	// If we can fire again
	if (timeSinceLastFire > fireRate)
	{
		VECTOR2f fireDirection = VECTOR2f(0, 0);

		// Calculate the direction to fire based on input
		if (m_pInput->isActive(VK_UP))
		{
			fireDirection = fireDirection + VECTOR2f(0, -1);
		}
		if (m_pInput->isActive(VK_LEFT))
		{
			fireDirection = fireDirection + VECTOR2f(-1, 0);
		}
		if (m_pInput->isActive(VK_DOWN))
		{
			fireDirection = fireDirection + VECTOR2f(0, 1);
		}
		if (m_pInput->isActive(VK_RIGHT))
		{
			fireDirection = fireDirection + VECTOR2f(1, 0);
		}

		// If we have a direction then the player is pressing buttons
		if (fireDirection.length() > 0)
		{
			timeSinceLastFire = 0;
			SpawnProjectile(fireDirection, color, PLAYER, 500);
		}
	}

}

void Player::update_impl(float const & dt)
{
	Character::update_impl(dt); // Call Super function

	timeSinceLastFire += dt;
}

void Player::draw_impl(Canvas & c)
{
	Character::draw_impl(c); // Call Super function
}

void Player::OnCollision(GAMEOBJECT_PTR other)
{
	// Try to cast to a projectile
	PROJECTILE_PTR projectile = std::dynamic_pointer_cast<Projectile>(other);

	// If it is a projectile and we are able to be hit again
	if (projectile && projectile->GetType() == ENEMY && !isHit)
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

	// Try to cast to a health pickup
	HEALTHPICKUP_PTR pickup = std::dynamic_pointer_cast<HealthPickup>(other);
	if(pickup && !isHit)
	{
		health++;
		pickup->Destroy();

		// Set isHit here so the player flashes white when getting the pickup and makes the player invincible for a short amount of time
		isHit = true;
		hitTime = 0;
	}
}


