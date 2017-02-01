#include <iostream>
#include "process.h"
#include <WindowsX.h>
#include <string>

#include "WinCanvas.h"
#include "InputState.h"

static WinCanvas * s_pCanvas;
static InputState	 * s_pInput;

LRESULT CALLBACK MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_PAINT:
			break;

		case WM_SIZE:
			if (s_pCanvas != nullptr)
				s_pCanvas->Resize();
			break;

		case WM_KEYDOWN:
			{
				if (s_pInput != nullptr)
					s_pInput->SetKeyDown(wParam);
			}
			break;
		
		case WM_KEYUP:
			{
				if (s_pInput != nullptr)
					s_pInput->SetKeyUp(wParam);

			}
			break;
		case WM_LBUTTONDOWN:
			if (s_pInput != nullptr)
				s_pInput->SetButtonDown(LBUTTON);
			break;

		case WM_RBUTTONDOWN:
			if (s_pInput != nullptr)
				s_pInput->SetButtonDown(RBUTTON);
			break;

		case WM_MBUTTONDOWN:
			{
				if (s_pInput != nullptr)
				{
					s_pInput->SetButtonDown(MBUTTON);
					s_pInput->SetMouseCoordinate(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
				}
			}
			break;

		case WM_LBUTTONUP:
			if (s_pInput != nullptr)
				s_pInput->SetButtonUp(LBUTTON);
			break;
		case WM_RBUTTONUP:
			if (s_pInput != nullptr)
				s_pInput->SetButtonUp(RBUTTON);
			break;

		case WM_MBUTTONUP:
			if (s_pInput != nullptr)
				s_pInput->SetButtonUp(MBUTTON);
			break;

		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	};

	// if we successfully handle a message we return 0
	return 0;
}

void OpenUIHandler(InputState & rIn)
{
	// We need access to the input device created in the other thread, so
	// we store the address of the argument InputState object in a global
	// static pointer. NOT a very safe way of handling this... but this
	// *is* a demo project and NOT a commercial code base!
	s_pInput = &rIn;
}

void OpenWinCanvas(void * canvas)
{
	WinCanvas * pCanvas = (WinCanvas*)canvas;
	s_pInput = nullptr;
	char szTitle[MAXBUFLEN];
	GetConsoleTitleA(szTitle, MAXBUFLEN);
	HWND hWnd = FindWindowA(NULL,szTitle);
	HINSTANCE hInstance = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

	ATOM ar = pCanvas->RegisterAppClass(hInstance);

	if (FAILED(ar))
		std::cout << "Error: Unable to register Win32 Canvas Applet" << std::endl;

	pCanvas->InitAppInstance(hInstance,SW_SHOWNORMAL);

	// Create device context for drawing
	pCanvas->m_hdcScreen = BeginPaint(pCanvas->m_hWnd,&(pCanvas->m_ps));

	// Create drawing buffers/bitmaps for double buffering display
	RECT wRect;
	pCanvas->m_hdcBuffer = CreateCompatibleDC(pCanvas->m_hdcScreen);
	GetClientRect(pCanvas->m_hWnd, &wRect);
	pCanvas->m_hbmBuffer = CreateCompatibleBitmap(pCanvas->m_hdcScreen, wRect.right, wRect.bottom);
	pCanvas->m_hbmDefault = SelectBitmap(pCanvas->m_hdcBuffer,pCanvas->m_hbmBuffer);

	// Indicate that this canvas object is ready for drawing
	pCanvas->m_bIsReady = true;

	// Start Windows message pump - process messages until WM_QUIT is received
	// 
	MSG msg;
	while (!pCanvas->m_bExit)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				pCanvas->m_bExit = true;
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
	};

	// Clean up drawing buffers/bitmaps
	SelectBitmap(pCanvas->m_hdcBuffer,pCanvas->m_hbmDefault);
	DeleteBitmap(pCanvas->m_hbmBuffer);
	DeleteDC(pCanvas->m_hdcBuffer);

	// Close device context
	EndPaint(pCanvas->m_hWnd,&(pCanvas->m_ps));

}

