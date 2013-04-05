#ifndef BB_ENTITY_FACTORY_H
#define BB_ENTITY_FACTORY_H

#include "BaconBox/Core/Entity.h"
#include "BaconBox/MovieClipEntity/MovieClipEntity.h"
#include "BaconBox/Display/TexturePointer.h"
#include "BaconBox/Display/Text/TextEntity.h"

namespace BaconBox {

	class EntityFactory {
	public:
	    
		static MovieClipEntity *getMovieClipEntity(const std::string &key, bool autoPlay = false);
#if defined(BB_FLASH_PLATEFORM)
		static TextEntity *getTextEntity(const std::string &key);
#else
		static MovieClipEntity *getMovieClipEntityFromSymbol(Symbol* symbol, bool autoPlay = false);
		static MovieClipEntity *getMovieClipEntityFromSubTexture(SubTextureInfo* subTex, const Vector2 & origin = Vector2());
#endif
	private:


	};

}

#endif
