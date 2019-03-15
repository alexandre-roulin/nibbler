#ifndef NIBBLER_DATA_HPP
#define NIBBLER_DATA_HPP

#include <IDisplay.hpp>
#include <component/PositionComponent.hpp>
#include <nibbler.hpp>
#include <boost/date_time/posix_time/ptime.hpp>

enum class eHeader : uint16_t {
	kChat = 1,
	kOpenGame,
	kStartGame,
	kResizeMap,
	kBorderless,
	kId,
	kPause,
	kInput,
	kFood,
	kForcePause,
	kPock,
	kSnake,
	kSnakeArray,
	kCloseConnection,
	kSnakeUI,
	kSnakeUX,
	kSnakeUN,
	kShowScore,
	kGameSpeed,
	kBarrier

};

inline std::ostream &operator<<(std::ostream  &os, eHeader const & header) {
	switch (header) {


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

struct FoodInfo {
	FoodInfo()
	: positionComponent(PositionComponent()),fromSnake(false), id_(0) {

	}

	FoodInfo(PositionComponent component,bool b, int16_t id) :
			positionComponent(component),
			fromSnake(b),
			id_(id)
			{

	}

	PositionComponent positionComponent;
	bool fromSnake;
	int16_t id_;
};

struct StartInfo {
	size_t nu;
	boost::posix_time::ptime time_duration;
};

class ChatInfo {
public:
	ChatInfo() {
		std::memset(chat_, 0, CHAT_BUFFER);
		std::memset(name_, 0, NAME_BUFFER);
	}
	ChatInfo(std::string const &name, std::string const &chat) {
		std::strncpy(name_, name.c_str(), NAME_BUFFER);
		std::strncpy(chat_, chat.c_str(), CHAT_BUFFER);
	}
	char offset_[OFFSET];
	char chat_[CHAT_BUFFER];
	char name_[NAME_BUFFER];
	size_t _strlen(const char *string, size_t max) {
		size_t index = 0;
		while (string[index] && index < max) index++;
		return index;
	}

	std::string toString() {
		std::string message;
		message.append("[", 1);
		message.append(name_, _strlen(name_, NAME_BUFFER - 1));
		message.append("] ", 2);
		message.append(chat_, _strlen(chat_, CHAT_BUFFER - 1));
		return message;
	}
};

struct ScoreInfo {
	std::string name;
	uint16_t 	score;
	ScoreInfo(std::string const &n, uint16_t s) : name(n), score(s) {};
};

#endif //NIBBLER_DATA_HPP
