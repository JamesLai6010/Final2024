#include "SoundCenter.h"
#include "../Utils.h"

using namespace std;

// fixed settings
namespace SoundSetting {
	constexpr int RESERVED_SAMPLES = 16;
	constexpr int UPDATE_PERIOD = 60;
}

SoundCenter::SoundCenter () : update_period{SoundSetting::UPDATE_PERIOD} {}

SoundCenter::~SoundCenter() {
	for(auto &[path, sample_pair] : samples) {
		auto &[sample, insts] = sample_pair;
		al_destroy_sample(sample);
		for(ALLEGRO_SAMPLE_INSTANCE *inst : insts)
			al_destroy_sample_instance(inst);
	}
}

/**
 * @brief Reserve samples to have default mixer work.
 */
bool
SoundCenter::init() {
	bool res = true;
	res &= al_restore_default_mixer();
	res &= al_reserve_samples(SoundSetting::RESERVED_SAMPLES);
	res &= (al_get_default_mixer() != nullptr);
	return res;
}

/**
 * @brief The update function searches all sample instances and destroy instances that have finished playing.
 * @details An instance that has finished playing needs to satisfy all the following conditions:
 * @details * The instance is paused (or stopped).
 * @details * The audio track position is 0 (at initial position).
 * @details * The instance is not set to loop mode.
 */
void
SoundCenter::update() {
	if (update_period == 0) {
		update_period = SoundSetting::UPDATE_PERIOD;
		for(auto &[path, audio_pair] : samples) {
			auto &[sample, insts] = audio_pair;
			for(auto it = insts.begin(); it != insts.end();) {
				if(al_get_sample_instance_playing(*it)) ++it;
				else if(al_get_sample_instance_position(*it) != 0) ++it;
				else if(al_get_sample_instance_playmode(*it) == ALLEGRO_PLAYMODE_LOOP) ++it;
				else {
					al_destroy_sample_instance(*it);
					it = insts.erase(it);
				}
			}
		}
	} else {
		--update_period;
	}
}

/**
 * @brief Remove a sample.
 * @details This function will also destroy all sample instances played by the sample to be destroyed.
 * @param path audio path.
 * @return True if the sample of the path is destroyed. False if the sample does not exist.
 */
bool
SoundCenter::erase_sample(const std::string &path) {
	auto it = samples.find(path);
	if(it == samples.end()) {
		return false;
	}
	auto &[sample, insts] = it->second;
	for(auto inst : insts) {
		al_destroy_sample_instance(inst);
	}
	insts.clear();
	al_destroy_sample(sample);
	return true;
}

/**
 * @brief Play an audio.
 * @param path the audio file path.
 * @param mode the play mode defined by allegro5.
 * @return The curresponding played ALLEGRO_SAMPLE_INSTANCE* instance.
 * @details For the list of supported play modes, refer to [manual](https://liballeg.org/a5docs/trunk/audio.html#allegro_playmode).
 */
ALLEGRO_SAMPLE_INSTANCE*
SoundCenter::play(const string &path, ALLEGRO_PLAYMODE mode) {
	auto it = samples.find(path);
	if(it == samples.end()) {
		ALLEGRO_SAMPLE *sample = al_load_sample(path.c_str());
		GAME_ASSERT(sample != nullptr, "cannot find sample: %s.", path.c_str());
		it = samples.insert({path, {sample, {}}}).first;
	}
	auto &[sample, insts] = it->second;
	ALLEGRO_SAMPLE_INSTANCE *instance = al_create_sample_instance(sample);
	insts.emplace_back(instance);

	al_set_sample_instance_playmode(instance, mode);
	al_attach_sample_instance_to_mixer(instance, al_get_default_mixer());
	al_play_sample_instance(instance);
	return instance;
}

/**
 * @brief Check is an instance is currently playing.
 */
bool
SoundCenter::is_playing(const ALLEGRO_SAMPLE_INSTANCE *const inst) {
	return al_get_sample_instance_playing(inst);
}

/**
 * @brief Pause or play an audio, depends on its current playing state.
 */
void
SoundCenter::toggle_playing(ALLEGRO_SAMPLE_INSTANCE *inst) {
	bool is_playing = al_get_sample_instance_playing(inst);
	if(is_playing) {
		unsigned int pos = al_get_sample_instance_position(inst);
		al_stop_sample_instance(inst);
		// As the sample stops, allegro will automatically reset the play position to 0. We need to set it back to be able to resume.
		al_set_sample_instance_position(inst, pos);
	} else al_play_sample_instance(inst);
}
