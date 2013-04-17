#include "EntityFactory.h"

#include "BaconBox/PlatformFlagger.h"
#include "BaconBox/Components/Transform.h"
#include "BaconBox/Components/Mesh.h"
#include "BaconBox/Components/ColorFilter.h"
#include "BaconBox/Components/MeshDriverRenderer.h"
#include "BaconBox/Components/DefaultEntityContainer.h"
#include "BaconBox/Components/DefaultTimeline.h"

#include "BaconBox/Components/Texture.h"
#include "BaconBox/ResourceManager.h"
#include "BaconBox/Display/TextureInformation.h"
#include "BaconBox/Console.h"
#include "Components/DefaultEntityContainer.h"
#include "Components/DefaultMatrix.h"
#include "Core/Engine.h"

#ifdef BB_FLASH_PLATEFORM
#include <AS3/AS3.h>
#include <AS3/AS3++.h>
#include "BaconBox/Helper/Flash/FlashHelper.h"

#endif

#include <BaconBox/Console.h>



namespace BaconBox {
	
	EntityFactory::EntityFactory():movieClipPool(0){


	}

	
	void EntityFactory::initMovieClipPool(int size){
		getInstance().movieClipPool.setNbAvailableObjects(size);
	}

	MovieClipEntity *EntityFactory::getMovieClipEntity(const std::string &key, bool autoPlay) {
		return getInstance().internalGetMovieClipEntity(key, autoPlay);
	}

	
	MovieClipEntity *EntityFactory::internalGetMovieClipEntity(const std::string &key, bool autoPlay) {
#ifdef BB_DEBUG
    try{
#endif
#ifdef BB_FLASH_PLATEFORM
		AS3::local::var mc =  FlashHelper::construct(key);
		AS3::local::var entity = FlashHelper::getProperty(mc, "entity");
		AS3::local::var entityPointerAS3 = FlashHelper::getProperty(entity, "swigCPtr");
		MovieClipEntity *entityPointer = (MovieClipEntity *)int_valueOf(entityPointerAS3);
		return entityPointer;

#else
		MovieClipEntity * entity;
		Symbol * symbol = ResourceManager::getSymbol(key);
 		if(symbol){
			entity = getMovieClipEntityFromSymbol(symbol, autoPlay);
		}
		else{
		    entity = getMovieClipEntityFromSubTexture(ResourceManager::getSubTexture(key));
		}
		if(!entity){
		    Console__error("EntityFactory::getMovieClipEntity can't return entity with key: " << key);
		}
		return entity;
//		SubTextureInfo* subTex = ResourceManager::getSubTexture(key);
//		return getMovieClipEntityFromSubTexture(subTex);


#endif
		
#ifdef BB_DEBUG
    }
    catch(...){
    Console__error("Error in EntityFactory::getMovieClipEntity with key: " << key);
    throw;
    }
#endif		
	}
	
#if  defined(BB_FLASH_PLATEFORM)
TextEntity * EntityFactory::getTextEntity(const std::string &key){
		AS3::local::var mc =  FlashHelper::construct(key);
		AS3::local::var entity = FlashHelper::getProperty(mc, "entity");
		AS3::local::var entityPointerAS3 = FlashHelper::getProperty(entity, "swigCPtr");
		TextEntity *entityPointer = (TextEntity *)int_valueOf(entityPointerAS3);
		return entityPointer;
}
#else

