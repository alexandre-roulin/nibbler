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
	kTree = (1 << 26),

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

#endif //NIBBLER_SPRITE_HPP
