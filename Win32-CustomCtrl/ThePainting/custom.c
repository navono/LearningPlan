/* File custom.c
 * (custom control implementation)
 */

#include "custom.h"
#include <wingdi.h>


#define CELLSIZE        48
#define DARKCOLOR       RGB(0,47,127)
#define LIGHTCOLOR      RGB(241,179,0)


typedef struct CustomData {
	HWND hwnd;
	DWORD style;
	HBRUSH hbrLight;
	HBRUSH hbrDark;
} CustomData;


static void
CustomPaint(CustomData* pData, HDC hDC, RECT* rcDirty, BOOL bErase)
{
	int x, y;
	RECT r;
	HBRUSH hBrush;

	// Note we paint only the cells overlapping with the dirty rectangle.
	for(y = rcDirty->top / CELLSIZE; y <= rcDirty->bottom / CELLSIZE; y++) {
		for(x = rcDirty->left / CELLSIZE; x <= rcDirty->right / CELLSIZE; x++) {
			hBrush = ((x+y) % 2 == 0)  ?  pData->hbrLight  :  pData->hbrDark;
			SetRect(&r, x * CELLSIZE, y * CELLSIZE, (x+1) * CELLSIZE, (y+1) * CELLSIZE);
			FillRect(hDC, &r, hBrush);
		}
	}
}

static void
CustomDoubleBuffer(CustomData* pData, PAINTSTRUCT* pPaintStruct)
{
    int cx = pPaintStruct->rcPaint.right - pPaintStruct->rcPaint.left;
    int cy = pPaintStruct->rcPaint.bottom - pPaintStruct->rcPaint.top;
    HDC hMemDC;
    HBITMAP hBmp;
    HBITMAP hOldBmp;
    POINT ptOldOrigin;

    // Create new bitmap-back device context, large as the dirty rectangle.
    hMemDC = CreateCompatibleDC(pPaintStruct->hdc);
    hBmp = CreateCompatibleBitmap(pPaintStruct->hdc, cx, cy);
    hOldBmp = SelectObject(hMemDC, hBmp);

    // Do the painting into the memory bitmap.
    OffsetViewportOrgEx(hMemDC, -(pPaintStruct->rcPaint.left),
                        -(pPaintStruct->rcPaint.top), &ptOldOrigin);
    CustomPaint(pData, hMemDC, &pPaintStruct->rcPaint, TRUE);
    SetViewportOrgEx(hMemDC, ptOldOrigin.x, ptOldOrigin.y, NULL);

    // Blit the bitmap into the screen. This is really fast operation and altough
    // the CustomPaint() can be complex and slow there will be no flicker any more.
    BitBlt(pPaintStruct->hdc, pPaintStruct->rcPaint.left, pPaintStruct->rcPaint.top,
           cx, cy, hMemDC, 0, 0, SRCCOPY);

    // Clean up.
    SelectObject(hMemDC, hOldBmp);
    DeleteObject(hBmp);
    DeleteDC(hMemDC);
}

static LRESULT CALLBACK
CustomProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CustomData* pData = (CustomData*) GetWindowLongPtr(hwnd, 0);

    switch(uMsg) {
        case WM_ERASEBKGND:
            return FALSE;  // Defer erasing into WM_PAINT

        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            BeginPaint(hwnd, &ps);
			// We let application to choose whether to use double buffering or not by using the style CCS_DOUBLEBUFFER.
            if(pData->style & XXS_DOUBLEBUFFER)
                CustomDoubleBuffer(pData, &ps);
            else
                CustomPaint(pData, ps.hdc, &ps.rcPaint, ps.fErase);
            EndPaint(hwnd, &ps);
            return 0;
		}

		case WM_PRINTCLIENT:
        {
            RECT rc;
            GetClientRect(hwnd, &rc);
            CustomPaint(pData, (HDC) wParam, &rc, TRUE);
            return 0;
        }

		case WM_STYLECHANGED:
			if(wParam == GWL_STYLE)
				pData->style = lParam;
			break;

		case WM_NCCREATE:
			pData = (CustomData*) malloc(sizeof(CustomData));
			if(pData == NULL)
				return FALSE;
			SetWindowLongPtr(hwnd, 0, (LONG_PTR)pData);
			pData->hwnd = hwnd;
			pData->style = ((CREATESTRUCT*)lParam)->style | XXS_DOUBLEBUFFER;
			pData->hbrDark = CreateSolidBrush(DARKCOLOR);
			pData->hbrLight = CreateSolidBrush(LIGHTCOLOR);
			return TRUE;

		case WM_NCDESTROY:
			if(pData != NULL) {
				DeleteObject(pData->hbrDark);
				DeleteObject(pData->hbrLight);
				free(pData);
			}
			return 0;
    }
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void
CustomRegister(void)
{
    WNDCLASS wc = { 0 };

	// Note we do not use CS_HREDRAW and CS_VREDRAW.
	// This means when the control is resized, WM_SIZE (as handled by DefWindowProc()) 
	// invalidates only the newly uncovered area.
	// With those class styles, it would invalidate complete client rectangle.
    wc.style = CS_GLOBALCLASS;
    wc.lpfnWndProc = CustomProc;
	wc.cbWndExtra = sizeof(CustomData*);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CUSTOM_WC;
    RegisterClass(&wc);
}

void
CustomUnregister(void)
{
    UnregisterClass(CUSTOM_WC, NULL);
}
