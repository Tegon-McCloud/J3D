#include "Mouse.h"

Mouse::Event::Event(Type type, const State& state) :
	type(type), state(state) {}

Mouse::Event::Type Mouse::Event::getType() {
	return type;
}

int Mouse::Event::getX() {
	return state.x;
}

int Mouse::Event::getY() {
	return state.y;
}

bool Mouse::Event::leftIsPressed() {
	return state.ldown;
}

bool Mouse::Event::rightIsPressed() {
	return state.rdown;
}

Mouse::State Mouse::Event::getState() {
	return state;
}

std::optional<Mouse::Event> Mouse::nextEvent() {
	if (!events.empty()) {
		Event front = events.front();
		events.pop();
		return front;
	}

	return {};
}

void Mouse::flush() {
	events = {};
}

void Mouse::mouseMoved(const State& state) {
	this->state.x = state.x;
	this->state.y = state.y;

	events.push(Event(Event::Type::Move, state));
}

void Mouse::leftPressed(const State& state) {
	this->state.ldown = true;
	
	events.push(Event(Event::Type::LPress, state));
}

void Mouse::leftReleased(const State& state) {
	this->state.ldown = false;
	
	events.push(Event(Event::Type::LRelease, state));
}


void Mouse::rightPressed(const State& state) {
	this->state.rdown = true;
	
	events.push(Event(Event::Type::RPress, state));
}

void Mouse::rightReleased(const State& state) {
	this->state.rdown = false;
	
	events.push(Event(Event::Type::RRelease, state));
}

void Mouse::wheelMoved(const State& state, int delta) {
	wheelDeltaCarry += delta;

	while (wheelDeltaCarry >= WHEEL_DELTA) {
		wheelUp(state);
		wheelDeltaCarry -= WHEEL_DELTA;
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA) {
		wheelDown(state);
		wheelDeltaCarry += WHEEL_DELTA;
	}

}

void Mouse::wheelUp(const State& state) {
	events.push(Event(Event::Type::WheelUp, state));
}

void Mouse::wheelDown(const State& state) {
	events.push(Event(Event::Type::WheelDown, state));
}

Mouse::State::State(LPARAM lParam, WPARAM wParam) :
	x(HIWORD(lParam)), y(LOWORD(lParam)), ldown(wParam & 0x0001), rdown(wParam & 0x0002) {}
