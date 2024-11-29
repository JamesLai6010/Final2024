#ifndef GIFCENTER_H_INCLUDED
#define GIFCENTER_H_INCLUDED

#include <map>
#include <string>
#include "../algif5/algif.h"

/**
 * @brief Stores and manages bitmaps.
 * @details GIFCenter loads bitmap data dynamically and persistently. That is, an GIF will only be loaded when demanded by getter function, and will be stored inside memory once loaded.
 * This center does not free any bitmap as long as the game is running. You can manually free bitmaps that will not be used again if you want to reduce the memory usage.
 */
class GIFCenter
{
public:
	static GIFCenter *get_instance() {
		static GIFCenter GIFC;
		return &GIFC;
	}
	~GIFCenter();
	ALGIF_ANIMATION *get(const std::string &path);
	ALGIF_ANIMATION *get(const char *path) { return get(std::string{path}); }
	bool erase(const std::string &path);
private:
	GIFCenter() {}
	/**
	 * @brief All loaded bitmaps are stored in this map container.
	 * @details The key object of this map is the GIF path. Make sure the path must be the same if the same GIF will be queried multiple times, otherwise the GIF will be duplicately loaded.
	 */
	std::map<std::string, ALGIF_ANIMATION*> gifs;
};

#endif
