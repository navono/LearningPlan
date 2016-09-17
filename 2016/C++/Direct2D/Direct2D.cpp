// Direct2D.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Direct2D.h"

#define MAX_LOADSTRING 100


#pragma comment(lib, "d2d1")
#pragma comment(lib, "Dwrite")


// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	// Use HeapSetInformation to specify that the process should
	// terminate if the heap manager detects an error in any heap used
	// by the process.
	// The return value is ignored, because we want to continue running in the
	// unlikely event that HeapSetInformation fails.
	HeapSetInformation(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0);

	if (SUCCEEDED(CoInitialize(nullptr)))
	{
		{
			Direct2DApp app;

			if (SUCCEEDED(app.Initialize()))
			{
				app.RunMessageLoop();
			}
		}

		CoUninitialize();
	}

	return 0;
}

Direct2DApp::Direct2DApp()
	: m_hwnd(nullptr)
	, _pDirect2dFactory(nullptr)
	, _pRenderTarget(nullptr)
	, _pLightSlateGrayBrush(nullptr)
	, _pCornflowerBlueBrush(nullptr)
	, _pPathGeometry(nullptr)
	, _pBlackBrush(nullptr)
	, _pLGBrush(nullptr)
	, _pDWriteFactory(nullptr)
	, _pTextFormat(nullptr)
{

}

Direct2DApp::~Direct2DApp()
{
	SafeRelease(&_pDirect2dFactory);
	SafeRelease(&_pRenderTarget);
	SafeRelease(&_pLightSlateGrayBrush);
	SafeRelease(&_pCornflowerBlueBrush);
	SafeRelease(&_pPathGeometry);
}

HRESULT Direct2DApp::Initialize()
{
	HRESULT hr;

	// Initialize device-independent resources, such as the Direct2D factory
	hr = CreateDeviceIndependentResources();
	if (SUCCEEDED(hr))
	{
		// Register the window class
		WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
		wcex.style = CS_HREDRAW | CS_VREDRAW;
		wcex.lpfnWndProc = Direct2DApp::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = sizeof(LONG_PTR);
		wcex.hInstance = HINST_THISCOMPONENT;
		wcex.hbrBackground = nullptr;
		wcex.lpszMenuName = nullptr;
		wcex.hCursor = LoadCursor(nullptr, IDI_APPLICATION);
		wcex.lpszClassName = L"Direct2DApp";

		RegisterClassEx(&wcex);

		// Because the CreateWindow function takes its size in pixels,
		// obtain the system DPI and use it to scale the window size
		//FLOAT _dpix, _dpiy;

		// The factory returns the current system DPI. This is also the value
		// it will use to create its own windows.
		_pDirect2dFactory->GetDesktopDpi(&_dpix, &_dpiy);

		m_hwnd = CreateWindow(
			L"Direct2DApp",
			L"Direct2D Demp App",
			WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			static_cast<UINT>(ceil(640.f * _dpix / 96.f)),
			static_cast<UINT>(ceil(480.f * _dpix / 96.f)),
			nullptr,
			nullptr,
			HINST_THISCOMPONENT,
			this);
			
		hr = m_hwnd ? S_OK : E_FAIL;
		if (SUCCEEDED(hr))
		{
			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
		}
	}

	return hr;
}

