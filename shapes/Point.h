#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

#include "Shape.h"
#include <cmath>

/**
 * @see Shape.cpp
 */
class Point : public Shape
{
public:
	static double dist2(const Point &p1, const Point &p2) {
		return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
	}
	static double dist(const Point &p1, const Point &p2) {
		return std::sqrt(dist2(p1, p2));
	}
public:
	bool overlap(const Shape &s) const;
	double center_x() const { return x; }
	double center_y() const { return y; }
	void update_center_x(const double &x) { this->x = x; }
	void update_center_y(const double &y) { this->y = y; }
	const ShapeType getType() const { return ShapeType::POINT; }
public:
	Point() {}
	Point(double x, double y) : x{x}, y{y} {}
	Point(int x, int y) :
		x{static_cast<double>(x)},
		y{static_cast<double>(y)} {}
	double x, y;
};

#endif
