#include "Window.h"

#include <codecvt>
#include <iostream>

WindowClass::WindowClass(const std::wstring& className, WNDPROC wndProc) :
	name(className),
	hInstance(GetModuleHandleW(NULL)) {

	WNDCLASSW desc;

	desc.style = CS_DBLCLKS | CS_OWNDC;
	desc.lpfnWndProc = wndProc;
	desc.cbClsExtra = 0;
	desc.cbWndExtra = 0;
	desc.hInstance = hInstance;
	desc.hIcon = NULL;
	desc.hCursor = LoadCursor(NULL, IDC_IBEAM);
	desc.hbrBackground = NULL;
	desc.lpszMenuName = NULL;
	desc.lpszClassName = name.c_str();

	RegisterClassW(&desc);
}

WindowClass::~WindowClass() {
	UnregisterClassW(name.c_str(), hInstance);
}

std::wstring WindowClass::getName() const {
	return name;
}

WindowClass Window::windowClass(L"J3DWindowClass", Window::wndSetupProc);

Window::Window(int width, int height, const std::string& windowTitle) :
	pKeyboard(std::make_unique<Keyboard>()),
	pGraphics(std::make_unique<Mouse>()) {

	std::wstring wWindowTitle(windowTitle.begin(), windowTitle.end());

	const DWORD style = WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME;

	RECT windowRect{ 0, 0, width, height };
	if (AdjustWindowRect(&windowRect, style, false) == 0) {
		throw std::runtime_error("");
	}

	hWnd = CreateWindowW(
		windowClass.getName().c_str(),
		wWindowTitle.c_str(),
		style,
		CW_USEDEFAULT, CW_USEDEFAULT,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		GetModuleHandleW(NULL),
		this
	);

	if (!hWnd) {
		throw std::runtime_error("yikes");
	}

	ShowWindow(hWnd, SW_SHOW);

	pGfx = std::make_unique<Graphics>(hWnd);
	pGfx->windowResized();
}

Window::~Window() {
	
}

Keyboard* Window::getKeyboard() const {
	return pKeyboard.get();
}

Mouse* Window::getMouse() const {
	return pGraphics.get();
}

Graphics* Window::getGraphics() const {
	return pGfx.get();
}

std::optional<int32_t> Window::poll() {
	MSG msg;

	while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			return (int)msg.wParam;
		}

		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}

	return {};
}

LRESULT Window::wndSetupProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (msg == WM_NCCREATE) {
		const auto pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);

		Window* pWnd = static_cast<Window*>(pCreate->lpCreateParams);
		SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
		SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(wndProc));
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}

LRESULT Window::wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	Window* pWnd = reinterpret_cast<Window*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
	return pWnd->handleMsg(hWnd, msg, wParam, lParam);
}

LRESULT Window::handleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
	switch (msg)
	{
	// if window receives a close message the application should quit
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_WINDOWPOSCHANGED:
	{
		WINDOWPOS* pWndPos = reinterpret_cast<WINDOWPOS*>(lParam);
		if (!(pWndPos->flags & SWP_NOSIZE)) {
			pGfx->windowResized();
		}
		break;
	}

	// Keyboard message handling
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		// if key was up before message or if keyboard wants repeat messages
		if (!(lParam & (1 << 30)) || pKeyboard->repeatOnKeyHeld) {
			pKeyboard->keyPressed(static_cast<unsigned char>(wParam));
		}

		break;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		pKeyboard->keyReleased(static_cast<unsigned char>(wParam));

		break;
	}

	// Mouse message handling
	case WM_MOUSEMOVE:
	{
		pGraphics->mouseMoved(Mouse::State(lParam, wParam));

		break;
	}
	case WM_LBUTTONDOWN:
	{
		pGraphics->leftPressed(Mouse::State(lParam, wParam));
		break;
	}
	case WM_LBUTTONUP:
	{
		pGraphics->leftReleased(Mouse::State(lParam, wParam));
		break;
	}
	case WM_RBUTTONDOWN:
	{
		pGraphics->rightPressed(Mouse::State(lParam, wParam));
		break;
	}
	case WM_RBUTTONUP:
	{
		pGraphics->rightReleased(Mouse::State(lParam, wParam));
		break;
	}
	case WM_MOUSEWHEEL:
	{
		pGraphics->wheelMoved(Mouse::State(lParam, wParam), GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	}
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}

