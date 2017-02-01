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

#ifndef BASE_H
#define BASE_H

#include <memory>

#include "math_t.h"
#include "Canvas.h"
#include "Geometry.h"
#include "InputState.h"
#include "GameTypes.h"

using namespace math::affine;

/*
 *  GameEntity:
 *       Abstract base class for all game object classes
 *       Provides unique id and type label
 */
class GameEntity
{
	private:
		// Each GameEntity has a unique ID
		UID_TYPE			m_uid;
		LABEL_TYPE			m_type;

		// GameEntity base class is responsible for assigning next valid ID
		static UID_TYPE		s_uNextValidUID;

	protected:

		// GameEntity objects can only be constructed by derived class objects
		explicit GameEntity(LABEL_TYPE _t);

		// Ensure default copy and assignment but only through derived class objects
		GameEntity(GameEntity const &) = default;
		GameEntity & operator=(GameEntity const &) = default;


	public:
		virtual ~GameEntity();

		// Report Unique ID for this object
		UID_TYPE const & GetUID() const;
		LABEL_TYPE const & GetType() const;

};


/*
*  Sprite:
*       Abstract base class for drawable object classes
*       Provides basic drawable state (screen position)
*       as well as state accessors and mutators, and game core
*       function draw
*       Applies PIMPL idiom as demonstration
*/
class Sprite
{
private:

	virtual void draw_impl(Canvas & c) = 0;

protected:
	// Ensure that Sprite objects can only be constructed by derived classes
	explicit Sprite(POINT2f const & p = { 0, 0 });

	// All Sprite objects have a position state variable
	POINT2f	 m_position;


	// Sprite objects can only be copied by derived class objects
	Sprite(const Sprite&) = default;
	Sprite& operator=(const Sprite&) = default;

public:

	virtual ~Sprite(){}

	void OnDraw(Canvas & c);

	void SetPosition(POINT2f const & pos);
	POINT2f const & GetPosition() const;

};


/*
*  Controllable:
*       Abstract base class for all controllable object classes
*       Provides connection to input state buffer, with mutator
*		for resetting this connection. Provides input method for
*		core game function.
*		Applies PIMPL idiom as demonstration
*/
class Controllable
{
	private:
		virtual void input_impl() = 0;

	protected:
		InputState * m_pInput;

		// Controllable objects can only be constructed by derived class objects
		explicit Controllable(InputState * = nullptr);

		// Controllable objects can only by copied or assigned through derived class objects
		Controllable(Controllable const &) = default;
		Controllable & operator=(Controllable const &) = default;

	public:
		virtual ~Controllable();

		void Attach(InputState *);
		void OnInput();
};

/*
*  Dynamic:
*       Abstract base class for all dynamic object classes
*       Provides velocity state, with accessor and mutator
*		Also provides update method for cord game function
*		Applies PIMPL idiom as demonstration
*/
class Dynamic
{
	private:
		virtual void update_impl(float const & dt) = 0;

	protected:
		VECTOR2f	m_velocity;
		VECTOR2f	m_acceleration;

		// Dynamic objects can only be created through derived classes
		explicit Dynamic(VECTOR2f const & v = { 0, 0 });

		Dynamic(Dynamic const &) = default;
		Dynamic & operator=(Dynamic const &) = default;

	public:
		void OnUpdate(float const & dt);

		void Dynamic::SetVelocity(VECTOR2f const & v);
		VECTOR2f const & Dynamic::GetVelocity() const;

		void Dynamic::SetAcceleration(VECTOR2f const & a);
		VECTOR2f const & Dynamic::GetAcceleration() const;
};

// Define type alias using std::shared_ptr objects for 
// (reference counted) managed dynamic memory

using SPRITE_PTR = std::shared_ptr<Sprite>;
using ENTITY_PTR = std::shared_ptr<GameEntity>;
using DYNAMIC_PTR = std::shared_ptr<Dynamic>;
using CONTROL_PTR = std::shared_ptr<Controllable>;

#endif