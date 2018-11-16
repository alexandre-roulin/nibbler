#pragma once

#include <KINU/Entity.hpp>
#include <ostream>
#include "PositionComponent.hpp"

struct PreviousComponent {
private:
	static std::string const SKIP;
	static std::string const ID_FOLLOWED;
	static std::string const POSITION_COMPONENT;
public:
	friend std::ostream &
	operator<<(std::ostream &os, const PreviousComponent &component);

	static std::string const NAME_COMPONENT;

	explicit PreviousComponent(KINU::Entity::Id id = 0, bool skip = false);

	explicit PreviousComponent(Json::Value);

	KINU::Entity::Id idPrevious;
	PositionComponent positionComponent;
	bool skip;

	Json::Value serializeComponent();
};


