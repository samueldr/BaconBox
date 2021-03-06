
#include "BaconBox/Core/IDManager.h"
#include "BaconBox/Core/Component.h"
#include "BaconBox/Components/EntityManager.h"
#include "BaconBox/Core/State.h"
namespace BaconBox {
	BB_ID_IMPL(EntityManager);

	EntityManager::EntityManager() : Component(), entities(), toAdd(), toRemove() {
	}

	EntityManager::~EntityManager() {
	    for (std::vector<Entity *>::iterator i = this->entities.begin(); i != this->entities.end(); ++i) {
			delete (*i);
		}
	}

	void EntityManager::add(Entity *newEntity) {
	    if(newEntity){
		this->toAdd.push_back(newEntity);
	    }
	}

	void EntityManager::remove(Entity *newEntity) {
		if (newEntity) {
			this->toRemove.insert(newEntity);
		}
	}

	void EntityManager::update(){
	    	// We add the entities.
		if (!this->toAdd.empty()) {
			this->entities.reserve(this->entities.size() + this->toAdd.size());


			for (std::list<Entity *>::iterator i = this->toAdd.begin(); i != this->toAdd.end(); ++i) {
				this->entities.push_back(*i);
			}

			this->toAdd.clear();
		}

		// We remove the entities.
		if (!this->toRemove.empty()) {
			std::vector<Entity *>::size_type i = 0;

			while (i < this->entities.size()) {
				if (this->toRemove.find(this->entities[i]) != this->toRemove.end()) {
					this->entities.erase(this->entities.begin() + i);
				} else {
					++i;
				}
			}

			this->toRemove.clear();
		}

		// We update the entities.
		for (std::vector<Entity *>::iterator i = this->entities.begin(); i != this->entities.end(); ++i) {
			(*i)->update();
		}

	}


	void EntityManager::receiveMessage(int senderID, int destID, int message, void *data) {
		if(senderID != State::ID)return;

		if(message == State::MESSAGE_ADDED_ENTITY){
		    add(static_cast<Entity*>(data));
		}
		else if(message == State::MESSAGE_REMOVED_ENTITY){
		    remove(static_cast<Entity*>(data));
		}
		else if(message == State::MESSAGE_LOST_FOCUS || message == State::MESSAGE_GET_FOCUS){
		    for (std::vector<Entity *>::iterator i = this->entities.begin(); i != this->entities.end(); ++i) {
                (*i)->sendMessage(senderID, destID, message, data);
            }
		}
	}

}
