/**
 * @file
 * @ingroup Audio
 */
#ifndef BB_SOUNDFX_H
#define BB_SOUNDFX_H

#include "BaconBox/Audio/Sound.h"

namespace BaconBox {
	/**
	 * Abstract class for sound effects. Used by engine specific classes for
	 * sound effects.
	 * @ingroup Audio
	 */
	class SoundFX : public Sound {
		friend class AudioEngine;
	public:
		/**
		 * Destructor.
		 */
		virtual ~SoundFX();
	protected:
		/**
		 * Default constructor. Can only be called by the sound engine.
		 */
		SoundFX();
	};
}

#endif // BB_SOUNDFX_H
