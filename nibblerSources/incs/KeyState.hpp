#pragma once

#include <chrono>
#include <iostream>

class KeyState {
public:
	enum eKeyState {
		kNone = (1 << 0),
		kDown = (1 << 1),
		kPress = (1 << 2),
		kUp = (1 << 3),

		kMaskOn = (kDown | kPress | kUp),
		kMaskOff = (kNone)
	};

	KeyState();
	KeyState(const KeyState& value);
	KeyState(eKeyState value);

	void setState(eKeyState state);

	void		update();
	bool		isUpdate() const;
	bool		setUpdate(bool update);

	KeyState	getState() const;
	std::chrono::milliseconds		getDurationPress() const ;
	std::chrono::milliseconds		getDurationLastPress() const ;


	KeyState		&operator=(KeyState const &rhs);
	KeyState		&operator=(eKeyState rhs);
	inline operator eKeyState() const;
	inline bool		operator==(KeyState const &rhs) const;
	inline bool		operator==(KeyState::eKeyState rhs) const;
	inline bool		operator!=(KeyState::eKeyState rhs) const;
	inline bool		operator!=(KeyState const &rhs) const;
	inline bool operator!() const;
	inline operator bool() const;

private:
	eKeyState 	value_;
	bool 		update_;
	size_t		countDown_;
	size_t		countPress_;
	size_t		countUp_;

	std::chrono::steady_clock::time_point	tpLastInput_;
	std::chrono::steady_clock::time_point	tpLastDown_;
	std::chrono::milliseconds				durationLastPress_;
};

inline KeyState::operator eKeyState() const {
	return value_;
}
inline bool		KeyState::operator==(KeyState const &rhs) const {
	return value_ == rhs.value_;
}
inline bool		KeyState::operator==(KeyState::eKeyState rhs) const {
	return value_ == rhs;
}
inline bool		KeyState::operator!=(KeyState::eKeyState rhs) const {
	return value_ != rhs;
}
inline bool		KeyState::operator!=(KeyState const &rhs) const {
	return value_ != rhs.value_;
}
inline bool KeyState::operator!() const {
	return value_ & kMaskOff;
}
inline KeyState::operator bool() const {
	return value_ & kMaskOn;
}

std::ostream &operator<<(std::ostream &o, KeyState const &keyState);