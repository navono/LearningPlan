/// http://www.codeproject.com/Articles/559385/Custom-Controls-in-Win32-API-The-Basics

#include <tchar.h>
#include <windows.h>
#include "custom.h"

static HINSTANCE hInstance;
static HWND hwndCustom;

#define CUSTOM_ID		100
#define MARGIN			7

static LRESULT CALLBACK MainProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
	{
		if (wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED)
		{
			WORD cx = LOWORD(lParam);
			WORD cy = HIWORD(lParam);
			SetWindowPos(hwndCustom, nullptr, MARGIN, MARGIN,
				cx - 2 * MARGIN, cy - 2 * MARGIN, SWP_NOZORDER);
		}
	}
	break;

	case WM_CREATE:
		hwndCustom = CreateWindow(CUSTOM_WC, nullptr, WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hwnd, (HMENU)CUSTOM_ID, hInstance, nullptr);
		break;

	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int APIENTRY _tWinMain(HINSTANCE hInst, HINSTANCE hInstPrev, TCHAR* lpszCmdLine, int iCmdShow)
{
	WNDCLASS wc = { 0 };
	HWND hwnd;
	MSG msg;

	hInstance = hInst;

	CustomRegister();

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wc.lpszClassName = _T("MainWindow");
	RegisterClass(&wc);

	hwnd = CreateWindow(_T("MainWindow"), _T("App Name"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 350, 250, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(hwnd, iCmdShow);

	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (IsDialogMessage(hwnd, &msg))
		{
			continue;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	CustomUnregister();

	return (int)msg.wParam;
}