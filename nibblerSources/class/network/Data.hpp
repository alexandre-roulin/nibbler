#ifndef NIBBLER_DATA_HPP
#define NIBBLER_DATA_HPP

#include <IDisplay.hpp>
#include <component/PositionComponent.hpp>
#include <nibbler.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

enum class eHeader : uint16_t {
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
	kSnakeUIArray,		//12
	kSnakeUXArray,		//12
	kBaseSnakeArray,		//12
	kRemoveSnake,		//13
	kAddScore,			//14
	kCloseConnection,	//15
	kKillSnake,			//16
	kSnakeUI,
	kSnakeUX,
	kBaseSnake
};

inline std::ostream &operator<<(std::ostream  &os, eHeader const & header) {
	switch (header) {

		case eHeader::kAddScore: {
			os << "kAddScore";
			break;
		}
		case eHeader::kRemoveSnake: {
			os << "kRemoveSnake";
			break;
		}
		case eHeader::kChat: {
			os << "kChat";
			break;
		}

		case eHeader::kOpenGame: {
			os << "kOpenGame";
			break;
		}

		case eHeader::kStartGame: {
			os << "kStartGame";
			break;
		}

		case eHeader::kResizeMap: {
			os << "kResizeMap";
			break;
		}

		case eHeader::kBorderless: {
			os << "kBorderless";
			break;
		}

		case eHeader::kId: {
			os << "kId";
			break;
		}

		case eHeader::kPause: {
			os << "kPause";
			break;
		}

		case eHeader::kInput: {
			os << "kInput";
			break;
		}

		case eHeader::kFood: {
			os << "kFood";
			break;
		}

		case eHeader::kForcePause: {
			os << "kForcePause";
			break;
		}

		case eHeader::kPock: {
			os << "kPock";
			break;
		}

		case eHeader::kSnake: {
			os << "kSnake";
			break;
		}

		case eHeader::kSnakeArray: {
			os << "kSnakeArray";
			break;
		}
		default :
			break;
	}

	return os;
}

struct InputInfo {
	InputInfo() = default;
	InputInfo(uint16_t id, eDirection dir) : id_(id), dir_(dir) {};
	uint16_t id_;
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
	size_t nu;
	boost::posix_time::ptime time_duration;
};

struct ChatInfo {
	ChatInfo() {
		std::memset(chat_, 0, CHAT_BUFFER);
		std::memset(offset_, 0, OFFSET);
		std::memset(name_, 0, NAME_BUFFER);
	}
	ChatInfo(std::string const &name, std::string const &chat) {
		std::strncpy(name_, name.c_str(), NAME_BUFFER);
		std::strncpy(chat_, chat.c_str(), CHAT_BUFFER);
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
