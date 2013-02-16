#include "TextComponent.h"

#include "BaconBox/Core/Entity.h"
#include "BaconBox/Console.h"
#include "Helper/StringHelper.h"
#include "Helper/UTFConvert.h"
#include "EntityFactory.h"
#include "Mesh.h"
#include "BaconBox/Display/Text/Font.h"
#include "BaconBox/Core/Engine.h"
#include "BaconBox/Display/Text/TextAlignment.h"
#include "BaconBox/Helper/Vector2ChangedData.h"

namespace BaconBox {
    
	int TextComponent::MESSAGE_TEXT_CHANGED = IDManager::generatetID();
	int TextComponent::MESSAGE_ALIGNMENT_CHANGED = IDManager::generatetID();
	int TextComponent::MESSAGE_SIZE_CHANGED = IDManager::generatetID();
	BB_ID_IMPL(TextComponent);
	
	TextComponent::TextComponent() : Component(), alignment(TextAlignment::LEFT), boundingBox(Vector2(), Vector2(200,200)) {
	}

	TextComponent::~TextComponent() {
	}
	


	void TextComponent::receiveMessage(int senderID, int destID, int message, void *data) {
	    if(senderID == Transform::ID){
		if(message == Transform::MESSAGE_POSITION_CHANGED){
		    boundingBox.setPosition(reinterpret_cast<Vector2ChangedData*>(data)->newValue);
		}
	    }
	}
	void TextComponent::setAlignment(TextAlignment alignment){
	    this->alignment = alignment;
	    sendMessage(Entity::BROADCAST, TextComponent::MESSAGE_ALIGNMENT_CHANGED, reinterpret_cast<void*>(&alignment));
	}	

	void TextComponent::setSize(Vector2 size){
	    boundingBox.setSize(size);
	    sendMessage(Entity::BROADCAST, TextComponent::MESSAGE_SIZE_CHANGED, reinterpret_cast<void*>(&boundingBox));
	}

	
	void TextComponent::setText(const std::string & text){
	    std::string temp = text;
	    sendMessage(Entity::BROADCAST, TextComponent::MESSAGE_TEXT_CHANGED, reinterpret_cast<void*>(&temp));
	}
	
	Vector2 TextComponent::getSize(){
	    return boundingBox.getSize();
	}

	Font * TextComponent::getFont(){
	    return font;
	}
	
	TextComponentProxy::TextComponentProxy(Entity* entity, Font * font, bool mustAddComponent): BB_PROXY_CONSTRUCTOR(new TextComponent())  {
	    reinterpret_cast<TextComponent*>(component)->font = font;
	}
	void TextComponentProxy::setAlignment(TextAlignment alignment){
	    reinterpret_cast<TextComponent*>(component)->setAlignment(alignment);
	}
	
	Font * TextComponentProxy::getFont(){
	    return reinterpret_cast<TextComponent*>(component)->getFont();
	}
	Vector2 TextComponentProxy::getSize(){
	    return reinterpret_cast<TextComponent*>(component)->getSize();
	}

	void TextComponentProxy::setSize(Vector2 size){
	    reinterpret_cast<TextComponent*>(component)->setSize(size);
	}
	
	void TextComponentProxy::setText(const std::string & text){
	    reinterpret_cast<TextComponent*>(component)->setText(text);
	}
	
}