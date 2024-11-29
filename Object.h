#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include "shapes/Shape.h"
#include <memory>

class Object
{
public:
	Object() {}
	virtual ~Object() {}
public:
	// pure function for drawing the object
	virtual void draw() = 0;
public:
	std::unique_ptr<Shape> shape;
};

#endif
