#ifndef FONTCENTER_H_INCLUDED
#define FONTCENTER_H_INCLUDED

#include <array>
#include <map>
#include <allegro5/allegro_font.h>

// fixed settings
namespace FontSize
{
	static constexpr int SMALL = 12, MEDIUM = 24, LARGE = 36, XL = 72, XXL = 150;
	static constexpr std::array<int, 5> list({
		FontSize::SMALL, FontSize::MEDIUM, FontSize::LARGE, FontSize::XL, FontSize::XXL
	});
};

/**
 * @brief Stores and manages fonts.
 * @details While FontCenter is initializing, it will use the fixed settings to create ALLEGRO_FONT* instances and store them. The created font instances will be stored in map and use font size as the key.
 */
class FontCenter
{
public:
	static FontCenter *get_instance() {
		static FontCenter FC;
		return &FC;
	}
	~FontCenter();
	void init();
public:
	std::map<int, ALLEGRO_FONT*> caviar_dreams;
	std::map<int, ALLEGRO_FONT*> courier_new;
	std::map<int, ALLEGRO_FONT*> SuperMarioBros;
	std::map<int, ALLEGRO_FONT*> pirulen;
	std::map<int, ALLEGRO_FONT*> street_fight;
private:
	FontCenter() {}
};

#endif
