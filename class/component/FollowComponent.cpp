#include "FollowComponent.hpp"


std::string const FollowComponent::NAME_COMPONENT = "followComponent";
std::string const FollowComponent::SKIP = "skip";
std::string const FollowComponent::ID_FOLLOWED = "idFollowed";
std::string const FollowComponent::POSITION_COMPONENT = "positionComponent";

FollowComponent::FollowComponent(KNU::Entity::ID id, bool skip)
		: _idFollowed(id),
		  skip(skip) {

}

FollowComponent &
FollowComponent::operator=(FollowComponent const &followComponent) {
	if (this != &followComponent) {
		_idFollowed = followComponent._idFollowed;
		skip= followComponent.skip;
	}
	return *this;
}

Json::Value FollowComponent::serializeComponent() {
	Json::Value component;
	component[NAME_COMPONENT][SKIP] = skip;
	component[NAME_COMPONENT][ID_FOLLOWED]= _idFollowed;
	component[NAME_COMPONENT][POSITION_COMPONENT] = positionComponent.serializeComponent();
	return component;
}
