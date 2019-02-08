#ifndef NIBBLER_SPRITE_HPP
#define NIBBLER_SPRITE_HPP

#include <iostream>

enum class eSprite {
	kNone = 0,

	kGreen = 1,
	kBlue = 2,
	kPurple = 3,
	kPink = 4,
	kGrey = 5,
	kYellow = 6,
	kOrange = 7,
	kRed = 8,

	kMaskColor = 0xFF,

	kHead = (1 << 8),
	kBody = (1 << 9),
	kTail = (1 << 10),

	kMaskBody = kHead | kBody | kTail,

	kNorth = (1 << 11),
	kSouth = (1 << 12),
	kEast = (1 << 13),
	kWest = (1 << 14),

	kMaskDirection = kNorth | kSouth | kEast | kWest,

	kFromNorth = (1 << 15),
	kFromSouth = (1 << 16),
	kFromEast = (1 << 17),
	kFromWest = (1 << 18),

	kMaskFrom = kFromNorth | kFromSouth | kFromEast | kFromWest,

	kToNorth = (1 << 19),
	kToSouth = (1 << 20),
	kToEast = (1 << 21),
	kToWest = (1 << 22),

	kMaskTo = kToNorth | kToSouth | kToEast | kToWest,

	kBitwiseTo = 8,
	kBitwiseFrom = 4,


	kWall = (1 << 23),
	kFood = (1 << 24),
	kGround = (1 << 25),

	kYourSnake = (1 << 30)
};

inline eSprite operator|(eSprite const lhs, eSprite const rhs) {
	return static_cast<eSprite> (static_cast<int>(lhs) | static_cast<int>(rhs));
}
inline eSprite const &operator|=(eSprite &lhs, eSprite const &rhs) {
	lhs = static_cast<eSprite> (static_cast<int>(lhs) | static_cast<int>(rhs));
	return (lhs);
}
inline eSprite operator&(eSprite const lhs, eSprite const rhs) {
	return static_cast<eSprite> (static_cast<int>(lhs) &  static_cast<int>(rhs));
}
inline eSprite operator&(int const lhs, eSprite const rhs) {
	return static_cast<eSprite> (lhs &  static_cast<int>(rhs));
}
inline eSprite operator&(eSprite const lhs, int const rhs) {
	return static_cast<eSprite> (static_cast<int>(lhs) & rhs);
}
inline eSprite operator^(int const lhs, eSprite const rhs) {
	return static_cast<eSprite> (lhs ^  static_cast<int>(rhs));
}
inline eSprite operator^(eSprite const lhs, int const rhs) {
	return static_cast<eSprite> (static_cast<int>(lhs) ^ rhs);
}
inline eSprite operator<<(eSprite const lhs, eSprite const rhs) {
	return static_cast<eSprite> (static_cast<int>(lhs) << static_cast<int>(rhs));
}
inline eSprite operator>>(eSprite const lhs, eSprite const rhs) {
	return static_cast<eSprite> (static_cast<int>(lhs) >> static_cast<int>(rhs));
}
inline std::ostream &operator<<(std::ostream &os, eSprite &sprite) {
	switch (sprite) {
		case eSprite ::kGreen :
			os << "GREEN";
			break;
		case eSprite ::kBlue :
			os << "BLUE";
			break;
		case eSprite ::kPurple :
			os << "PURPLE";
			break;
		case eSprite ::kPink :
			os << "PINK";
			break;
		case eSprite ::kGrey :
			os << "GREY";
			break;
		case eSprite ::kYellow :
			os << "YELLOW";
			break;
		case eSprite ::kOrange :
			os << "ORANGE";
			break;
		case eSprite ::kRed :
			os << "RED";
			break;
		default :
			break;
	}

	if ((sprite & eSprite::kHead) == eSprite::kHead)
		os << "Head";
	else if ((sprite & eSprite::kTail) == eSprite::kTail)
		os << "Tail";
	else if ((sprite & eSprite::kBody) == eSprite::kBody)
		os << "Body";
	else if ((sprite & eSprite::kWall) == eSprite::kWall)
		os << "WALL";
	else if ((sprite & eSprite::kFood) == eSprite::kFood)
		os << "OOOO";
	else if ((sprite & eSprite::kNone) == eSprite::kNone)
		os << "-N-";
	else
		os << static_cast<int>(sprite);
	return os;
}

#endif //NIBBLER_SPRITE_HPP
