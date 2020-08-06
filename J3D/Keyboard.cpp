#include "Keyboard.h"

void Keyboard::keyPressed(unsigned char key) {
    keyStates[key] = true;

    events.push(Event(key, true));
}

void Keyboard::keyReleased(unsigned char key) {
    keyStates[key] = false;

    events.push(Event(key, false));
}

Keyboard::Keyboard(bool repeatOnKeyHeld) : repeatOnKeyHeld(repeatOnKeyHeld) {}

bool Keyboard::isKeyDown(unsigned char key) const {
    return keyStates[key];
}

std::optional<Keyboard::Event> Keyboard::readKey() {
    if (!events.empty()) {
        Event front = events.front();
        events.pop();
        return front;
    }

    return {};
}

void Keyboard::flush() {
    events = {};
}

Keyboard::Event::Event(unsigned char key, bool press) :
    key(key), 
    press(press) {}

unsigned char Keyboard::Event::getKey() {
    return key;
}

bool Keyboard::Event::isPress() {
    return press;
}

bool Keyboard::Event::isRelease() {
    return !press;
}
