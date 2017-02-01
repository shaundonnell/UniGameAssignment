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

#ifndef WIN_TEXTURE_H
#define WIN_TEXTURE_H

#ifndef _WINDOWS_
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
#endif

#include <fstream>
#include <string>
#include "BaseClasses.h"
#include "Texture.h"
#include "Canvas.h"

class WinTexture : public Texture
{

protected:
	HBITMAP	m_hbmImage;
	HBITMAP m_hbmMask;
	BITMAP	m_bmParam;


	virtual void LoadFromResource(UINT rid);
	virtual void LoadFromFile(std::string const & strFileName);
	void CreateMask(DWORD dwTransparent);

public:
	// Construct from Windows Resource
	WinTexture(UINT uid, COLORREF dwTransparent);

	// Constructor from file load
	WinTexture(std::string const & strFilename, COLORREF dwTransparent);


	virtual ~WinTexture();

	// Inherited interface
//	virtual Rect const & GetBounds() const;
	virtual int Width() const;
	virtual int Height() const;

	HBITMAP ImageHandle() const;
	HBITMAP MaskHandle() const;

};


#endif