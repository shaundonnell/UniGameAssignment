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

#ifndef CANVAS_H
#define CANVAS_H

#include <string>
#include <memory>

#include "Geometry.h"
#include "Texture.h"

#define LRGB(r,g,b)          ((unsigned long)(((unsigned char)(r)|((unsigned short)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))



class Canvas
{
	public:
		Canvas(){}
		virtual ~Canvas(){}

		virtual bool IsActive() = 0;
		virtual void Terminate() = 0;

		virtual void Display() = 0;
		virtual void Clear(int r, int g, int b) = 0;

		virtual void Resize() = 0;
		virtual void Size(short & x, short & y) = 0;

		virtual void DrawLine(Segment const & s, unsigned long line_rgb = LRGB(0, 0, 255), int width = 1) = 0;
		virtual void DrawRect(Rect const & r, unsigned long line_rgb = LRGB(0, 0, 255), int line_width = 1) = 0;
		virtual void DrawCircle(Circle const & c, unsigned long line_rgb = LRGB(0, 0, 255), int line_width = 1) = 0;
		virtual void DrawTriangle(Triangle const & t, unsigned long line_rgb = LRGB(0, 0, 255), int width = 1) = 0;

		virtual void DrawSolidRect(Rect const & r, unsigned long fill_rgb = LRGB(0, 0, 255), unsigned long line_rgb = LRGB(0, 0, 255), int line_width = 1) = 0;
		virtual void DrawSolidCircle(Circle const & c, unsigned long fill_rgb = LRGB(0, 0, 255), unsigned long line_rgb = LRGB(0, 0, 255), int line_width = 1) = 0;
		virtual void DrawSolidTriangle(Triangle const & t, unsigned long fill_rgb = LRGB(0, 0, 255), unsigned long line_rgb = LRGB(0, 0, 255), int width = 1) = 0;

		virtual void DrawTexture(Texture const & t, POINT2f pos, POINT2f s = { 0, 0 }) = 0;

		virtual void Write(int x, int y, std::string text, int r = 0, int g = 0, int b = 0) = 0;
};


#endif