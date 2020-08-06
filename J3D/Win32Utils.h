#pragma once

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <windows.h>

#include <stdexcept>

inline void tif(HRESULT hr) {
	if (FAILED(hr)) {
		throw std::runtime_error("");
	}
};

namespace Win32Utils {
	
	struct Rect : public RECT {
		Rect(
			int32_t left,
			int32_t top,
			int32_t right,
			int32_t bottom
		) :
			RECT{ left, top, right, bottom } {}

	};

	struct Point : public POINT {
		Point(
			int32_t x,
			int32_t y
		) :
			POINT{ x, y } {}
	};

}
