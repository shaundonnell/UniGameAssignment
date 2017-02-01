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

#ifndef WINCANVAS_H
#define WINCANVAS_H

#ifdef WIN32
	#ifndef _WINDOWS_
		#define WIN32_LEAN_AND_MEAN
		#include <windows.h>
		// Include the Windows Library "MSIMG32.lib for Transparetn Bitmap support
		#pragma comment(lib,"Msimg32.lib")
	#endif
#endif

#include <string>
#include "Canvas.h"
#include "WinTexture.h"
#include "Geometry.h"

const short MAXBUFLEN = 256;


class WinCanvas : public Canvas/*, public Interface*/
{
	private:
		HWND		m_hWnd;				// Handle to Window Object
		HINSTANCE	m_hInstance;		// Handle to application object
		PAINTSTRUCT	m_ps;				// Paint struct for 
		HDC			m_hdcScreen;
		HDC			m_hdcBuffer;
		HBITMAP		m_hbmBuffer;
		HBITMAP		m_hbmDefault;
		short		m_sWidth;
		short		m_sHeight;
		bool		m_bExit;
		bool		m_bIsReady;
//		int			m_iMouseX;
//		int			m_iMouseY;
//		int			m_iMouseButton;
//		bool		m_bMouseSet;
//		int			m_szKeyChar;
//		bool		m_bKeySet;
//		bool		m_bKeyState[256];	// 256 Keys


		ATOM RegisterAppClass(HINSTANCE hInstance);
		BOOL InitAppInstance(HINSTANCE hInstance, int nCmdShow);

		WCHAR szClassName[MAXBUFLEN];

		friend void OpenWinCanvas(void * canvas);
		friend LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:
		WinCanvas(short x, short y, std::wstring s = L"WinCanvas");
		~WinCanvas();

		virtual void Display();
		virtual void Clear(int r, int g, int b);
		virtual void Resize();
		virtual void Size(short & x, short & y);
		virtual void Terminate() { PostQuitMessage(0); m_bExit = true; }
		virtual bool IsActive() { return !m_bExit; }
		//virtual void Attach(InputState * pIn);


		virtual void DrawLine(Segment const & s, unsigned long line_rgb = RGB(0, 0, 255), int width = 1);
		virtual void DrawRect(Rect const & r, unsigned long line_rgb = RGB(0,0,255), int line_width = 1);
		virtual void DrawCircle(Circle const & c, unsigned long line_rgb = RGB(0, 0, 255), int line_width = 1);
		virtual void DrawTriangle(Triangle const & t, unsigned long line_rgb = RGB(0, 0, 255), int width = 1);
		
		virtual void DrawSolidRect(Rect const & r, unsigned long fill_rgb = RGB(0, 0, 255), unsigned long line_rgb = RGB(0, 0, 255), int line_width = 1);
		virtual void DrawSolidCircle(Circle const & c, unsigned long fill_rgb = RGB(0, 0, 255), unsigned long line_rgb = RGB(0, 0, 255), int line_width = 1);
		virtual void DrawSolidTriangle(Triangle const & t, unsigned long fill_rgb = RGB(0, 0, 255), unsigned long line_rgb = RGB(0, 0, 255), int width = 1);

		virtual void DrawTexture(Texture const & t, POINT2f pos, POINT2f s = { 0, 0 });

		virtual void Write(int x, int y, std::string text, int r = 0, int g = 0, int b = 0);

//		void lastKey( int &k, bool &set );
//		void lastMouse( int &x, int &y, int &button, bool &set);
//		bool IsKeyPressed(int k) { return m_bKeyState[k]; }

		HDC GetBufferDC() const { return m_hdcBuffer; }

};

void OpenUIHandler(InputState & rIn);

#endif