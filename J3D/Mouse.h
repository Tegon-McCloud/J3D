#pragma once

#include <queue>
#include <optional>
#include "Win32Utils.h"

class Mouse {
	friend class Window;
public:
	struct State {
		State() = default;
		State(LPARAM lParam, WPARAM wParam);
		int x, y;
		bool ldown, rdown;
	};

	class Event {
	public:
		enum class Type {
				LPress,
				LRelease,
				RPress,
				RRelease,
				WheelUp,
				WheelDown,
				Move,
		};

		Event(Type type, const State& state);

		Type getType();
		int getX();
		int getY();
		bool leftIsPressed();
		bool rightIsPressed();
		State getState();

	private:
		Type type;
		State state;
	};

public:
	std::optional<Event> nextEvent();
	void flush();
private:
	void mouseMoved(const State& state);
	void leftPressed(const State& state);
	void leftReleased(const State& state);
	void rightPressed(const State& state);
	void rightReleased(const State& state);
	void wheelMoved(const State& state, int delta);
	void wheelUp(const State& state);
	void wheelDown(const State& state);

	State state;
	int wheelDeltaCarry;
	std::queue<Event> events;
	
};
