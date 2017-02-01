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

#include "InputState.h"

/*
 *		The InputState class provides an interface to the keyboard
 *		and mouse devices. It facilitates storage of key and mouse
 *      state data which is set by the message pump in the WinCanvas
 *		execution thread. Input state data is read by the game execution
 *      thread. Since memory is being shared across threads we must
 *      prevent race conditions using mutex variables and lock objects.
 *		A 'lock' is aquired from the operating system to ensure only 
 *		one thread can be access the state data at once (for read or write).
 */

InputState::InputState()
	: m_key_lock(),
	  m_mouse_lock()
{
	memset(&m_keyboard, false, sizeof(bool)*NUM_KEYS);
	memset(&m_mouse, false, sizeof(bool)*NUM_MOUSE_BUTTONS);
}


void InputState::SetKeyDown(char ch)
{
	std::unique_lock<std::mutex>	locked(m_key_lock);
	m_keyboard[ch] = true;
}

void InputState::SetKeyUp(char ch)
{
	std::unique_lock<std::mutex>	locked(m_key_lock);
	m_keyboard[ch] = false;
}

void InputState::SetButtonDown(MOUSE_BUTTON b)
{
	std::unique_lock<std::mutex>	locked(m_mouse_lock);
	m_mouse[b] = true;
}

void InputState::SetButtonUp(MOUSE_BUTTON b)
{
	std::unique_lock<std::mutex>	locked(m_mouse_lock);
	m_mouse[b] = false;
}

void InputState::ClearKeyState(char ch)
{
	SetKeyUp(ch);
}

void InputState::ClearButtonState(MOUSE_BUTTON b)
{
	SetButtonUp(b);
}

bool InputState::isActive(char ch)
{
	std::unique_lock<std::mutex>	locked(m_key_lock);
	return m_keyboard[ch];
}

bool InputState::isActive(MOUSE_BUTTON b)
{
	std::unique_lock<std::mutex>	locked(m_mouse_lock);
	return m_mouse[b];
}

key_map const & InputState::GetKeyMap()
{
	std::unique_lock<std::mutex>	locked(m_key_lock);
	return m_keyboard;
}

mouse_map const & InputState::GetButtonMap()
{
	std::unique_lock<std::mutex>	locked(m_mouse_lock);
	return m_mouse;
}

void InputState::SetMouseCoordinate(const int& x, const int& y)
{
	std::unique_lock<std::mutex>	locked(m_mouse_lock);
	m_mouseCoord.x = x;
	m_mouseCoord.y = y;
}
POINT2 InputState::GetMouseCoordinate()
{
	std::unique_lock<std::mutex>	locked(m_mouse_lock);
	return m_mouseCoord;
}