	MovieClipEntity *EntityFactory::getMovieClipEntityFromSymbol(Symbol* symbol, bool autoPlay){
	MovieClipEntity * entity = NULL;
	    if(symbol->isTexture){
		if(!symbol->subTex->textureInfo){
		    std::string textureKey = symbol->textureKey;
			TextureInformation * textureInfo = NULL;
			if(ResourceManager::isLoadedTexture(textureKey)){
				textureInfo = ResourceManager::getTexture(textureKey);
			}
			else{
				textureInfo = ResourceManager::loadTexture(symbol->textureKey);
			}
			symbol->subTex = ResourceManager::getSubTexture(symbol->key);
			symbol->subTex->textureInfo = textureInfo;
		}
		entity = getMovieClipEntityFromSubTexture(symbol->subTex, symbol->registrationPoint);
	    }
	    else{
		entity = movieClipPool.create();
		DefaultEntityContainer * container = reinterpret_cast<DefaultEntityContainer*>(entity->getComponent(DefaultEntityContainer::ID));
		DefaultTimeline * timeline = reinterpret_cast<DefaultTimeline*>(entity->getComponent(DefaultTimeline::ID));
		timeline->setNbFrames(symbol->frameCount);
		std::map<int, std::map <int, Symbol::Part*> > orderedPart;
		for(std::list<Symbol::Part>::iterator i = symbol->parts.begin();
			i != symbol->parts.end(); i++){
			for(std::map<int,int>::iterator j = i->indexByFrame.begin(); j != i->indexByFrame.end(); j++){
			    orderedPart[j->first][j->second] = &(*i);
			}
		}
			std::map<std::string, MovieClipEntity*> childByName;
			
		
		for(std::map<int, std::map <int, Symbol::Part*> >::iterator i = orderedPart.begin();
			i != orderedPart.end(); i++){
			entity->gotoAndStop(i->first);
		    for(std::map <int, Symbol::Part*>::iterator j = i->second.begin(); j != i->second.end(); j++){
			MovieClipEntity * childEntity;
				std::multimap<std::string, MovieClipEntity*>::iterator currentMovieClip = childByName.find(j->second->name);
				if(currentMovieClip == childByName.end()){
					childEntity = getMovieClipEntityFromSymbol(j->second->symbol, autoPlay);
					childByName[j->second->name] = childEntity;
				}
				else{
					childEntity = currentMovieClip->second;
				}
			childEntity->setName(j->second->name);
			reinterpret_cast<DefaultMatrix*>(childEntity->getComponent(DefaultMatrix::ID))->matrixByParentFrame = j->second->matrices;
			container->addChildToCurrentFrame(childEntity);
		    }
		}
		if(autoPlay){
		    entity->gotoAndPlay(0);
		}
		else{
		    entity->gotoAndStop(0);
		}
		    
	    }
	entity->setSymbol(symbol);
	
	return entity;
	}
	
	EntityFactory &EntityFactory::getInstance(){
		static EntityFactory instance;
		return instance;
	}
	
	MovieClipEntity *EntityFactory::getMovieClipEntityFromSubTexture(SubTextureInfo* subTex, const Vector2 & origin){
	    MovieClipEntity *result = NULL;

		if (subTex) {
			result = movieClipPool.create();

			Mesh *mesh = new Mesh();

			mesh->getPreTransformVertices().resize(4);
			
			mesh->getPreTransformVertices()[1].x = subTex->size.x;
			mesh->getPreTransformVertices()[2].y = subTex->size.y;
			mesh->getPreTransformVertices()[3] = subTex->size;
			mesh->getPreTransformVertices().move(origin.x, origin.y);
			mesh->syncMesh();

			result->addComponent(mesh);
			
			Texture *textureComponent = new Texture();

			textureComponent->setTexture(subTex->textureInfo);

			textureComponent->getTextureCoordinates().resize(4);
			textureComponent->getTextureCoordinates()[0].x= subTex->position.x/subTex->textureInfo->poweredWidth;
			textureComponent->getTextureCoordinates()[0].y= subTex->position.y/subTex->textureInfo->poweredHeight;
			textureComponent->getTextureCoordinates()[1].x = (subTex->position.x + subTex->size.x)/subTex->textureInfo->poweredWidth;
			textureComponent->getTextureCoordinates()[1].y = (subTex->position.y)/subTex->textureInfo->poweredHeight;
			textureComponent->getTextureCoordinates()[2].x = (subTex->position.x)/subTex->textureInfo->poweredWidth;
			textureComponent->getTextureCoordinates()[2].y = (subTex->position.y + subTex->size.y)/subTex->textureInfo->poweredHeight;
			textureComponent->getTextureCoordinates()[3].x = (subTex->position.x + subTex->size.x)/subTex->textureInfo->poweredWidth;
			textureComponent->getTextureCoordinates()[3].y = (subTex->position.y + subTex->size.y)/subTex->textureInfo->poweredHeight;
			result->addComponent(textureComponent);

			result->addComponent(new MeshDriverRenderer(RenderMode::SHAPE | RenderMode::COLOR | RenderMode::TEXTURE));
		}
		
		return result;
	}
#endif

}
