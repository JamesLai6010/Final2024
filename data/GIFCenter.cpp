#include "GIFCenter.h"
#include <allegro5/bitmap_io.h>
#include "../Utils.h"

GIFCenter::~GIFCenter() {
	for(auto &[path, gif] : gifs) {
		algif_destroy_animation(gif);
	}
}

/**
 * @brief The getter function searches if a bitmap is loaded and return the bitmap. If not loaded, it will try to load the GIF and return.
 * @details If the respective GIF does not exist, it will immediately call GAME_ASSERT and terminate the game. This exception can be handled in various ways. e.g. load a "missing texture" when an GIF fails to load.
 * @param path the GIF path.
 * @return The curresponding loaded ALGIF_ANIMATION* instance.
 */
ALGIF_ANIMATION*
GIFCenter::get(const std::string &path) {
	std::map<std::string, ALGIF_ANIMATION*>::iterator it = gifs.find(path);
	if(it == gifs.end()) {
		ALGIF_ANIMATION *gif = algif_load_animation(path.c_str());
		GAME_ASSERT(gif != nullptr, "cannot find GIF: %s.", path.c_str());
		gifs[path] = gif;
		return gif;
	} else {
		return it->second;
	}
}

/**
 * @brief Remove a bitmap.
 * @param path the GIF path.
 * @return True if the bitmap of the path is removed. False if the bitmap does not exist.
 */
bool
GIFCenter::erase(const std::string &path) {
	std::map<std::string, ALGIF_ANIMATION*>::iterator it = gifs.find(path);
	if (it == gifs.end()) {
		return false;
	}
	ALGIF_ANIMATION *bitmap = it->second;
	algif_destroy_animation(bitmap);
	gifs.erase(it);
	return true;
}
