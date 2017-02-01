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

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <stdint.h>
#include "math_t.h"

using namespace math::affine;

class Segment
{
	protected:

		POINT2f		p, q;	// end points of segment
		VECTOR2f	n;		// normal vector of the wall

	public:

		Segment(POINT2f const & _p, POINT2f const & _q, VECTOR2f const & _n)
			: p(_p),
			q(_q),
			n(normalise(_n))
		{}

		VECTOR2f			direction()	const	{ return normalise(q - p); }
		VECTOR2f const &	normal() const		{ return n; }
		POINT2f const &		start()		const { return p; }
		POINT2f const &		end()		const { return q; }
		float				length()	const { return (q - p).length(); }

};

class Polytype
{
	protected:
		uint32_t		type_;

		/*
		 * Construction only by derived types
		 */
		explicit Polytype(uint32_t _t)
			: type_(_t)
		{}

	public:
		virtual ~Polytype(){}
		uint32_t	type() const	{ return type_; }
};


typedef enum _polygon_types : uint32_t
{
	CIRCLE,
	RECTANGLE,
	TRIANGLE
} POLYGON_TYPE;


class Circle : public Polytype
{
	protected:

		POINT2f		p;		// origin
		float		r;		// radius

	public:

		Circle(POINT2f const & _p, double _r = 1.0)
			: Polytype(CIRCLE),
			  p(_p),
			  r((float)_r)
		{}

		float const &		radius()	const { return r; }
		POINT2f const &		origin()	const { return p; }

		void position(POINT2f const & dp)	{ p = dp; }
		void scale(float const & dr)	{ r = dr; }
};


class Rect : public Polytype
{
	protected:
		POINT2f		ul,
					br;

	public:

		Rect(POINT2f const & _ul, POINT2f const & _br)
			: Polytype(RECTANGLE),
			  ul(_ul),
			  br(_br)
		{}

		POINT2f centre() const	{ return (ul + br) / 2.0f; }

		POINT2f const & start() const	{ return ul; }
		POINT2f const & end() const		{ return br; }

};

class Triangle : public Polytype
{
protected:
	POINT2f  v[3];

public:
	Triangle(POINT2f v0, POINT2f v1, POINT2f v2)
		: Polytype(TRIANGLE)
	{
		v[0] = v0;
		v[1] = v1;
		v[2] = v2;
	}

		POINT2f const & vertex(size_t i) const { return v[i % 3]; }
		POINT2f & vertex(size_t i) { return v[i % 3]; }

};

/*
 * Consider adding functionality to test for collisions between Geometry objects
 */

/*
bool CircleCircleIntersection(Circle const &, Circle const &);
bool RectRectIntersection(Rect const &, Rect const &);
bool CircleRectIntersection(Circle const &, Rect const &);
*/
#endif