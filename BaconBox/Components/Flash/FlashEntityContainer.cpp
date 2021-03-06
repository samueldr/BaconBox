#include "FlashEntityContainer.h"
#include "BaconBox/Core/Entity.h"
#include "BaconBox/Components/ComponentConnection.h"
#include "BaconBox/Helper/Flash/FlashHelper.h"
#include "BaconBox/Console.h"
#include <AS3/AS3.h>
#include <AS3/AS3++.h>

namespace BaconBox {
	
	FlashEntityContainer::FlashEntityContainer() : EntityContainer() {
		initializeConnections();
	}
	
	FlashEntityContainer::FlashEntityContainer(const FlashEntityContainer &src) : EntityContainer(src) {
	}
	
	FlashEntityContainer::~FlashEntityContainer() {
		

	}

	/*
	void FlashEntityContainer::update(){
		int numChildren = getNbChildren();
		for(int i = 0; i < numChildren; i++){
			MovieClipEntity * entity = getChildAt(i);
			if(entity)entity->update();
		}
	}

	void FlashEntityContainer::render(){
		int numChildren = getNbChildren();
		for(int i = 0; i <numChildren; i++){
			MovieClipEntity * entity = getChildAt(i);
			if(entity)entity->render();
		}
	}
	*/


	


	void FlashEntityContainer::propagateMessage(int senderID, int destID, int message, void *data){
  		int numChildren = getNbChildren();
		for(int i = 0; i < numChildren; i++){
			MovieClipEntity * entity = getChildAt(i);
			if(entity){
				entity->propagateMessage(senderID, destID, message, data);
			}
		}
    }

	
	void FlashEntityContainer::initializeConnections(){
	    this->addConnection(new ComponentConnection<MovieClipHolder>(&this->movieClipHolder));
	}
	
	void FlashEntityContainer::receiveMessage(int senderID, int destID, int message, void *data) {
				this->EntityContainer::receiveMessage(senderID, destID, message, data);
	}
	
	void FlashEntityContainer::addChild(MovieClipEntity *newChild){
		AS3::local::var childMC = newChild->getComponent<MovieClipHolder>()->getMovieClip();
		AS3::local::var args[1] = {childMC};
		movieClipHolder->callMethod("addChild", 1,args);
	}
	
	void FlashEntityContainer::addChildAt(MovieClipEntity *newChild, int index){
		AS3::local::var childMC = newChild->getComponent<MovieClipHolder>()->getMovieClip();
		AS3::local::var args[2] = {childMC, AS3::local::internal::new_int(index)};
		movieClipHolder->callMethod("addChildAt", 1, args);
	}
	
	bool FlashEntityContainer::contains(MovieClipEntity *child) const {
		AS3::local::var childMC = child->getComponent<MovieClipHolder>()->getMovieClip();
		AS3::local::var args[1] = {childMC};
		AS3::local::var result = movieClipHolder->callMethod("contains", 1, args);
		return AS3::local::internal::bool_valueOf(result);
	}
	
	MovieClipEntity *FlashEntityContainer::getChildAt(int index){
		return internalGetChildAt(index);
	}

	const MovieClipEntity *FlashEntityContainer::getChildAt(int index) const{
		return internalGetChildAt(index);
	}
	
	MovieClipEntity *FlashEntityContainer::getChildByName(const std::string &name){
		return internalgetChildByName(name);
	}
	
	const MovieClipEntity *FlashEntityContainer::getChildByName(const std::string &name) const{
		return internalgetChildByName(name);
	}

	MovieClipEntity *FlashEntityContainer::internalgetChildByName(const std::string &name) const{
		AS3::local::var args[1] = {AS3::local::internal::new_String(name.c_str())};
		AS3::local::var result = movieClipHolder->callMethod("getChildByName", 1, args);
		return FlashHelper::getMCEntityFromMC(result);

	}
	MovieClipEntity *FlashEntityContainer::internalGetChildAt(int index) const{
		AS3::local::var args[1] = {AS3::local::internal::new_int(index)};
		AS3::local::var result = movieClipHolder->callMethod("getChildAt", 1, args);
		return FlashHelper::getMCEntityFromMC(result);
	}

	
	
	int FlashEntityContainer::getChildIndex(MovieClipEntity *child) const{
		AS3::local::var args[1] = {child->getComponent<MovieClipHolder>()->getMovieClip()};
		AS3::local::var index = movieClipHolder->callMethod("getChildIndex", 1, args);
		return AS3::local::internal::int_valueOf(index);
	}
	
	std::vector<MovieClipEntity *> FlashEntityContainer::getObjectsUnderPoint(const Vector2 &point){
		throw "not implemented yet";
	}
	
	std::vector<const MovieClipEntity *> FlashEntityContainer::getObjectsUnderPoint(const Vector2 &point) const{
		throw "not implemented yet";
	}
	
	MovieClipEntity *FlashEntityContainer::removeChild(MovieClipEntity *child){
		AS3::local::var args[1] = {child->getComponent<MovieClipHolder>()->getMovieClip()};
		movieClipHolder->callMethod("removeChild", 1, args);
	}
	
	MovieClipEntity *FlashEntityContainer::removeChildAt(int index){
		AS3::local::var args[1] = { AS3::local::internal::new_int(index)};
		movieClipHolder->callMethod("removeChild", 1, args);
	}
	
	void FlashEntityContainer::removeChildren(int beginIndex, int endIndex){
		throw "not implemented yet";
	}
	
	void FlashEntityContainer::setChildIndex(MovieClipEntity *child, int index){
		AS3_DeclareVar(mc,*);
		AS3::local::var tempMc = movieClipHolder->getMovieClip();
		AS3_CopyVarxxToVar(mc,tempMc);
		AS3_DeclareVar(ch,*);
		AS3::local::var tempCh = child->getComponent<MovieClipHolder>()->getMovieClip();
		AS3_CopyVarxxToVar(ch,tempCh);
		inline_as3(
			"mc.setChildIndex(ch,%0);\n"
			: : "r"(index)
		);
	}
	
	void FlashEntityContainer::swapChildren(MovieClipEntity *child1, MovieClipEntity *child2){
		AS3::local::var args[2] = {child1->getComponent<MovieClipHolder>()->getMovieClip(), child2->getComponent<MovieClipHolder>()->getMovieClip()};
		movieClipHolder->callMethod("swapChildren", 1, args);
	}
	
	void FlashEntityContainer::swapChildrenAt(int index1, int index2){
		AS3::local::var args[2] = { AS3::local::internal::new_int(index1),  AS3::local::internal::new_int(index2)};
		movieClipHolder->callMethod("swapChildrenAt", 1, args);
	}
	
	int FlashEntityContainer::getNbChildren() const{
		AS3::local::var result = movieClipHolder->getProperty("numChildren");
		return AS3::local::internal::int_valueOf(result);
	}
	
	MovieClipEntity * FlashEntityContainer::getParent() const{
		AS3::local::var result = movieClipHolder->getProperty("parent");
		return FlashHelper::getMCEntityFromMC(result);
	}

	void FlashEntityContainer::removeAllChildren(){
		AS3_DeclareVar(mc, *);
		AS3::local::var tempMC = movieClipHolder->getMovieClip();
		AS3_CopyVarxxToVar(mc, tempMC);
		inline_as3(
			"while (mc.numChildren > 0) {\n"
				"mc.removeChildAt(0);\n"
			"}\n"
		);

	}

	void FlashEntityContainer::deleteAllChildren(){
		removeAllChildren();
	}

}
