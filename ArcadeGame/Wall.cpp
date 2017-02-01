#include "Wall.h"
#include "WinCanvas.h"


Wall::Wall(unsigned int width, unsigned int height, InputState* ui, Scene* scene, LABEL_TYPE label, POINT2f const & position)
	: GameObject(width, height, ui, scene, label, position)
{
	// Walls cannot move
	isStatic = true;
}


Wall::~Wall()
{
}

void Wall::input_impl()
{
}

void Wall::draw_impl(Canvas & c)
{
	c.DrawSolidRect(*this, RGB(30, 40, 40));
}

void Wall::update_impl(float const & dt)
{
	VECTOR2f d = br - ul;
	ul = m_position;
	br.x = m_position.x + d.x;
	br.y = m_position.y + d.y;
}
