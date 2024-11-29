#ifndef SHAPE_H_INCLUDED
#define SHAPE_H_INCLUDED

class Point;
class Rectangle;
class Circle;

enum class ShapeType {
	POINT, RECTANGLE, CIRCLE
};

/**
 * @brief Base shape class.
 * @details A "Shape" can be useful in many ways - you can treat Shape as a bounding box, attack range, colliding detection, and many other things. Basically if you want to make objects interact to each other, the Shape is indispensable.
 * @see Shape.cpp
 */
class Shape
{
public:
	virtual bool overlap(const Shape &s) const = 0;
	virtual double center_x() const = 0;
	virtual double center_y() const = 0;
	virtual void update_center_x(const double &x) = 0;
	virtual void update_center_y(const double &y) = 0;
	virtual const ShapeType getType() const = 0;
	virtual ~Shape() {}
};

#endif
