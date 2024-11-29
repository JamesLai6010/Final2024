#include "ImageCenter.h"
#include <allegro5/bitmap_io.h>
#include "../Utils.h"

ImageCenter::~ImageCenter() {
	for(auto &[path, bitmap] : bitmaps) {
		al_destroy_bitmap(bitmap);
	}
}

/**
 * @brief The getter function searches if a bitmap is loaded and return the bitmap. If not loaded, it will try to load the image and return.
 * @details If the respective image does not exist, it will immediately call GAME_ASSERT and terminate the game. This exception can be handled in various ways. e.g. load a "missing texture" when an image fails to load.
 * @param path the image path.
 * @return The curresponding loaded ALLEGRO_BITMAP* instance.
 */
ALLEGRO_BITMAP*
ImageCenter::get(const std::string &path) {
	std::map<std::string, ALLEGRO_BITMAP*>::iterator it = bitmaps.find(path);
	if(it == bitmaps.end()) {
		ALLEGRO_BITMAP *bitmap = al_load_bitmap(path.c_str());
		GAME_ASSERT(bitmap != nullptr, "cannot find image: %s.", path.c_str());
		bitmaps[path] = bitmap;
		return bitmap;
	} else {
		return it->second;
	}
}

/**
 * @brief Remove a bitmap.
 * @param path the image path.
 * @return True if the bitmap of the path is removed. False if the bitmap does not exist.
 */
bool
ImageCenter::erase(const std::string &path) {
	std::map<std::string, ALLEGRO_BITMAP*>::iterator it = bitmaps.find(path);
	if (it == bitmaps.end()) {
		return false;
	}
	ALLEGRO_BITMAP *bitmap = it->second;
	al_destroy_bitmap(bitmap);
	bitmaps.erase(it);
	return true;
}
