#ifndef NIBBLER_DATA_HPP
#define NIBBLER_DATA_HPP

#include <IDisplay.hpp>
#include <component/PositionComponent.hpp>
#include <nibbler.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

enum class eHeaderK : uint16_t {
	kChat,				//0
	kOpenGame,			//1
	kStartGame,			//2
	kResizeMap,			//3
	kBorderless,		//4
	kId,				//5
	kPause,				//6
	kInput,				//7
	kFood,				//8
	kForcePause,		//9
	kPock,				//10
	kSnake,				//11
	kSnakeArray,		//12
	kRemoveSnake		//13
};

inline std::ostream &operator<<(std::ostream  &os, eHeaderK const & header) {
	switch (header) {


		case eHeaderK::kRemoveSnake: {
			os << "kRemoveSnake";
			break;
		}
		case eHeaderK::kChat: {
			os << "kChat";
			break;
		}

		case eHeaderK::kOpenGame: {
			os << "kOpenGame";
			break;
		}

		case eHeaderK::kStartGame: {
			os << "kStartGame";
			break;
		}

		case eHeaderK::kResizeMap: {
			os << "kResizeMap";
			break;
		}

		case eHeaderK::kBorderless: {
			os << "kBorderless";
			break;
		}

		case eHeaderK::kId: {
			os << "kId";
			break;
		}

		case eHeaderK::kPause: {
			os << "kPause";
			break;
		}

		case eHeaderK::kInput: {
			os << "kInput";
			break;
		}

		case eHeaderK::kFood: {
			os << "kFood";
			break;
		}

		case eHeaderK::kForcePause: {
			os << "kForcePause";
			break;
		}

		case eHeaderK::kPock: {
			os << "kPock";
			break;
		}

		case eHeaderK::kSnake: {
			os << "kSnake";
			break;
		}

		case eHeaderK::kSnakeArray: {
			os << "kSnakeArray";
			break;
		}

	}
	return os;
}

struct InputInfo {
	InputInfo() = default;
	InputInfo(int16_t id, eDirection dir) : id_(id), dir_(dir) {};
	int16_t id_;
	eDirection dir_;
};

struct FoodInfo {
	FoodInfo()
	: positionComponent(PositionComponent()),fromSnake(false) {

	}

	FoodInfo(
			PositionComponent component,
			bool b
			) :
			positionComponent(component),
			fromSnake(b)
			{

	}

	PositionComponent positionComponent;
	bool fromSnake;

};

struct StartInfo {
	unsigned int nu;
	boost::posix_time::ptime time_duration;
};

struct ChatInfo {
	ChatInfo() {
		std::memset(chat_, 0, CHAT_BUFFER);
		std::memset(offset_, 0, OFFSET);
		std::memset(name_, 0, NAME_BUFFER);
	}
	ChatInfo(std::string const &name, std::string const &chat) {
		std::strncpy(chat_, chat.c_str(), CHAT_BUFFER);
		std::strncpy(name_, name.c_str(), NAME_BUFFER);
	}
	char offset_[OFFSET];
	char chat_[CHAT_BUFFER];
	char name_[NAME_BUFFER];

	std::string toString() {
		std::string message;
		message.append("[");
		message.append(name_);
		message.append("] ");
		message.append(chat_);
		message.resize(SIZEOF_CHAT_PCKT);
		return message;
	}
};
#endif //NIBBLER_DATA_HPP
