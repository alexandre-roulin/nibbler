#include "JoystickEvent.hpp"


JoystickEvent::JoystickEvent(eJoystick input, std::string tag_player)
		: joystick(input),
		  tag_player(tag_player) {

}
