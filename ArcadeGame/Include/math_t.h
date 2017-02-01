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

#ifndef MATH_T_H
#define MATH_T_H

//#include <stdlib.h>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <limits>

/*
 * Open namespace: math
 */
namespace math {
	/*
	 * Open namespace: math::affine
	 */
	namespace affine {


	template <typename T> struct _tuple_2;

	/*
	 * _tuple_2<T>		2 element tuple class
	 *
	 * @param: 
	 *		T			element type
	 */
	template <typename T>
	struct _tuple_2
	{
		typedef T	value_type;

		// Attributes
		T x, y;

		static const size_t size = 2;

		/*
		 * Construction
		 */

		// Default
		_tuple_2()
			: x((T)0), y((T)0)
		{}

		// Initialisation
		_tuple_2( T const _x, T const _y )
			: x(_x), y(_y)
		{}

		// Copy
		_tuple_2( _tuple_2 const& t )
		{
			if (this != &t)
			{
				x = t.x;
				y = t.y;
			}
		}

		/*
		 * Array access
		 */
		// Read-only-access
		T const& operator[]( size_t const i) const
		{
			return *(&x + i);
		}

		// Write-access
		T& operator[]( size_t const i )
		{
			return *(&x + i);
		}

		/*
		 * Assignment
		 */
		_tuple_2 const& operator=( _tuple_2 const& t )
		{
			if (this != &t)
			{
				x = t.x;
				y = t.y;
			}
			return *this;
		}

		/*
		 * Logical operators
		 */

		bool const operator==( _tuple_2 const& t) const
		{
			return ( x == t.x ) && ( y == t.y ) ;
		}

		bool const operator!=( _tuple_2 const& t) const
		{
			return ! ( *this == t ) ;
		}

		/*
		 * Mathematical/ computational operators
		 */

		// Negation
		_tuple_2 const operator-() const
		{
			return _tuple_2( -x, -y );
		}

		// Additive assignment
		_tuple_2 const& operator+=( _tuple_2 const& t)
		{
			x += t.x;
			y += t.y;
			return *this;
		}

		// Subtractive assignment
		_tuple_2 const& operator-=( _tuple_2 const& t)
		{
			x -= t.x;
			y -= t.y;
			return *this;
		}

		// Scalar multiplication
		_tuple_2 const& operator*=( T const a )
		{
			x *= a;
			y *= a;
			return *this;
		}

		// Scalar division
		_tuple_2 const& operator/=( T const a )
		{
#ifdef _DEBUG
				assert( a != 0 && "Divide by zero error in _tuple_2<T>::operator/=");
#endif
			if ( a != 0 ) 
			{
				x /= a;
				y /= a;
			}
			else
			{
				x = y = std::numeric_limits<T>::quiet_NaN();
			}
			return *this;
		}

		// Addition
		inline _tuple_2 const operator+( _tuple_2 const& t ) const
		{
			return _tuple_2( x + t.x, y + t.y );
		}

		// Subtraction
		inline _tuple_2 const operator-( _tuple_2 const& t ) const
		{
			return _tuple_2( x - t.x, y - t.y );
		}

		// Postfix scalar multiplication
		inline _tuple_2 const operator*( T const a ) const
		{
			return _tuple_2( x*a, y*a );
		}

		// Prefix scalar multiplication
		template <typename U>
		friend inline _tuple_2<U> const operator*( T const a, _tuple_2<U> const& t)
		{
			return t * a;
		}

		// Postfix scalar division
		inline _tuple_2 const operator/( T const a ) const
		{
#ifdef _DEBUG
				assert( a != 0 && "Divide by zero error in _tuple_2<T>::operator/=");
#endif
			if ( a != 0 ) 
				return _tuple_2( x/a, y/a );
			else
				return _tuple_2(std::numeric_limits<T>::quiet_NaN(),std::numeric_limits<T>::quiet_NaN());
		}


	};



	template <typename T>
	void ZERO_TUPLE(_tuple_2<T> & t)
	{
		t.x = (T)0;
		t.y = (T)0;
	}



	/*
	* _vector_2<T>				2 element vector class
	*   : public _tuple_2<T>	extends tuple_2<T>
	*
	* @param:
	*		T			element type
	*/
	template <typename T>
	struct _vector_2 : public _tuple_2<T>
	{
		typedef typename _tuple_2<T>::value_type	value_type;

		/*
		* Construction
		*/

		// Default
		_vector_2()
			: _tuple_2()
		{}

		// Initialisation
		_vector_2(T const _x, T const _y)
			: _tuple_2(_x, _y)
		{}

