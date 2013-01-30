/**
 * @file
 */
#ifndef BB_MOVIECLIPENTITY_H
#define BB_MOVIECLIPENTITY_H

#include "BaconBox/Core/Entity.h"
#include "BaconBox/PlatformFlagger.h"

#ifdef BB_FLASH_PLATEFORM
#include <AS3/AS3.h>
#include <AS3/AS3++.h>
#endif

namespace BaconBox {
	/**
	 * 
	 */
	class MovieClipEntity : public Entity { 
    public:
	MovieClipEntity();
	
#ifdef BB_FLASH_PLATEFORM
		void setMovieClip(AS3::local::var aMC);
#endif
		
    private:

		
	};
}

#endif // BB_MOVIECLIPENTITY_H
