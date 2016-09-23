#include "custom.h"

static void CustomPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rect;

	GetClientRect(hwnd, &rect);
	hdc = BeginPaint(hwnd, &ps);
	
	SetTextColor(hdc, RGB(0, 0, 0));
	SetBkMode(hdc, TRANSPARENT);
	DrawText(hdc, _T("Hello."), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	EndPaint(hwnd, &ps);
}

static LRESULT CALLBACK CustomProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 1. Many tutorials allocate(and free) the data in WM_CREATE(and WM_DESTROY).
	//	  I recommend to use their "NC" counterparts as the code above shows.
	//	  This is safer because during window creation WM_NCCREATE is the very 
	//	  first message sent and then several other messages are sent before the 
	//	  WM_CREATE eventually comes.If you would ever add a handler for such a 
	//	  message into your code, you might easily introduce a dereference of 
	//	  the NULL pointer, leading to well - known consequences : 
	//	  The application crashes.Similarly it is the message WM_NCDESTROY 
	//	  which is guaranteed to be the last message received when the 
	//    control is being destroyed, and not WM_DESTROY.
	// 2. Note that Windows here does not follow the common error handling 
	//	  pattern that release / terminate code is called only when the 
	//	  corresponding initialization has previously succeeded.
	//	  If WM_NCCREATE fails(i.e., when it returns FALSE), Windows sends 
	//	  WM_NCDESTROY anyway.Hence the app has to make sure the pointer is 
	//	  not NULL before trying to dereference the pointer in the WM_NCDESTROY handler.
	//	  The same would stand for WM_CREATE and WM_DESTROY : if the former returns - 1, 
	//	  the latter is called anyway.
	// 3. Also many tutorials avoid using the "extra bytes" and use GWL_USERDATA 
	//    as a parameter for SetWindowLongPtr() and GetWindowLongPtr().
	//    However for general - purpose control this is a wrong idea as the 
	//    control should leave the GWL_USERDATA for the application as it also 
	//    may need to associate some additional data with the control for its own purpose.



	// Retrieve the pointer to the control data so it may be used in all message
	// handlers below:
	CustomData *pData = (CustomData*)GetWindowLongPtr(hwnd, 0);

	switch (uMsg)
	{
	case WM_NCCREATE:
	{
		// Allocate, setup and remember the control data
		pData = (CustomData*)malloc(sizeof(CustomData));
		if (pData == nullptr)
		{
			return FALSE;
		}
		SetWindowLongPtr(hwnd, 0, (LONG_PTR)pData);
		
		// Initialize CustomData members
		pData->id = 10;
	}
	return TRUE;

	case WM_NCDESTROY:
	{
		if (pData != nullptr)
		{
			free(pData);
			pData = nullptr;
		}
	}
	return 0;
		
	case WM_PAINT:
		CustomPaint(hwnd);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static void
CommonSendSomeNotification(HWND hwnd)
{
	// WPARAM must specify the ID of the control, and LPARAM must point to 
	// NMHDR or a control - specific structure which begins with that.


	//NMHDR hdr;

	//hdr.hwndFrom = hwnd;  // The custom control handle
	//hdr.idFrom = GetWindowLong(hwnd, GWL_ID);
	//hdr.code = XXN_NOTIFICATIONCODE;
	//SendMessage(GetParent(hwnd), WM_NOTIFY, hdr.idFrom, (LPARAM)&hdr);
}

void CustomRegister(void)
{
	WNDCLASS wc = { 0 };

	wc.style = CS_GLOBALCLASS | CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = CustomProc;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = CUSTOM_WC;


	// 如果有额外的数据需要传输，可通过cbWndExtra属性
	wc.cbWndExtra = sizeof(CustomData*);

	RegisterClass(&wc);
}

void CustomUnregister(void)
{
	UnregisterClass(CUSTOM_WC, nullptr);
}

