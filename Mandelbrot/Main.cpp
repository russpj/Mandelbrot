// Mandelbrot.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Main.h"

#include <sstream>
using std::wstringstream;

#include <string>
using std::wstring;

#include "Graphics.h"
#include "Mandelbrot.h"

#define MAX_LOADSTRING 1000

// Global Variables:
struct CalculationState
{
	Complex ulStart = Complex(-2.1, 1.2);
	Complex lrStart = Complex(0.6, -1.2);
	Complex ulCurrent = ulStart;
	Complex lrCurrent = lrStart;
	int levels = 256;
	bool fTrackMouse = false;
	POINT ptMouseDown;
	POINT ptMouseUp;
	HBITMAP displayBitmap = NULL;
	ComplexMapper mapper;
	WCHAR const* szTitle;

	CalculationState(WCHAR const *szWindowTitle)
	{
		szTitle = szWindowTitle;
	}

	void UpdateWindowSize(HWND hWnd, int width, int height)
	{
		mapper = ComplexMapper(ulCurrent, lrCurrent, width, height);
		UpdateWindowTitle(hWnd);
	}

	void UpdateWindowRange(HWND hWnd, Complex ul, Complex lr)
	{
		RECT rect;
		GetClientRect(hWnd, &rect);
		ulCurrent = ul;
		lrCurrent = lr;
		mapper = ComplexMapper(ul, lr, rect.right - rect.left, rect.bottom - rect.top);
		UpdateWindowTitle(hWnd);
	}

	void ResetWindowRange(HWND hWnd)
	{
		UpdateWindowRange(hWnd, ulStart, lrStart);
	}

	void IncreaseLevels(HWND hWnd)
	{
		levels = levels*2;
		UpdateWindowTitle(hWnd);
	}

	void DecreaseLevels(HWND hWnd)
	{
		levels = max(1, levels / 2);
		UpdateWindowTitle(hWnd);
	}

	void StartTracking(LPARAM lParam)
	{
		ptMouseDown.x = GET_X_LPARAM(lParam);
		ptMouseDown.y = GET_Y_LPARAM(lParam);
		fTrackMouse = true;
	}

	void EndTracking(HWND hWnd, LPARAM lParam, int width, int height)
	{
		ptMouseUp.x = GET_X_LPARAM(lParam);
		ptMouseUp.y = GET_Y_LPARAM(lParam);
		fTrackMouse = false;
		int xUL = min(ptMouseDown.x, ptMouseUp.x);
		int yUL = min(ptMouseDown.y, ptMouseUp.y);
		int xLR = max(ptMouseDown.x, ptMouseUp.x);
		int yLR = max(ptMouseDown.y, ptMouseUp.y);
		ulCurrent = mapper.Map(xUL, yUL);
		lrCurrent = mapper.Map(xLR, yLR);
		mapper = ComplexMapper(ulCurrent, lrCurrent, width, height);
		UpdateWindowTitle(hWnd);
	}

	void UpdateWindowTitle(HWND hWnd)
	{
		wstringstream str;
		str << szTitle;
		str << ' ';
		str << levels << " Levels";
		wstring title = str.str();
		SetWindowText(hWnd, title.c_str());
	}
};

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

CalculationState state(szTitle);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MANDELBROT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MANDELBROT));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MANDELBROT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MANDELBROT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_SIZE:
		{
			int width = LOWORD(lParam);
			int height = HIWORD(lParam);
			state.UpdateWindowSize(hWnd, width, height);
		}
		break;
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_CHAR:
		switch (wParam)
		{
			case '0':
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				state.ResetWindowRange(hWnd);

				InvalidateRect(hWnd, &rect, false);
			}
			break;
		case '+':
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				state.IncreaseLevels(hWnd);
				InvalidateRect(hWnd, &rect, false);
			}
			break;
		case '-':
			{
				RECT rect;
				GetClientRect(hWnd, &rect);
				state.DecreaseLevels(hWnd);
				InvalidateRect(hWnd, &rect, false);
			}
			break;
		}
		break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			const int dxSlice = 1;
            HDC hdc = BeginPaint(hWnd, &ps);
			ColorMapper comap(RGB(50, 10, 0), RGB(255, 215, 0), RGB(0, 0, 0), state.levels);
			Calculator calc(comap, state.mapper);

			int xMax = min(ps.rcPaint.left + dxSlice, ps.rcPaint.right);
			for (int x = ps.rcPaint.left; x < xMax; x++)
			{
				for (int y = ps.rcPaint.top; y < ps.rcPaint.bottom; y++)
				{
					auto co = calc.MapPoint(x, y);
					SetPixel(hdc, x, y, co);
				}
			}
			RECT unpaintedRect = ps.rcPaint;

            EndPaint(hWnd, &ps);

			if (xMax < unpaintedRect.right)
			{
				unpaintedRect.left = xMax;
				InvalidateRect(hWnd, &unpaintedRect, false);
			}
        }
        break;
	case WM_LBUTTONDOWN:
		state.StartTracking(lParam);
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_LBUTTONUP:
		{
			RECT rect;
			GetClientRect(hWnd, &rect);
			state.EndTracking(hWnd, lParam, rect.right-rect.left, rect.bottom-rect.top);
			InvalidateRect(hWnd, NULL, true);
		}
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	case WM_MOUSEMOVE:
		{
			POINT pt;
			pt.x = GET_X_LPARAM(lParam);
			pt.y = GET_Y_LPARAM(lParam);
		}
  		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
  case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