void Direct2DApp::RunMessageLoop()
{
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

HRESULT Direct2DApp::CreateDeviceIndependentResources()
{
	// Direct2D provides two types of resources : device - independent resources that can last for the duration of the application, 
	// and device - dependent resources.
	// Device - dependent resources are associated with a particular rendering device and will cease to function if that device is removed.

	HRESULT hr = S_OK;

	// Create a Direct2D factory
	// D2D1_FACTORY_TYPE_SINGLE_THREADED constant merely specifies that the 
	// factory object, as well as any and all objects rooted in that factory, 
	// may be accessed by only a single thread at a time.
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_pDirect2dFactory);

	ID2D1GeometrySink *pSink;
	_pDirect2dFactory->CreatePathGeometry(&_pPathGeometry);

	_pPathGeometry->Open(&pSink);
	pSink->SetFillMode(D2D1_FILL_MODE_ALTERNATE);

	pSink->BeginFigure(
		D2D1::Point2F(50, 50),
		D2D1_FIGURE_BEGIN_FILLED
		);

	pSink->AddLine(D2D1::Point2F(100, 50));
	pSink->AddLine(D2D1::Point2F(100, 30));

	pSink->AddLine(D2D1::Point2F(150, 60));	// 右顶点

	pSink->AddLine(D2D1::Point2F(100, 90));

	pSink->AddLine(D2D1::Point2F(100, 70));
	pSink->AddLine(D2D1::Point2F(50, 70));


	//pSink->AddLine(D2D1::Point2F(200, 0));

	//pSink->AddBezier(
	//	D2D1::BezierSegment(
	//	D2D1::Point2F(150, 50),
	//	D2D1::Point2F(150, 150),
	//	D2D1::Point2F(200, 200))
	//	);

	//pSink->AddLine(D2D1::Point2F(0, 200));

	//pSink->AddBezier(
	//	D2D1::BezierSegment(
	//	D2D1::Point2F(50, 150),
	//	D2D1::Point2F(50, 50),
	//	D2D1::Point2F(0, 0))
	//	);


	pSink->EndFigure(D2D1_FIGURE_END_CLOSED);
	pSink->Close();
	SafeRelease(&pSink);

	hr = DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED, 
		__uuidof(IDWriteFactory), 
		reinterpret_cast<IUnknown**>(&_pDWriteFactory));

	_text = L"优先级";
	_textLen = (UINT32)wcslen(_text);

	if (SUCCEEDED(hr))
	{
		_pDWriteFactory->CreateTextFormat(
			//L"Consolas",
			L"宋体",
			nullptr,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			50.0f,
			//L"en-us",
			L"zh-cn",
			&_pTextFormat);
	}

	if (SUCCEEDED(hr))
	{
		hr = _pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	}

	if (SUCCEEDED(hr))
	{
		hr = _pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	}

	return hr;
}

/// This method creates the window's device-dependent resources, a render target, 
/// and two brushes. Retrieve the size of the client area and create an 
/// ID2D1HwndRenderTarget of the same size that renders to the window's HWND. 
/// Store the render target in the m_pRenderTarget class member.
HRESULT Direct2DApp::CreateDeviceResouces()
{
	HRESULT hr = S_OK;

	if (_pRenderTarget == nullptr)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(
			rc.right - rc.left,
			rc.bottom - rc.top);

		// Create a Direct2D render target
		hr = _pDirect2dFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&_pRenderTarget);

		// Use the target to create a gray ID2DSolidColorBrush and a cornflower
		// blue ID2DSolidColorBrush
		if (SUCCEEDED(hr))
		{
			hr = _pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&_pLightSlateGrayBrush);
		}

		if (SUCCEEDED(hr))
		{
			hr = _pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&_pCornflowerBlueBrush);
		}

		hr = _pRenderTarget->CreateSolidColorBrush( D2D1::ColorF(D2D1::ColorF::Black),
			&_pBlackBrush);

		if (SUCCEEDED(hr))
		{
			// Create a linear gradient.
			static const D2D1_GRADIENT_STOP stops[] =
			{
				{   0.f,  { 0.f, 1.f, 1.f, 0.25f }  },
				{   1.f,  { 0.f, 0.f, 1.f, 1.f }  },
			};

			ID2D1GradientStopCollection *pGradientStops;
			hr = _pRenderTarget->CreateGradientStopCollection(
				stops,
				ARRAYSIZE(stops),
				&pGradientStops
				);

			if (SUCCEEDED(hr))
			{
				hr = _pRenderTarget->CreateLinearGradientBrush(
					&D2D1::LinearGradientBrushProperties(D2D1::Point2F(100, 0), D2D1::Point2F(100, 200)),
					&D2D1::BrushProperties(),
					pGradientStops,
					&_pLGBrush
					);
			}

			SafeRelease(&pGradientStops);
		}
	}

	return hr;
}

