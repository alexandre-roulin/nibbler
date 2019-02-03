#include "KeyStateManager.hpp"

KeyState::eKeyState       KeyStateManager::getKeyState(int key) const {
	for (auto keyState : key_) {
		if (keyState.first == key)
			return (keyState.second);
	}
	return (KeyState::kNone);
}
KeyState const &KeyStateManager::getKey(int key) const {
	for (auto &keyState : key_) {
		if (keyState.first == key)
			return (keyState.second);
	}
	return (KeyStateManager::KeyStateNone_);
}

void KeyStateManager::update() {

	keyPast_.clear();
	std::for_each(key_.begin(), key_.end(),
				  [&](std::pair< int, KeyState > const &pair){
					  if (pair.second.getState())
						  keyPast_.insert(pair);
				  });

	for (auto &key : key_) {
		if (!key.second.isUpdate())
			key.second.update();
		else
			key.second.setUpdate(false);
	}
}

void            			KeyStateManager::callbackKey(int key, KeyState::eKeyState state) {

	KeyState pastKeyState = KeyStateManager::getKeyStateOf_(key, keyPast_);

	if (state == KeyState::kUp) {
		key_[key].setState(KeyState::kUp);
	}
	else if (!pastKeyState && state == KeyState::kPress) {
		key_[key].setState(KeyState::kDown);
	}
	else if (pastKeyState == KeyState::kDown) {
		key_[key].setState(KeyState::kPress);
	}
	else if (state == KeyState::kDown) {
		key_[key].setState(KeyState::kDown);
	}
	else if (state == KeyState::kPress) {
		key_[key].setState(KeyState::kPress);
	}
}

KeyState::eKeyState		KeyStateManager::getKeyStateOf_(int key, std::map<int, KeyState> const &keyStateMap) {
	for (auto &keyState : keyStateMap) {
		if (keyState.first == key)
			return (keyState.second);
	}
	return (KeyState::kNone);
}

KeyState       KeyStateManager::KeyStateNone_;