// -------------------------------------------------------------------------
// Constructor Method for the WinCanvas object
// -------------------------------------------------------------------------
WinCanvas::WinCanvas(short w, short h, std::wstring s)
	:	Canvas(),
	    /*Interface(),*/
		m_hWnd(NULL),
		m_hInstance(NULL),
		m_hdcScreen(NULL),
		m_hdcBuffer(NULL),
		m_hbmBuffer(NULL),
		m_hbmDefault(NULL),
		m_ps(),
		m_bExit(false),
		m_bIsReady(false),
		m_sWidth(w),
		m_sHeight(h)
{
	wcscpy_s(szClassName,s.c_str());
	s_pCanvas = this;

	_beginthread(OpenWinCanvas, 0, this);

	while (!m_bIsReady)
	{
		Sleep(50);
	};

}

// -------------------------------------------------------------------------
// Destructor Method for the WinCanvas object
// -------------------------------------------------------------------------
WinCanvas::~WinCanvas()
{
	s_pCanvas = nullptr;
	s_pInput = nullptr;
}

// -------------------------------------------------------------------------
// Methods to manage the WinCanvas object
// -------------------------------------------------------------------------
void WinCanvas::Display()
{
	RECT wRect;
	GetClientRect(this->m_hWnd, &wRect);
	BitBlt(this->m_hdcScreen, 0, 0, wRect.right, wRect.bottom, this->m_hdcBuffer, 0, 0, SRCCOPY);
}

void WinCanvas::Clear(int r, int g, int b)
{
	HBRUSH hBrush = CreateSolidBrush(RGB(r,g,b));
	SelectObject(this->m_hdcBuffer,hBrush);
	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(r,g,b));
	SelectObject(this->m_hdcBuffer, hPen);

	RECT wRect;
	GetClientRect(this->m_hWnd, &wRect);
	FillRect(this->m_hdcBuffer, &wRect, hBrush);

	DeleteObject(hPen);
	DeleteObject(hBrush);

}

void WinCanvas::Size(short & x, short & y)
{
	RECT wRect;
	GetClientRect(m_hWnd, &wRect);
	x = m_sWidth = (short)(wRect.right - wRect.left);
	y = m_sHeight = (short)(wRect.bottom - wRect.top);
}

void WinCanvas::Resize()
{
	RECT wRect;
	if (m_bIsReady)
	{
		// Clean up drawing buffers/bitmaps
		SelectBitmap(m_hdcBuffer,m_hbmDefault);
		DeleteBitmap(m_hbmBuffer);
		DeleteDC(m_hdcBuffer);
		EndPaint(m_hWnd,&m_ps);

		m_hdcScreen = BeginPaint(m_hWnd,&m_ps);
	
		m_hdcBuffer = CreateCompatibleDC(m_hdcScreen);
		GetClientRect(m_hWnd, &wRect);
		m_hbmBuffer = CreateCompatibleBitmap(m_hdcScreen, wRect.right, wRect.bottom);
		m_hbmDefault = SelectBitmap(m_hdcBuffer,m_hbmBuffer);
		m_sWidth = (short)(wRect.right - wRect.left);
		m_sHeight = (short)(wRect.bottom - wRect.top);
	}
}

// -------------------------------------------------------------------------
// Methods to draw shapes to display buffer
// -------------------------------------------------------------------------
void WinCanvas::DrawLine(Segment const & s, unsigned long line_rgb, int line_width)
{
	HPEN NewPen = CreatePen(PS_SOLID, line_width, line_rgb);

	SelectObject(m_hdcBuffer, NewPen);

	// create the set of points
	POINT pts[2];

	pts[0].x = (long)s.start().x;
	pts[0].y = (long)s.start().y;
	pts[1].x = (long)s.end().x;
	pts[1].y = (long)s.end().y;

	// Move pen to starting position
	MoveToEx(m_hdcBuffer, pts[0].x, pts[0].y, NULL);
	// Call Polygon() with the 4 points that form the rectangle
	PolylineTo(m_hdcBuffer, pts, 2);

	DeleteObject(NewPen);
}

