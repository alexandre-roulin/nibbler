#pragma once

#include "KeyState.hpp"
#include <map>

class KeyStateManager {
public:
	KeyStateManager() = default;
	~KeyStateManager() = default;
	KeyStateManager(KeyState const &) = delete;
	KeyStateManager &operator=(KeyStateManager const &keyState) = delete;


	KeyState::eKeyState       getKeyState(int key) const;
	KeyState const &getKey(int key) const;

protected:
	virtual void update();
	void callbackKey(int key, KeyState::eKeyState state);

private:

	std::map<int, KeyState>		key_;
	std::map<int, KeyState>		keyPast_;

	static KeyState				KeyStateNone_;
	static KeyState::eKeyState	getKeyStateOf_(int key, std::map<int, KeyState> const &keyState_);
};