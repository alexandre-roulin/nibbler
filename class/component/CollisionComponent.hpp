#pragma once
#include <json/json.h>

struct CollisionComponent {
private:
	static std::string const IS_WALL;
public:
	static std::string const NAME_COMPONENT;
	explicit CollisionComponent();
	CollisionComponent(Json::Value);
	Json::Value serializeComponent();
};


