#pragma once

#include "Win32Utils.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"

#include <string>
#include <optional>
#include <memory>

class WindowClass {
public:
	WindowClass(const std::wstring& className, WNDPROC wndProc);
	~WindowClass();

	std::wstring getName() const;

private:
	const HINSTANCE hInstance;
	const std::wstring name;
};

class Window {
public:
	Window(int width, int height, const std::string& windowTitle = "");
	~Window();

	Keyboard* getKeyboard() const;
	Mouse* getMouse() const;
	Graphics* getGraphics() const;

	static std::optional<int32_t> poll();

private:
	static LRESULT CALLBACK wndSetupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static WindowClass windowClass;

	LRESULT handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND hWnd;

	std::unique_ptr<Keyboard> pKeyboard;
	std::unique_ptr<Mouse> pGraphics;
	std::unique_ptr<Graphics> pGfx;
};
