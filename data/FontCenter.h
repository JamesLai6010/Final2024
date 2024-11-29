#ifndef FONTCENTER_H_INCLUDED
#define FONTCENTER_H_INCLUDED

#include <array>
#include <map>
#include <allegro5/allegro_font.h>

// fixed settings
namespace FontSize
{
	static constexpr int SMALL = 12, MEDIUM = 24, LARGE = 36;
	static constexpr std::array<int, 3> list({
		FontSize::SMALL, FontSize::MEDIUM, FontSize::LARGE
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
private:
	FontCenter() {}
};

#endif
