#pragma once


#include <json/json.h>

struct CollisionComponent {
private:
	static std::string const NAME_COMPONENT;
	static std::string const IS_WALL;
public:
	explicit CollisionComponent(bool is_wall = true);
	bool isWall;
	Json::Value serializeComponent();
};


