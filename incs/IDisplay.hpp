#pragma once

#include "Grid.tpp"
#define SPRITE_GROUND 8 * SIZE_LINE_TILESET
#define SPRITE_WALL 8 * SIZE_LINE_TILESET + 1
#define SPRITE_FOOD SIZE_LINE_TILESET
#define PATH_TILESET "./ressource/snake_tileset.png"
#define SIZE_LINE_TILESET 15
//#define PATH_DISPLAY_LIBRARY_SFML "./extern/display_sfml/libdisplay_sfml.so"
//#define PATH_DISPLAY_LIBRARY_SFML "./extern/test_glfw/cmake-build-debug/libGlitterd.so"
#define PATH_DISPLAY_LIBRARY_SFML "./extern/display_sdl/libdisplay_sdl.so"

#define PATH_DISPLAY_LIBRARY_SDL "./extern/display_sdl/libdisplay_sdl.so"
#define PATH_SOUND_LIBRARY_SFML "./extern/sound_sfml/libsound_sfml.so"
#define PATH_SOUND_LIBRARY_SDL "./extern/sound_sdl/libsound_sdl.so"
#define MUSIC_ZELDA "./ressource/sound/zelda.ogg"

#define DISPLAY_DEFAULT_TILESET_PATH "snake_tileset.png"
#define DISPLAY_DEFAULT_TILE_SIZE 32

#ifdef _WIN32
#define DISPLAY_SLASH "\\"
#else
#define DISPLAY_SLASH "/"
#endif

enum class eSprite {
	NONE = 0,

	GREEN = 1,
	BLUE = 2,
	PURPLE = 3,
	PINK = 4,
	GREY = 5,
	YELLOW = 6,
	ORANGE = 7,
	RED = 8,

	MASK_COLOR = 0xFF,

	HEAD = (1 << 8),
	BODY = (1 << 9),
	TAIL = (1 << 10),

	MASK_BODY = HEAD | BODY | TAIL,

	NORTH = (1 << 11),
	SOUTH = (1 << 12),
	EAST = (1 << 13),
	WEST = (1 << 14),

	MASK_DIRECTION = NORTH | SOUTH | EAST | WEST,

	FROM_NORTH = (1 << 15),
	FROM_SOUTH = (1 << 16),
	FROM_EAST = (1 << 17),
	FROM_WEST = (1 << 18),

	MASK_FROM = FROM_NORTH | FROM_SOUTH | FROM_EAST | FROM_WEST,

	TO_NORTH = (1 << 19),
	TO_SOUTH = (1 << 20),
	TO_EAST = (1 << 21),
	TO_WEST = (1 << 22),

	MASK_TO = TO_NORTH | TO_SOUTH | TO_EAST | TO_WEST,

	BITWISE_TO = 8,
	BITWISE_FROM = 4,


	WALL = (1 << 23),
	FOOD = (1 << 24),
	GROUND = (1 << 25)
};

inline bool operator==(eSprite const lhs, eSprite const rhs) {
	return static_cast<int>(lhs) == static_cast<int>(rhs);
}

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
		case eSprite ::GREEN :
			os << "GREEN";
			break;
		case eSprite ::BLUE :
			os << "BLUE";
			break;
		case eSprite ::PURPLE :
			os << "PURPLE";
			break;
		case eSprite ::PINK :
			os << "PINK";
			break;
		case eSprite ::GREY :
			os << "GREY";
			break;
		case eSprite ::YELLOW :
			os << "YELLOW";
			break;
		case eSprite ::ORANGE :
			os << "ORANGE";
			break;
		case eSprite ::RED :
			os << "RED";
			break;
		default :
			break;
	}

	if ((sprite & eSprite::HEAD) == eSprite::HEAD)
		os << "Head";
	else if ((sprite & eSprite::TAIL) == eSprite::TAIL)
		os << "Tail";
	else if ((sprite & eSprite::BODY) == eSprite::BODY)
		os << "Body";
	else if ((sprite & eSprite::WALL) == eSprite::WALL)
		os << "WALL";
	else if ((sprite & eSprite::FOOD) == eSprite::FOOD)
		os << "OOOO";
	else if ((sprite & eSprite::NONE) == eSprite::NONE)
		os << "-N-";
	else
		os << static_cast<int>(sprite);
	return os;
}

enum eDirection {
	NORTH = 1,							// 0000 0001
	SOUTH = 3,							// 0000 0011
	EAST = 4,							// 0000 0100
	WEST = 12							// 0000 1100
};

class IDisplay {
public:

	virtual ~IDisplay() {}

	virtual bool exit(void) const = 0;

	virtual void render(void) = 0;

	virtual void update(void) = 0;

	virtual void drawGrid(Grid<eSprite> const &grid) = 0;

	virtual void setBackground(Grid<eSprite> const &grid) = 0;

	virtual eDirection getDirection(void) const = 0;
};

