#ifndef TOWER_H_INCLUDED
#define TOWER_H_INCLUDED

#include "../Object.h"
#include "../shapes/Rectangle.h"
#include <allegro5/bitmap.h>
#include <string>
#include <array>

class Bullet;

// fixed settings
enum class TowerType {
	ARCANE, ARCHER, CANON, POISON, STORM, TOWERTYPE_MAX
};
namespace TowerSetting {
	const std::array<std::string, static_cast<int>(TowerType::TOWERTYPE_MAX)> tower_full_img_path = {
		"./assets/image/tower/Arcane.png",
		"./assets/image/tower/Archer.png",
		"./assets/image/tower/Canon.png",
		"./assets/image/tower/Poison.png",
		"./assets/image/tower/Storm.png"
	};
	const std::array<std::string, static_cast<int>(TowerType::TOWERTYPE_MAX)> tower_menu_img_path = {
		"./assets/image/tower/Arcane_Menu.png",
		"./assets/image/tower/Archer_Menu.png",
		"./assets/image/tower/Canon_Menu.png",
		"./assets/image/tower/Poison_Menu.png",
		"./assets/image/tower/Storm_Menu.png"
	};
	const std::array<std::string, static_cast<int>(TowerType::TOWERTYPE_MAX)> tower_bullet_img_path = {
		"./assets/image/tower/Arcane_Beam.png",
		"./assets/image/tower/Archer_Beam.png",
		"./assets/image/tower/Canon_Beam.png",
		"./assets/image/tower/Poison_Beam.png",
		"./assets/image/tower/Storm_Beam.png"
	};
	const std::array<int, static_cast<int>(TowerType::TOWERTYPE_MAX)> tower_price = {50, 100, 150, 200, 250};
};

class Tower : public Object
{
public:
	/**
	 * @brief Get the ALLEGRO_BITMAP* instance of the full image of a specific TowerType.
	 */
	static ALLEGRO_BITMAP *get_bitmap(TowerType type);
	/**
	 * @brief Create a Tower* instance by the type.
	 * @param type the type of a tower.
	 * @param p center point of the tower.
	 */
	static Tower *create_tower(TowerType type, const Point &p);
public:
	Tower(const Point &p, double attack_range, int attack_freq, TowerType type);
	virtual ~Tower() {}
	void update();
	virtual bool attack(Object *target);
	void draw();
	Rectangle get_region() const;
	virtual Bullet *create_bullet(Object *target) = 0;
	virtual const double attack_range() const = 0;
	TowerType type;
private:
	/**
	 * @var attack_freq
	 * @brief Tower attack frequency. This variable will be set by its child classes.
	 **
	 * @var counter
	 * @brief Tower attack cooldown.
	 */
	int attack_freq;
	int counter;
	ALLEGRO_BITMAP *bitmap;
};

#endif
