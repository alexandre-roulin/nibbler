#include "KeyState.hpp"
#include <iostream>

KeyState::KeyState() :
		value_(kNone),
		update_(false),
		countDown_(0),
		countPress_(0),
		countUp_(0),
		tpLastInput_(std::chrono::steady_clock::now()),
		durationLastPress_(0)
{}

KeyState::KeyState(const KeyState& value) :
		value_(value.value_),
		update_(false),
		countDown_(0),
		countPress_(0),
		countUp_(0),
		tpLastInput_(std::chrono::steady_clock::now()),
		durationLastPress_(0)
{}

KeyState::KeyState(eKeyState value) :
value_(value),
update_(false),
countDown_(0),
countPress_(0),
countUp_(0),
tpLastInput_(std::chrono::steady_clock::now()),
durationLastPress_(0)
{}



void KeyState::setState(eKeyState state) {
	update_ = true;
	tpLastInput_ = std::chrono::steady_clock::now();
	if (value_ == kPress && state != kPress)
		durationLastPress_ = std::chrono::duration_cast<std::chrono::milliseconds>(tpLastDown_ - tpLastInput_);
	value_ = state;

	if (value_ != kPress)
		tpLastInput_ = std::chrono::steady_clock::now();
	if (value_ == kDown)
		tpLastDown_ = std::chrono::steady_clock::now();

	if (value_ == kDown)
		++countDown_;
	else if (value_ == kPress)
		++countPress_;
	else if (value_ == kUp)
		++countUp_;
}

void		KeyState::update() {
	if (value_ == kPress)
		durationLastPress_ = std::chrono::duration_cast<std::chrono::milliseconds>(tpLastDown_ - tpLastInput_);
	if (value_ == kUp)
		value_ = kNone;
	if (value_ == kDown) {
		durationLastPress_ = std::chrono::duration_cast<std::chrono::milliseconds>(tpLastDown_ - tpLastInput_);
		value_ = kPress;
	}
}
bool		KeyState::isUpdate() const {
	return update_;
}
void		KeyState::setUpdate(bool update) {
	update_ = update;
}

KeyState	KeyState::getState() const {
	return value_;
}
std::chrono::milliseconds		KeyState::getDurationPress() const {
	return std::chrono::duration_cast<std::chrono::milliseconds>(tpLastDown_ - std::chrono::steady_clock::now());
}
std::chrono::milliseconds		KeyState::getDurationLastPress() const {
	if (countDown_)
		return durationLastPress_;
	return std::chrono::milliseconds(0);
}

KeyState		&KeyState::operator=(KeyState const &rhs) {
	setState(rhs.value_);
	return (*this);
}
KeyState		&KeyState::operator=(eKeyState rhs) {
	setState(rhs);
	return (*this);
}


std::ostream &operator<<(std::ostream &o, KeyState const &keyState) {
	if (keyState.getState() == KeyState::kUp)
		o << "kUp" << std::endl;
	else if (keyState.getState() == KeyState::kPress)
		o << "kPress" << std::endl;
	else if (keyState.getState() == KeyState::kDown)
		o << "kDown" << std::endl;
	else if (keyState.getState() == KeyState::kNone)
		o << "kNone" << std::endl;
	return (o);
}