void WinCanvas::DrawRect(Rect const & r, unsigned long line_rgb, int line_width)
{
	// Set border (outline) colour of the box
	HPEN NewPen = CreatePen(PS_SOLID, line_width, line_rgb);
	SelectObject(m_hdcBuffer, NewPen);

	// create the set of points
	POINT2f p[4] = {
		{ r.start().x, r.start().y },
		{ r.start().x, r.end().y },
		{ r.end().x, r.end().y },
		{ r.end().x, r.start().y }
	};
	POINT pts[4 + 1];
	pts[0].x = (long)p[0].x;
	pts[0].y = (long)p[0].y;
	pts[1].x = (long)p[1].x;
	pts[1].y = (long)p[1].y;
	pts[2].x = (long)p[2].x;
	pts[2].y = (long)p[2].y;
	pts[3].x = (long)p[3].x;
	pts[3].y = (long)p[3].y;
	pts[4].x = (long)p[0].x;
	pts[4].y = (long)p[0].y;

	// Move pen to starting position
	MoveToEx(m_hdcBuffer, pts[0].x, pts[0].y, NULL);
	// Call PolyLineTo() with the 4 points (5 pairs of points) that form the rectangle
	PolylineTo(m_hdcBuffer, pts, 4 + 1);

	DeleteObject(NewPen);
}

void WinCanvas::DrawSolidRect(Rect const & r, unsigned long fill_rgb, unsigned long line_rgb, int line_width)
{
	// Set fill colour of the Block
	HBRUSH NewBrush = CreateSolidBrush(fill_rgb);
	SelectObject(m_hdcBuffer, NewBrush);
	// Set border (outline) colour of the box
	HPEN NewPen = CreatePen(PS_SOLID, line_width, line_rgb);
	SelectObject(m_hdcBuffer, NewPen);

	// create the set of points
	POINT2f p[4] = {
		{ r.start().x, r.start().y },
		{ r.start().x, r.end().y },
		{ r.end().x, r.end().y },
		{ r.end().x, r.start().y }
	};
	POINT pts[4 + 1];
	pts[0].x = (long)p[0].x;
	pts[0].y = (long)p[0].y;
	pts[1].x = (long)p[1].x;
	pts[1].y = (long)p[1].y;
	pts[2].x = (long)p[2].x;
	pts[2].y = (long)p[2].y;
	pts[3].x = (long)p[3].x;
	pts[3].y = (long)p[3].y;
	pts[4].x = (long)p[0].x;
	pts[4].y = (long)p[0].y;

	// Call Polygon() with the 4 points that form the rectangle
	Polygon(m_hdcBuffer, pts, 4);

	DeleteObject(NewPen);
	DeleteObject(NewBrush);
}

void WinCanvas::DrawTriangle(Triangle const & t, unsigned long line_rgb, int line_width)
{
	// Set border (outline) colour of the box
	HPEN NewPen = CreatePen(PS_SOLID, line_width, line_rgb);
	SelectObject(m_hdcBuffer, NewPen);

	// create the set of points
	POINT pts[3 + 1];
	pts[0].x = (long)t.vertex(0).x;
	pts[0].y = (long)t.vertex(0).y;
	pts[1].x = (long)t.vertex(1).x;
	pts[1].y = (long)t.vertex(1).y;
	pts[2].x = (long)t.vertex(2).x;
	pts[2].y = (long)t.vertex(2).y;
	pts[3].x = (long)t.vertex(0).x;
	pts[3].y = (long)t.vertex(0).y;

	// Move pen to starting position
	MoveToEx(m_hdcBuffer, pts[0].x, pts[0].y, NULL);
	// Call Polygon() with the 3 points (4 pairs of points) that form the rectangle
	PolylineTo(m_hdcBuffer, pts, 3 + 1);

	DeleteObject(NewPen);
}

