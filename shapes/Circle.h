#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

#include "Shape.h"

/**
 * @see Shape.cpp
 */
class Circle : public Shape
{
public:
	bool overlap(const Shape &s) const;
	double center_x() const { return x; }
	double center_y() const { return y; }
	void update_center_x(const double &x) { this->x = x; }
	void update_center_y(const double &y) { this->y = y; }
	const ShapeType getType() const { return ShapeType::CIRCLE; }
public:
	Circle() {}
	Circle(double x, double y, double r) : x{x}, y{y}, r{r} {}
	Circle(int x, int y, int r) :
		x{static_cast<double>(x)},
		y{static_cast<double>(y)},
		r{static_cast<double>(r)} {}
	double x, y, r;
};

#endif
