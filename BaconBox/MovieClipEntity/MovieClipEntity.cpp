#include "MovieClipEntity.h"
#include <string>


#ifdef BB_FLASH_PLATFORM
#include "BaconBox/Components/Flash/MovieClipHolder.h"
#include "BaconBox/Components/Flash/FlashEntityContainer.h"
#include "BaconBox/Components/Flash/FlashTimeline.h"
#include "BaconBox/Components/Flash/FlashHasName.h"
#include "BaconBox/Components/Flash/FlashSizeComponent.h"
#include "BaconBox/Components/Flash/FlashTransform.h"
#include "BaconBox/Components/Flash/FlashAABBHitBox.h"
#include "BaconBox/Components/Flash/FlashColorTransform.h"
#include "BaconBox/Components/Flash/FlashMatrixComponent.h"

#else
#include "BaconBox/Components/Mesh.h"
#include "BaconBox/Components/StandardRenderer/MeshDriverRenderer.h"
#include "BaconBox/Components/DefaultMatrix.h"
#include "BaconBox/Components/Timeline.h"
#include "BaconBox/Components/DefaultEntityContainer.h"
#include "BaconBox/Components/DefaultTimeline.h"
#include "BaconBox/Components/DefaultAABBHitBox.h"
#include "BaconBox/Components/DefaultSizeComponent.h"

#endif

#include "BaconBox/Debug.h"

namespace BaconBox {

    BB_ID_IMPL(MovieClipEntity);

	MovieClipEntity::MovieClipEntity(): Entity(), HasNameProxy(this, "", false), TransformProxy(this, false), ColorTransformProxy(this), TimelineProxy(this), ClickableProxy(this), EntityContainerProxy(this), MatrixComponentProxy(this), SymbolComponentProxy(this), AABBHitBoxProxy(this), SizeComponentProxy(this), SpeedProxy(this), VisibilityProxy(this), UIElementProxy(this)
	#ifdef BB_LUA
        , LuaEntityProxy(this)
	#endif //BB_LUA
	{

#ifdef BB_FLASH_PLATFORM
	    setTimeline(new FlashTimeline());
	    setEntityContainer(new FlashEntityContainer());
	    setHasName(new FlashHasName());
        setSizeComponent(new FlashSizeComponent());
	    setTransform(new FlashTransform());
	    setAABBHitBox(new FlashAABBHitBox());
	   setColorTransform(new FlashColorTransform());
		addComponent(new MovieClipHolder());
	    setMatrixComponent(new FlashMatrixComponent());
	    
#else
	    	    setHasName(new HasName(""));

//		    Mesh *mesh  = new Mesh();
//		    mesh->getVertices().resize(4);
//		    addComponent(mesh);
//		    this->addComponent(new MeshDriverRenderer());
	    setTimeline(new DefaultTimeline());
	    setEntityContainer(new DefaultEntityContainer());
	    setMatrixComponent(new DefaultMatrix());
	    setTransform(new Transform());
        setAABBHitBox(new DefaultAABBHitBox());
        setColorTransform(new DefaultColorTransform());
		setSizeComponent(new DefaultSizeComponent());
		#endif
	}

	MovieClipEntity::MovieClipEntity(const MovieClipEntity& src) : Entity(), HasNameProxy(this, ""), TransformProxy(this), ColorTransformProxy(this), TimelineProxy(this), EntityContainerProxy(this), MatrixComponentProxy(this), ClickableProxy(this),SymbolComponentProxy(this), AABBHitBoxProxy(this), SizeComponentProxy(this), SpeedProxy(this), VisibilityProxy(this), UIElementProxy(this)
    #ifdef BB_LUA
        , LuaEntityProxy(this)
	#endif //BB_LUA
	{
#ifdef BB_FLASH_PLATFORM

#else

		//		    Mesh *mesh  = new Mesh();
		//		    mesh->getVertices().resize(4);
		//		    addComponent(mesh);
		//		    this->addComponent(new MeshDriverRenderer());
	    setTimeline(new DefaultTimeline());
	    setEntityContainer(new DefaultEntityContainer());
	    setMatrixComponent(new DefaultMatrix());

#endif
	}
	
	MovieClipEntity::~MovieClipEntity() {
		Debug::getInstance().destroyMovieClip(this->getKey());
	}

	MovieClipEntity &MovieClipEntity::operator=(const MovieClipEntity &src) {
		this->Entity::operator=(src);
		
		if (this != &src) {
		}
		
		return *this;
	}

#ifdef BB_FLASH_PLATFORM
	void MovieClipEntity::setMovieClip(AS3::local::var aMC) {
		MovieClipHolder *mcHolder = this->getComponent<MovieClipHolder>();
		mcHolder->setMovieClip(aMC);
	}
#endif



}
