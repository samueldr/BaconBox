/**
 * @file
 * @ingroup Audio
 */
#ifndef BB_SOUND_ENGINE_H
#define BB_SOUND_ENGINE_H

#include <string>

#include "BaconBox/Audio/SoundParameters.h"
#include "BaconBox/Audio/AudioEngine.h"

namespace BaconBox {
	class SoundFX;
	struct SoundInfo;
	/**
	 * Abstract class for sound engines. Audio engine implementations for sound
	 * effects must inherit from this class and implement the required abstract
	 * functions.
	 * @ingroup Audio
	 */
	class SoundEngine : public virtual AudioEngine {
		friend class ResourceManager;
		friend class AudioEngine;
		friend class Engine;
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
		virtual SoundFX* getSoundFX(const std::string& key, bool survive = true) = 0;

		/**
		 * Gets the global sound effects volume.
		 * @return Current global sound effects volume.
		 * @see BaconBox::SoundEngine::soundVolume;
		 */
		int getSoundVolume() const;

		/**
		 * Sets the global sound effects volume.
		 * @param newSoundVolume New global sound effects volume.
		 * @see BaconBox::SoundEngine::soundVolume;
		 */
		virtual void setSoundVolume(int newSoundVolume);

		/**
		 * Checks if the sound engine is muted.
		 * @return True if the sound engine is muted, false if not.
		 */
		bool isMuted() const;

		/**
		 * Mutes or unmutes the sound engine.
		 * @param newMuted True to mute, false to unmute.
		 */
		void setMuted(bool newMuted);

		/**
		 * Mutes the sound engine.
		 */
		void mute();

		/**
		 * Unmutes the sound engine.
		 */
		void unmute();
	protected:
		/**
		 * Default constructor. Engine initialization is specifically done in
		 * the init() method inherited from AudioEngine.
		 */
		SoundEngine();

		/**
		 * Destructor. Unloads the sound engine.
		 */
		virtual ~SoundEngine();

		/**
		 * Loads sound data from a file.
		 * @param filePath Path to the sound file to load.
		 * @return Pointer to the sound data loaded.
		 */
		virtual SoundInfo* loadSound(const std::string& filePath) = 0;

		/**
		 * Loads sound data from engine specific parameters.
		 * @param params Parameters to use to load the sound data. Each engine
		 * has implementation different parameters.
		 * @return Pointer to the sound data loaded.
		 */
		virtual SoundInfo* loadSound(const SoundParameters& params) = 0;

		/**
		 * Unloads sound data.
		 * @param sound Sound data that needs to be unloaded. Delete must not be
		 * called on it, the resource manager that calls this function takes
		 * care of that.
		 * @return True if the unloading was successful, false if not.
		 */
		virtual bool unloadSound(SoundInfo* sound) = 0;
	private:
		/// Value between 0 and 100 for the volume level. 100 is the maximum.
		int soundVolume;

		/**
		 * Value used to determine if the music engine is muted or not. When it
		 * is not muted, this attribute is set to -1. When it is muted, it
		 * contains the volume.
		 */
		int muteValue;
	};
}

#endif
