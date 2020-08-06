#pragma once
#include <jni.h>

#include <queue>
#include <bitset>
#include <optional>

class Keyboard {
	friend class Window;
public:
	class Event {
	public:
		Event(unsigned char key, bool press);
		
		unsigned char getKey();
		bool isPress();
		bool isRelease();

	private:
		unsigned char key;
		bool press;
	};
	
public:
	Keyboard(bool repeatOnKeyHeld = false);

	bool isKeyDown(unsigned char key) const;
	std::optional<Event> readKey();
	void flush();

private:
	void keyPressed(unsigned char key);
	void keyReleased(unsigned char key);

	static constexpr size_t nKeys = 1 << (sizeof(unsigned char) * CHAR_BIT);
	
	std::bitset<nKeys> keyStates;
	std::queue<Event> events;
	bool repeatOnKeyHeld;

};


