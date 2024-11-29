#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include "../Object.h"
#include <allegro5/bitmap.h>
#include <string>

/**
 * @brief The bullet shot from Tower.
 * @see Tower
 */
class Bullet : public Object
{
public:
	Bullet(const Point &p, const Point &target, const std::string &path, double v, int dmg, double fly_dist);
	void update();
	void draw();
	const double &get_fly_dist() const { return fly_dist; }
	const int &get_dmg() const { return dmg; }
private:
	/**
	 * @brief Velocity in x direction.
	 */
	double vx;
	/**
	 * @brief Velocity in y direction.
	 */
	double vy;
	/**
	 * @brief Flying distance limit of the bullet. If the flying distance exceeds the limit, the bullet should be erased.
	 */
	double fly_dist;
	/**
	 * @brief Base damage of the bullet when hit anything.
	 */
	int dmg;
	/**
	 * @brief ALLEGRO_BITMAP of the bullet.
	 */
	ALLEGRO_BITMAP *bitmap;
};

#endif