		_vector_2(_tuple_2<T> const& t)
			: _tuple_2(t)
		{}

		// Copy
		_vector_2(_vector_2 const& v)
		{
			if (this != &v)
			{
				x = v.x;
				y = v.y;
			}
		}

		// Magnitude
		inline T const length() const
		{
			return sqrt(length_sqr());
		}

		inline T const length_sqr() const
		{
			return T(x*x + y*y);
		}

		template <typename U>
		friend inline _vector_2<U>& normalise(_vector_2<U>& v)
		{
			v /= v.length();
			return v;
		}

		template <typename U>
		friend inline _vector_2<U> normalise(_vector_2<U> const & v)
		{
			return v / v.length();
		}

		template <typename U>
		friend inline U const inner_product(_vector_2<U> const& u, _vector_2<U> const& v)
		{
			return  U(u.x*v.x + u.y*v.y);
		}
	};

	/*
	* _point_2<T>		2 element point class implementing homogeneous coordinates
	*
	* @param:
	*		T			element type
	*/
	template <typename T>
	class _point_2 : public _tuple_2<T>
	{
	public:
		typedef typename _tuple_2<T>::value_type	value_type;

		_point_2()
			: _tuple_2()
		{}

		_point_2(_tuple_2 const & t)
			: _tuple_2(t)
		{}

		_point_2(_point_2 const & t)
			: _tuple_2(t.x, t.y)
		{}

		_point_2(T const & _x, T const & _y)
			: _tuple_2(_x, _y)
		{}

		/*
		// Cast to int type
		operator _point_2<int>()
		{
			return _point_2<int>((int)x, (int)y);
		}
		*/

		// Additive assignment
		_point_2 const& operator+=(_vector_2<T> const& t)
		{
			x += t.x;
			y += t.y;
			return *this;
		}

		// Subtractive assignment
		_point_2 const& operator-=(_vector_2<T> const& t)
		{
			x -= t.x;
			y -= t.y;
			return *this;
		}

		// Addition: point + vector
		inline _point_2 operator+(_vector_2<T> const& t) const
		{
			return _point_2(x + t.x, y + t.y);
		}

		// Subtraction: point - vector
		inline _point_2 operator-(_vector_2<T> const& t) const
		{
			return _point_2(x - t.x, y - t.y);
		}

		// Subtraction: point - point
		inline _vector_2<T> operator-(_point_2 const& t) const
		{
			return _vector_2<T>(x - t.x, y - t.y);
		}

		// Prefix multiplication by a scalar
		template <typename U>
		friend inline _point_2 operator*(U const a, _point_2 const& p)
		{
			return p * a;
		}


		/*
		* The following operators are overloaded and hidden as private since they are not meaningful for points
		* If C++0x11 is available, we can use the '=delete' modifier on the definition.
		*/
	private:
		// Additive assignment
		_tuple_2 const& operator+=(_tuple_2 const& t)
		{
			x += t.x;
			y += t.y;
			return *this;
		}

		// Subtractive assignment
		_tuple_2 const& operator-=(_tuple_2 const& t)
		{
			x -= t.x;
			y -= t.y;
			return *this;
		}


	};

	// Output
	template <typename U>
	std::ostream& operator<<(std::ostream& os, _tuple_2<U> const& t)
	{
		return os << "(" << t.x << "," << t.y << ")";
	}

	// Addition of point plus vector
	template <typename U>
	inline _point_2<U> operator+(_vector_2<U> const & v, _point_2<U> const & p)
	{
		return (_point_2<U>)((_tuple_2<U>)v + (_tuple_2<U>)p);
	}

	// Addition of vector plus point
	template <typename U>
	inline _point_2<U> operator+(_point_2<U> const & p, _vector_2<U> const & v)
	{
		return (v + p);
	}

	// Subtraction of point from vector
	template <typename U>
	inline _point_2<U> operator-(_vector_2<U> const & v, _point_2<U> const & p)
	{
		return (v + -p);
	}

	// Subtraction of vector from point
	template <typename U>
	inline _point_2<U> operator-(_point_2<U> const & p, _vector_2<U> const & v)
	{
		return (-v + p);
	}


	typedef _tuple_2<float>	TUPLE2f;
	typedef _point_2<float> POINT2f;
	typedef _vector_2<float> VECTOR2f;

	typedef _tuple_2<int>	TUPLE2;
	typedef _point_2<int>	POINT2;
	typedef _vector_2<int>	VECTOR2;

	enum AXIS2 : size_t {X = 0, Y = 1};

	} // close namespace 'math::affine'
} // close namesace 'math'

#endif