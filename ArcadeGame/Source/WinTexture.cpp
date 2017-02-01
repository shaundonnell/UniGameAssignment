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

#include <memory>
#include <assert.h>
#include "WinCanvas.h"
#include "WinTexture.h"

WinTexture::WinTexture(UINT uid, COLORREF dwTransparent)
	: Texture(),				// default construction of base class portion of object
	  m_hbmImage(NULL),		// NULL initialisation of handles
	  m_hbmMask(NULL)		// ...
{
	LoadFromResource(uid);
	CreateMask(dwTransparent);
}

WinTexture::WinTexture(std::string const & strFileName, COLORREF dwTransparent)
	: Texture(),				// as above
	  m_hbmImage(NULL),
	  m_hbmMask(NULL)
{
	LoadFromFile(strFileName);
	CreateMask(dwTransparent);
}

WinTexture::~WinTexture()
{
	// Ensure resources are released
	DeleteObject(m_hbmMask);
	DeleteObject(m_hbmImage);
}


void WinTexture::LoadFromResource(UINT uid)
{
	m_hbmImage = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(uid));
	if (m_hbmImage)
		GetObject(m_hbmImage, sizeof(BITMAP), &m_bmParam);
	else
		ZeroMemory(&m_bmParam, sizeof(BITMAP));
}

void WinTexture::LoadFromFile(std::string const & strFileName)
{
	std::wstring stemp = std::wstring(strFileName.begin(), strFileName.end());	// convert to wide string

	m_hbmImage = (HBITMAP)LoadImage(NULL,
									stemp.c_str(),
									IMAGE_BITMAP,
									0, 0,
									LR_DEFAULTSIZE | LR_LOADFROMFILE | LR_LOADTRANSPARENT
									);
	if (m_hbmImage == NULL)
	{
		DWORD err = GetLastError();
		std::wstring errorMsg = L"Failed to LoadImage - '" + stemp + L"', error code (" + std::to_wstring((long long)err) + L")";
		MessageBox(NULL, errorMsg.c_str(), L"WinTexture::LoadFromFile()", MB_OK);

		ZeroMemory(&m_bmParam, sizeof(BITMAP));

	}
	else
		GetObject(m_hbmImage, sizeof(BITMAP), &m_bmParam);

}

void WinTexture::CreateMask(DWORD dwTransparent)
{
	/*
	 *	Create a transparency mask for blending this Sprite with contents of back buffer during rendering
	 */
	if (m_hbmImage)
	{
		HDC hdcMem1 = CreateCompatibleDC(0);
		HDC hdcMem2 = CreateCompatibleDC(0);

		m_hbmMask = CreateBitmap(m_bmParam.bmWidth, m_bmParam.bmHeight, 1, 1, NULL);

		SelectObject(hdcMem1, m_hbmImage);
		SelectObject(hdcMem2, m_hbmMask);

		SetBkColor(hdcMem1, dwTransparent);

		BitBlt(hdcMem2, 0, 0, m_bmParam.bmWidth, m_bmParam.bmHeight,
			hdcMem1, 0, 0, SRCCOPY);
		BitBlt(hdcMem1, 0, 0, m_bmParam.bmWidth, m_bmParam.bmHeight,
			hdcMem2, 0, 0, SRCINVERT);

		DeleteDC(hdcMem1);
		DeleteDC(hdcMem2);
	}
}

//Rect const & WinTexture::GetBounds() const
//{
//}

int WinTexture::Width() const
{
	return m_bmParam.bmWidth;
}

int WinTexture::Height() const
{
	return m_bmParam.bmHeight;
}

HBITMAP WinTexture::ImageHandle() const
{
	return m_hbmImage;
}

HBITMAP WinTexture::MaskHandle() const
{
	return m_hbmMask;
}
