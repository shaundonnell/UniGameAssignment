#include "Projectile.h"
#include "WinCanvas.h"


Projectile::Projectile(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label, POINT2f const & position)
	: GameObject(width, height, ui, scene, label, position)
{
}


Projectile::~Projectile()
{
}

ProjectileType Projectile::GetType()
{
	return type;
}

void Projectile::SetType(ProjectileType _type)
{
	type = _type;
}

long Projectile::GetColor()
{
	return color;
}

void Projectile::setColor(long _color)
{
	color = _color;
}

void Projectile::input_impl()
{
}

/*
*		The Block implementaiton method for draw, which is called
*		from the base object (Shape) draw method.
*/
void Projectile::draw_impl(Canvas & c)
{
	// Use the GraphicsCanvas API to draw a solid Block in 'RED'
	c.DrawSolidRect(*this, color);
}

void Projectile::update_impl(float const & dt)
{
	if (m_velocity.length() > 0)
	{
		// Add some damping in the opposite direction to velocity to slow the projectile down
		VECTOR2f velocityDirection = m_velocity / m_velocity.length();
		SetAcceleration(velocityDirection * -200);
	}

	m_velocity = m_velocity + (m_acceleration * dt);
	m_position = m_position + (GetVelocity() * dt);

	VECTOR2f d = br - ul;
	ul = m_position;
	br.x = m_position.x + d.x;
	br.y = m_position.y + d.y;

	// Destroy once the projectile slows down engouh
	if(m_velocity.length() < 100)
	{
		Destroy();
	}

}