void WinCanvas::DrawSolidTriangle(Triangle const & t, unsigned long fill_rgb, unsigned long line_rgb, int line_width)
{
	// Set fill colour of the triangle
	HBRUSH NewBrush = CreateSolidBrush(fill_rgb);
	SelectObject(m_hdcBuffer, NewBrush);
	// Set border (outline) colour of the triangle
	HPEN NewPen = CreatePen(PS_SOLID, line_width, line_rgb);
	SelectObject(m_hdcBuffer, NewPen);

	// create the set of points
	POINT pts[3];
	pts[0].x = (long)t.vertex(0).x;
	pts[0].y = (long)t.vertex(0).y;
	pts[1].x = (long)t.vertex(1).x;
	pts[1].y = (long)t.vertex(1).y;
	pts[2].x = (long)t.vertex(2).x;
	pts[2].y = (long)t.vertex(2).y;
	pts[3].x = (long)t.vertex(0).x;
	pts[3].y = (long)t.vertex(0).y;

	// Call Polygon() with the 3 points that form the triangle
	Polygon(m_hdcBuffer, pts, 3);

	DeleteObject(NewPen);
	DeleteObject(NewBrush);
}

void WinCanvas::DrawCircle(Circle const & c, unsigned long line_rgb, int line_width)
{
	HPEN NewPen = CreatePen (PS_SOLID, line_width, line_rgb);

	int left	= (int)(c.origin().x - c.radius()),
		top		= (int)(c.origin().y - c.radius()),
		right	= (int)(c.origin().x + c.radius()),
		bottom	= (int)(c.origin().x + c.radius());

	SelectObject(m_hdcBuffer, (HBRUSH)GetStockObject(HOLLOW_BRUSH));
	SelectObject(m_hdcBuffer, NewPen); 
	Ellipse (m_hdcBuffer, left, top, right, bottom);

	DeleteObject(NewPen);
}

void WinCanvas::DrawSolidCircle(Circle const & c, unsigned long fill_rgb, unsigned long line_rgb, int line_width)
{
	HBRUSH NewBrush = CreateSolidBrush(fill_rgb);
	if (NewBrush == NULL)
	{
		DWORD err = GetLastError();
		std::wstring errorMsg = L"Failed to get brush";
		MessageBox(NULL, errorMsg.c_str(), L"WinCanvas::DrawSolidCircle()", MB_OK);

		PostMessage(m_hWnd, WM_DESTROY, NULL, NULL);	// Post a message to destroy (shutdown) the program
		return;
	}
	SelectObject(m_hdcBuffer, NewBrush); 

	int left = (int)(c.origin().x - c.radius()),
		top = (int)(c.origin().y - c.radius()),
		right = (int)(c.origin().x + c.radius()),
		bottom = (int)(c.origin().x + c.radius());

	HPEN NewPen = CreatePen (PS_SOLID, line_width, line_rgb);
	SelectObject(m_hdcBuffer, NewPen); 
	Ellipse(m_hdcBuffer, left, top, right, bottom);
	DeleteObject(NewPen);
	DeleteObject(NewBrush);
}

