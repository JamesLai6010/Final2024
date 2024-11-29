#ifndef SOUNDCENTER_H_INCLUDED
#define SOUNDCENTER_H_INCLUDED

#include <map>
#include <utility>
#include <string>
#include <vector>
#include <allegro5/allegro_audio.h>

/**
 * @brief Stores and manages audio samples and instances.
 * @details All data related to basic allegro audio (ALLEGRO_SAMPLE and ALLEGRO_SAMPLE_INSTANCE) are all managed by SoundCenter.
 * If any sample instance has finished playing, the sample instance will be destroyed via update function.
 */
class SoundCenter
{
public:
	static SoundCenter *get_instance() {
		static SoundCenter SC;
		return &SC;
	}
	~SoundCenter();
	bool init();
	void update();
	bool erase_sample(const std::string &path);
	ALLEGRO_SAMPLE_INSTANCE *play(const std::string &path, ALLEGRO_PLAYMODE mode);
	bool is_playing(const ALLEGRO_SAMPLE_INSTANCE *const inst);
	void toggle_playing(ALLEGRO_SAMPLE_INSTANCE *inst);
private:
	SoundCenter();
	/**
	 * @brief This map container stores all audio data managed by SoundCenter.
	 * @details Key object of the map is audio path, and the respective value objects are ALLEGRO_SAMPLE* and a list of ALLEGRO_SAMPLE_INSTANCE*. The ALLEGRO_SAMPLE* represents the corresponding audio sample, and the list of ALLEGRO_SAMPLE_INSTANCE* represent all playing samples.
	 * Once the sample (ALLEGRO_SAMPLE*) is created, the sample will not be destroyed until the game process ends.
	 */
	std::map<std::string, std::pair<ALLEGRO_SAMPLE*, std::vector<ALLEGRO_SAMPLE_INSTANCE*>>> samples;
	/**
	 * @brief Sound update period.
	 */
	int update_period;
};

#endif
