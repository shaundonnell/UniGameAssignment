#include "HealthPickup.h"
#include "WinCanvas.h"



HealthPickup::HealthPickup(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label, POINT2f const & position)
	: GameObject(width, height, ui, scene, label, position)
{
	// Pickups should not interact with other physics objects
	skipCollisions = true;
}


HealthPickup::~HealthPickup()
{
}

void HealthPickup::input_impl()
{
}

void HealthPickup::draw_impl(Canvas & c)
{
	TEXTURE_PTR heart = std::make_shared<WinTexture>("resource//heart.bmp", RGB(255, 255, 255));
	c.DrawTexture(*heart, GetPosition());
}

void HealthPickup::update_impl(float const & dt)
{
	VECTOR2f d = br - ul;
	ul = m_position;
	br.x = m_position.x + d.x;
	br.y = m_position.y + d.y;
}
