#include "pongGame.h"


void KeyboardInput::updateListeners(Key key) {
	for (auto i = listenerList.begin(); i != listenerList.end(); i++) {
		if (key.state == true) {
			(*i)->buttonPressed(key);
		}
		else {
			(*i)->buttonReleased(key);
		}
	}
}

void KeyboardInput::keyboardRead(bool* listen) {
	vector<Key> keys = getKeys();

	while (*listen) {
		for (int i = 0; i < keys.size(); i++) {
			if (GetKeyState(keys[i].key) & 0x8000) {
				keyState({ keys[i].key, true });
			}
			else {
				keyState({ keys[i].key, false });
			}
			Sleep(5);
		}
	}
}

KeyboardInput::KeyboardInput() {
	listen = true;
	thrKeyboard = thread(&KeyboardInput::keyboardRead, this, &listen);
}

KeyboardInput::~KeyboardInput() {
	listen = false;
	thrKeyboard.join();
}

void KeyboardInput::addListener(InputListener* listener) {
	listenerList.push_back(listener);
}

void KeyboardInput::removeListener(InputListener* listener) {
	listenerList.remove(listener);
}

void KeyboardInput::keyState(Key k) {
	for (int i = 0; i < keys.size(); i++) {
		if (k.key == keys[i].key) {
			if (k.state != keys[i].state) {
				keys[i].state = k.state;
				updateListeners(k);
			}
		}
	}
}

vector<Key> KeyboardInput::getKeys() {
	return keys;
}