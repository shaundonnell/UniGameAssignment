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

#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{

protected:
	// We cannot create or copy base class texture objects
	// We will only ever have pointers or references to base
	// class texture objects used in our program (and these
	// will refer to derived class textures
	Texture() = default;
	Texture(Texture const &) = default;
	Texture & operator=(Texture const &) = default;

	virtual void LoadFromFile(std::string const & strFileName) = 0;
	virtual void LoadFromResource(unsigned int rid) = 0;

public:
	virtual ~Texture(){}

	//	virtual Rect const & GetBounds() const = 0;
	virtual int Width() const = 0;
	virtual int Height() const = 0;
};

using TEXTURE_PTR = std::shared_ptr<Texture>;

#endif