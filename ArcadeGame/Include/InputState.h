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

#ifndef INPUT_STATE_H
#define INPUT_STATE_H

#include <vector>
#include <map>
#include <mutex>
#include "math_t.h"

using namespace math::affine;

typedef enum _mouse_buttons
{
	LBUTTON = 0,
	RBUTTON,
	MBUTTON
} MOUSE_BUTTON;

const size_t	NUM_MOUSE_BUTTONS = 3;
const size_t	NUM_KEYS = 256;

// type aliases for input maps
using key_map = bool[NUM_KEYS];
using mouse_map = bool[NUM_MOUSE_BUTTONS];

//typedef bool	key_map[NUM_KEYS];
//typedef bool	mouse_map[NUM_MOUSE_BUTTONS];


class InputState
{
	public:
		// Overload default construction to provide initialisation
		InputState();

		// Use default destruction
		~InputState() = default;


		void SetKeyDown(char ch);
		void SetKeyUp(char ch);
		void SetButtonDown(MOUSE_BUTTON b);
		void SetButtonUp(MOUSE_BUTTON b);

		void SetMouseCoordinate(const int& x, const int& y);
		POINT2 GetMouseCoordinate();

		void ClearKeyState(char ch);
		void ClearButtonState(MOUSE_BUTTON b);

		bool isActive(char ch);
		bool isActive(MOUSE_BUTTON b);

		key_map const & GetKeyMap();
		mouse_map const & GetButtonMap();

	private:
		mouse_map		m_mouse;
		key_map			m_keyboard;
		POINT2			m_mouseCoord;

		/*
		 * To make this object thread safe, we need to restrict
		 * access - we'll use mutex objects for this
		 */
		std::mutex		m_key_lock;
		std::mutex		m_mouse_lock;



};

#endif