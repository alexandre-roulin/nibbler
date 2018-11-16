#include "PreviousComponent.hpp"
#include <cassert>
#include <iostream>

std::string const PreviousComponent::NAME_COMPONENT = "PreviousComponent";
std::string const PreviousComponent::SKIP = "skip";
std::string const PreviousComponent::ID_FOLLOWED = "idPrevious";
std::string const PreviousComponent::POSITION_COMPONENT = "positionComponent";

PreviousComponent::PreviousComponent(KINU::Entity::Id id, bool skip)
		: idPrevious(id),
		  skip(skip),
		  positionComponent(PositionComponent()){
}

Json::Value PreviousComponent::serializeComponent() {
	Json::Value component;
	component[SKIP] = skip;
	component[ID_FOLLOWED] = idPrevious;
	component[POSITION_COMPONENT] = positionComponent.serializeComponent();
	return component;
}

PreviousComponent::PreviousComponent(Json::Value json) {
	assert(json[SKIP].isBool());
	assert(json[ID_FOLLOWED].isInt());
	assert(json[POSITION_COMPONENT].isObject());

	skip = json.get(SKIP, false).asBool();
	idPrevious = json.get(ID_FOLLOWED, -1).asInt();
	positionComponent = PositionComponent(json.get(POSITION_COMPONENT, {0, 0}));

}

std::ostream &operator<<(std::ostream &os, const PreviousComponent &component) {
	os << "idPrevious: " << component.idPrevious << " positionComponent: "
	   << component.positionComponent << " skip: " << component.skip;
	return os;
}
