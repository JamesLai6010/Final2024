#ifndef IMAGECENTER_H_INCLUDED
#define IMAGECENTER_H_INCLUDED

#include <map>
#include <string>
#include <allegro5/bitmap.h>

/**
 * @brief Stores and manages bitmaps.
 * @details ImageCenter loads bitmap data dynamically and persistently. That is, an image will only be loaded when demanded by getter function, and will be stored inside memory once loaded.
 * This center does not free any bitmap as long as the game is running. You can manually free bitmaps that will not be used again if you want to reduce the memory usage.
 */
class ImageCenter
{
public:
	static ImageCenter *get_instance() {
		static ImageCenter IC;
		return &IC;
	}
	~ImageCenter();
	ALLEGRO_BITMAP *get(const std::string &path);
	ALLEGRO_BITMAP *get(const char *path) { return get(std::string{path}); }
	bool erase(const std::string &path);
private:
	ImageCenter() {}
	/**
	 * @brief All loaded bitmaps are stored in this map container.
	 * @details The key object of this map is the image path. Make sure the path must be the same if the same image will be queried multiple times, otherwise the image will be duplicately loaded.
	 */
	std::map<std::string, ALLEGRO_BITMAP*> bitmaps;
};

#endif
