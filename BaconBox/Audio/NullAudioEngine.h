/**
 * @file
 * @ingroup Audio
 */
#ifndef BB_NULL_AUDIO_ENGINE_H
#define BB_NULL_AUDIO_ENGINE_H

#include <list>

#include "BaconBox/Audio/SoundEngine.h"
#include "BaconBox/Audio/MusicEngine.h"

namespace BaconBox {
	class NullAudio;
	/**
	 * Audio engine implementation that does nothing...
	 * @ingroup Audio
	 */
	class NullAudioEngine : public SoundEngine, public MusicEngine {
		friend class AudioEngine;
		friend class MusicEngine;
		BB_SINGLETON_DECL(NullAudioEngine);
	public:
		/**
		 * Constructs a sound effect. Gets the sound's data associated with the
		 * key.
		 * @param key Identifier for the sound data to use. The sound data needs
		 * to be loaded beforehand.
		 * @param survive Used to specify if the sound effect's destroy is
		 * managed by the user or by the engine when the sound is done playing.
		 * This lets the user simply call this function and play the sound
		 * directly if survive is at false or keep it and re-use it if surive is
		 * at true.
		 * @return Sound effect generated by this function, the user will either
		 * have to destroy it manually if survive is set at true, or will be
		 * automatically destroyed once the sound's state is at STOPPED. The
		 * sound effect returned is at the INITIAL state.
		 */
		SoundFX* getSoundFX(const std::string& key, bool survive = true);

		/**
		 * Constructs a background music. Gets the music's data associated with
		 * the key.
		 * @param key Identifier for the music data to use. The music data needs
		 * to be loaded beforehand.
		 * @param survive Used to specify if the music destroy is managed by the
		 * user or by the engine when the music is done playing. This lets the
		 * user simply call this function and play the music directly if survive
		 * is at false or keep it and re-use it if surive is at true.
		 * @return Background music generated by this function, the user will
		 * either have to destroy it manually if survive is set at true, or will
		 * be automatically destroyed once the music's state is at STOPPED. The
		 * background music returned is at the INITIAL state.
		 */
		BackgroundMusic* getBackgroundMusic(const std::string& key,
		                                    bool survive = true);
	private:
		/// List of NullAudios managed by the NullAudioEngine.
		std::list<NullAudio*> audios;

		/**
		 * Default constructor.
		 */
		NullAudioEngine();

		/**
		 * Destructor.
		 */
		~NullAudioEngine();

		/**
		 * Updates the necessary informations for the audio engine.
		 */
		void update();

		/**
		 * Loads sound data from a file.
		 * @param filePath Path to the sound file to load.
		 * @return Pointer to the sound data loaded.
		 */
		SoundInfo* loadSound(const std::string& filePath);

		/**
		 * Loads sound data from engine specific parameters.
		 * @param params Parameters to use to load the sound data. Each engine
		 * has implementation different parameters.
		 * @return Pointer to the sound data loaded.
		 */
		SoundInfo* loadSound(const SoundParameters& params);

		/**
		 * Creates sound data from a buffer, it is in the same loadable formats
		 * than loadSound would use, but from memory data.
		 */
		SoundInfo* loadSoundFromBuffer(unsigned char* buf, unsigned int size);

		/**
		 * Unloads sound data.
		 * @param sound Sound data that needs to be unloaded. Delete must not be
		 * called on it, the resource manager that calls this function takes
		 * care of that.
		 * @return True if the unloading was successful, false if not.
		 */
		bool unloadSound(SoundInfo* sound);

		/**
		 * Loads music data from a file.
		 * @param filePath Path to the music file to load.
		 * @return Pointer to the music data loaded.
		 */
		MusicInfo* loadMusic(const std::string& filePath);

		/**
		 * Loads music data from engine specific parameters.
		 * @param params Parameters to use to load the music data. Each engine
		 * has implementation different parameters.
		 * @return Pointer to the music data loaded.
		 */
		MusicInfo* loadMusic(const MusicParameters& params);

		/**
		 * Unloads music data.
		 * @param music Music data that needs to be unloaded. Delete must not be
		 * called on it, the resource manager that calls this function takes
		 * care of that.
		 * @return True if the unloading was successful, false if not.
		 */
		bool unloadMusic(MusicInfo* music);

	};
}

#endif