// -------------------------------------------------------------------------
// Methods to draw other items to display buffer (Bitmaps & Text)
// -------------------------------------------------------------------------
void WinCanvas::DrawTexture(Texture const & t, POINT2f pos, POINT2f s)
{

#ifdef WIN_TEXTURE_H
	WinTexture const * wt = static_cast<WinTexture const *>(&t);

	HDC hdcMemory = CreateCompatibleDC(m_hdcBuffer);

	HBITMAP hbmDefault = (HBITMAP)SelectObject(hdcMemory, wt->MaskHandle());
	BitBlt(m_hdcBuffer, (int)pos.x, (int)pos.y, t.Width(), t.Height(), hdcMemory, (int)s.x, (int)s.y, SRCAND);

	SelectObject(hdcMemory, wt->ImageHandle());
	BitBlt(m_hdcBuffer, (int)pos.x, (int)pos.y, t.Width(), t.Height(), hdcMemory, (int)s.x, (int)s.y, SRCPAINT);

	SelectObject(hdcMemory, hbmDefault);
	DeleteObject(hdcMemory);


#else
	std::wstring errorMsg = L"Unable to render Windows textures";
	MessageBox(NULL, errorMsg.c_str(), L"WinCanvas::DrawTexture()", MB_OK);
	PostMessage(m_hWnd, WM_DESTROY, NULL, NULL);	// Post a message to destroy (shutdown) the program
	return;
#endif
}

void WinCanvas::Write(int x, int y, std::string text, int r, int g, int b)
{
	HBRUSH NewBrush = CreateSolidBrush(RGB(r, g, b));
	SelectObject(m_hdcBuffer, NewBrush); 
	HPEN NewPen = CreatePen (PS_SOLID, 1, RGB(r, g, b));
	SelectObject(m_hdcBuffer, NewPen); 

	SetTextColor(m_hdcBuffer, RGB(r, g, b));
	SetBkMode(m_hdcBuffer, TRANSPARENT);

	RECT textRect = { 0, 0, 0, 0 };

	std::wstring ws;
	ws.assign (text.begin (), text.end ());

	// Get size of text box first... populate 'textRect'
	::DrawTextW(m_hdcBuffer, ws.c_str (), -1, &textRect, DT_CALCRECT);

	RECT rect;
	rect.left = x;
	rect.right = x + textRect.right;
	rect.top = y;
	rect.bottom = y + textRect.bottom;

	// Now 'actually' draw the text at the correct location and size!
	::DrawTextW(m_hdcBuffer, ws.c_str (), -1, &rect, DT_LEFT | DT_TOP );

	DeleteObject(NewPen);
	DeleteObject(NewBrush);
}

// -------------------------------------------------------------------------
// Methods to support interaction with the window (keyboard & mouse)
// -------------------------------------------------------------------------
/*
void WinCanvas::lastKey( int &k, bool &set )
{
	k = m_szKeyChar;
	set = m_bKeySet;
	m_bKeySet = false;
}

void WinCanvas::lastMouse( int &x, int &y, int &button, bool &set)
{
	x = m_iMouseX;
	y = m_iMouseY;
	button = m_iMouseButton;
	set = m_bMouseSet;
	m_bMouseSet = false;
}
*/
// -------------------------------------------------------------------------
// Methods to manage the window resource iteslf (interacts with operating system)
// -------------------------------------------------------------------------
ATOM WinCanvas::RegisterAppClass(HINSTANCE hInstance)
{
	m_hInstance = hInstance;

	WNDCLASSEX  wcex;
	ZeroMemory(&wcex,sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style	= CS_VREDRAW | CS_HREDRAW;
	wcex.lpfnWndProc = (WNDPROC)MsgProc;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(NULL,IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszClassName = szClassName;

	return RegisterClassEx(&wcex);
}

BOOL WinCanvas::InitAppInstance(HINSTANCE hInstance, int nCmdShow)
{
	m_hWnd = ::CreateWindowEx(
						NULL,
						szClassName,
						szClassName,
						WS_OVERLAPPEDWINDOW	/*WS_BORDER*/,			// Overlapped normal / Border no resize
						CW_USEDEFAULT,
						0,
						m_sWidth,
						m_sHeight,
						NULL,
						NULL,
						hInstance,
						NULL);

	// Window creation failure case
	if (!m_hWnd)
		return FALSE;

	// Display and force paint the window
	ShowWindow(m_hWnd,nCmdShow);
	UpdateWindow(m_hWnd);

	return TRUE;
}

