/* ********************************************************************************* *
* *  COPYRIGHT NOTICE                                                             * *
* *  ----------------                                                             * *
* *  (C)[2012] - [2015] Deakin University                                         * *
* *  All rights reserved.                                                         * *
* *  All information contained herein is, and remains the property of Deakin      * *
* *  University and the authors(Tim Wilkin, Michael Hobbs and Shaun Bangay).      * *
* *  Dissemination of this information or reproduction of this material is        * *
* *  strictly forbidden unless prior written permission is obtained from Deakin   * *
* *  University.The right to create derivative works from this material is        * *
* *  hereby granted to students enrolled in SIT153, but only for the purposes of  * *
* *  assessment while an enrolled student at Deakin University.                   * *
* *                                                                               * *
* ********************************************************************************* */


#include "BaseClasses.h"

UID_TYPE GameEntity::s_uNextValidUID = 1;


/*
 * GameEntity method definitions
 */
GameEntity::GameEntity(LABEL_TYPE _t)
  : m_uid(s_uNextValidUID++),
  m_type(_t)
{
}


GameEntity::~GameEntity()
{
}

UID_TYPE const & GameEntity::GetUID() const
{
	return m_uid;
}

LABEL_TYPE const & GameEntity::GetType() const
{
	return m_type;
}

void Dynamic::SetVelocity(VECTOR2f const & v)
{
	m_velocity = v;
}

void Dynamic::SetAcceleration(VECTOR2f const& a)
{
	m_acceleration = a;
}



/*
 * Sprite method definitions
 */
Sprite::Sprite(POINT2f const & p)
: m_position(p)
{}

void Sprite::OnDraw(Canvas & c)
{
	draw_impl(c);

}

POINT2f const & Sprite::GetPosition() const
{
	return m_position;
}

void Sprite::SetPosition(POINT2f const & pos)
{
	m_position = pos;
}


/*
 * Dynamic method definitions
 */
Dynamic::Dynamic(VECTOR2f const & v)
	: m_velocity(v)
{}

VECTOR2f const & Dynamic::GetVelocity() const
{
	return m_velocity;
}

VECTOR2f const& Dynamic::GetAcceleration() const
{
	return m_acceleration;
}

void Dynamic::OnUpdate(float const & dt)
{
	update_impl(dt);
}

/*
 * Controllable method definitions
 */
Controllable::Controllable(InputState * pIS)
: m_pInput(pIS)
{}

Controllable::~Controllable()
{
}

void Controllable::Attach(InputState * pIS)
{
	m_pInput = pIS;
}

void Controllable::OnInput()
{
	if (m_pInput != nullptr)
		input_impl();
}