HRESULT Direct2DApp::OnRender()
{
	HRESULT hr = S_OK;
	hr = CreateDeviceResouces();
	if (SUCCEEDED(hr))
	{
		// 单位矩阵
		_pRenderTarget->BeginDraw();
		_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		// Retrieve the size of the drawing area.
		D2D1_SIZE_F rtSize = _pRenderTarget->GetSize();

		// Draw a grid background
		auto width = static_cast<int>(rtSize.width);
		auto height = static_cast<int>(rtSize.height);
		for (int x = 0; x < width; x += 10)
		{
			_pRenderTarget->DrawLine(
				D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
				D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
				_pLightSlateGrayBrush,
				0.5f);
		}

		for (int y = 0; y < height; y += 10)
		{
			_pRenderTarget->DrawLine(
				D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
				D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
				_pLightSlateGrayBrush,
				0.5f);
		}

		// Draw two rectangles
		D2D1_RECT_F rectangle1 = D2D1::RectF(
			rtSize.width / 2 - 50.0f,
			rtSize.height / 2 - 50.0f,
			rtSize.width / 2 + 50.0f,
			rtSize.height / 2 + 50.0f);

		D2D1_RECT_F rectangle2 = D2D1::RectF(
			rtSize.width / 2 - 100.0f,
			rtSize.height / 2 - 100.0f,
			rtSize.width / 2 + 100.0f,
			rtSize.height / 2 + 100.0f);

		//// Draw a filled rectangle
		//_pRenderTarget->FillRectangle(&rectangle1, _pLightSlateGrayBrush);

		//// Draw the outline of a rectangle
		//_pRenderTarget->DrawRectangle(&rectangle2, _pCornflowerBlueBrush);


		//_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(50.f, 50.f));
		_pRenderTarget->DrawGeometry(_pPathGeometry, _pBlackBrush, 1.f);
		_pRenderTarget->FillGeometry(_pPathGeometry, _pLGBrush);


		D2D1_RECT_F layoutRect = D2D1::RectF(60.f, 15.f, 420.f, 100.f);
		_pRenderTarget->DrawText(_text, _textLen, _pTextFormat, layoutRect, _pBlackBrush);

		hr = _pRenderTarget->EndDraw();

		if (hr == D2DERR_RECREATE_TARGET)
		{
			hr = S_OK;
			DiscardDeviceResources();
		}
	}

	return hr;
}

void Direct2DApp::OnResize(UINT width, UINT height)
{
	if (_pRenderTarget != nullptr)
	{
		// Note: This method can fail, but it's okay to ignore the error here,
		// because the error will be returned again the next time EndDraw is called
		_pRenderTarget->Resize(D2D1::SizeU(width, height));
	}
}

void Direct2DApp::DiscardDeviceResources()
{
	SafeRelease(&_pRenderTarget);
	SafeRelease(&_pLightSlateGrayBrush);
	SafeRelease(&_pCornflowerBlueBrush);
	SafeRelease(&_pBlackBrush);
	SafeRelease(&_pLGBrush);
	SafeRelease(&_pDWriteFactory);
	SafeRelease(&_pTextFormat);
}

LRESULT CALLBACK Direct2DApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	if (message == WM_CREATE)
	{
		LPCREATESTRUCT pcs = (LPCREATESTRUCT)lParam;
		Direct2DApp *pApp = (Direct2DApp*)pcs->lpCreateParams;
		
		::SetWindowLongPtr(
			hWnd,
			GWLP_USERDATA,
			PtrToUlong(pApp));

		result = 1;
	}
	else
	{
		Direct2DApp *pApp = reinterpret_cast<Direct2DApp*>(static_cast<LONG_PTR>
			(::GetWindowLongPtr(hWnd, GWLP_USERDATA)));

		bool wasHandled = false;
		if (pApp)
		{
			switch (message)
			{
			case WM_SIZE:
			{
				auto width = LOWORD(lParam);
				auto height = HIWORD(lParam);
				pApp->OnResize(width, height);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DISPLAYCHANGE:
			{
				InvalidateRect(hWnd, nullptr, FALSE);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_PAINT:
			{
				pApp->OnRender();
				ValidateRect(hWnd, nullptr);
			}
			result = 0;
			wasHandled = true;
			break;

			case WM_DESTROY:
			{
				PostQuitMessage(0);
			}
			result = 0;
			wasHandled = true;
			break;
			}
		}

		if (!wasHandled)
		{
			result = DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return result;
}
