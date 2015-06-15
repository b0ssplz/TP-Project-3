// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "draw2.h"
#include <vector>
#include <cstdio>
#include <cmath>
#define MAX_LOADSTRING 100
#define TMR_1 1
#define TMR_2 2

#define startx 100
#define starty 400

int armx1 = 500;		int armx2 = 800;
int army1 = 200;		int army2 = 200;

int b0ss = 1;
int steering = 1;
//kulka:
#define R 100
int kulax1 = 550;
int kulay1 = 300;

int kulax2 = 700;
int kulay2 = 300;

//floor:
#define floorx1 0
#define floory1 400
#define floorx2 1000
#define floory2 400

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

INT time;

int choose_arm;
// buttons
HWND hwndButton;

// sent data
int col = 0;
std::vector<Point> data;
RECT drawArea1 = { 0, 0, 150, 200 };
RECT drawArea2 = { 50, 400, 650, 422 };
RECT drawAreaARM1 = { startx, starty, 500, 800 };
RECT drawAreaARM2 = { armx2, army2, armx1, army1 };

#define M_PI        3.14159265358979323846


WORD KulkaX = 100, KulkaY = 100;
SHORT SpeedX = 2, SpeedY = 2;
HWND g_hwnd;



// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);


void MyOnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));


	graphics.DrawLine(&pen, startx, starty, armx1, army1);
	graphics.DrawLine(&pen, armx1, army1, armx2, army2);


	graphics.DrawLine(&pen, floorx1, floory1, floorx2, floory2);


	SolidBrush blackBrush(Color(255, 0, 0, 0));
	graphics.FillEllipse(&blackBrush, kulax1, kulay1, R, R);

	if (time > 0)
	{
		kulay1 = kulay1 + 2;
	}
}


void draw_ARM(HDC hdc)
{

	Graphics graphics(hdc);
	Pen pen(Color(255, 0, 0, 255));

	graphics.DrawLine(&pen, floorx1, floory1, floorx2, floory2);



	if (choose_arm == 1)
	{
		armx1 += steering*((M_PI)* cos(b0ss / 180.0 * M_PI));
		army1 += steering*((M_PI)* sin(b0ss / 180.0 * M_PI));
		graphics.DrawLine(&pen, startx, starty, armx1, army1);


		armx2 += steering*((M_PI)* cos(b0ss / 180.0 * M_PI));
		army2 += steering*((M_PI)* sin(b0ss / 180.0 * M_PI));
		graphics.DrawLine(&pen, armx1, army1, armx2, army2);
	}
	if (choose_arm == 2)
	{

		graphics.DrawLine(&pen, startx, starty, armx1, army1);

		armx2 += steering*((M_PI)* cos(8 * b0ss / 180.0 * M_PI));
		army2 += steering*((M_PI)* sin(8 * b0ss / 180.0 * M_PI));
		graphics.DrawLine(&pen, armx1, army1, armx2, army2);
	}



	if (armx2 > kulax1 && armx2 <kulax1 + R && army2>kulay1  && army2 < kulay1 + R)
	{
		kulax1 = armx2 - R / 2;
		kulay1 = army2 - R / 2;
	}
	if (armx2 > kulax2 && armx2 <kulax2 + R && army2>kulay2  && army2 < kulay2 + R)
	{
		kulax2 = armx2 - R / 2;
		kulay2 = army2 - R / 2;
	}
	if (time == 25 && kulay1 - R<400 && kulay1 - R>=kulay2-R)
		kulay1 = kulay1 + 2;

	if (time == 25 && kulay2 - R<400 )
		kulay1 = kulay1 + 2;
	SolidBrush blackBrush(Color(255, 0, 0, 0));
	graphics.FillEllipse(&blackBrush, kulax1, kulay1, R, R);
	graphics.FillEllipse(&blackBrush, kulax2, kulay2, R, R);


}


void repaintWindow(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{
	if (drawArea == NULL)
		InvalidateRect(hWnd, NULL, TRUE); // repaint all
	else
		InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	MyOnPaint(hdc);
	EndPaint(hWnd, &ps);
}

void repaintARM(HWND hWnd, HDC &hdc, PAINTSTRUCT &ps, RECT *drawArea)
{

	InvalidateRect(hWnd, drawArea, TRUE); //repaint drawArea
	hdc = BeginPaint(hWnd, &ps);
	draw_ARM(hdc);
	EndPaint(hWnd, &ps);
}







// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	time = 0;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);




	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

	// main window
	hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	// create button and store the handle                                                       

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("Draw"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		0, 60,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON1,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need

	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("DrawAll"),                  // the caption of the button
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,  // the styles
		0, 0,                                  // the left and top co-ordinates
		80, 50,                              // width and height
		hWnd,                                 // parent window handle
		(HMENU)ID_BUTTON2,                   // the ID of your button
		hInstance,                            // the instance of your application
		NULL);                               // extra bits you dont really need
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("ARM 1"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 10,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON3,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("ARM 2"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 60,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON4,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),                      // The class name required is button
		TEXT("DROP"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		900, 110,
		80, 50,
		hWnd,
		(HMENU)ID_BUTTON5,
		hInstance,
		NULL);
	// create button and store the handle                                                       

	hwndButton = CreateWindow(TEXT("button"), TEXT("Timer ON"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		90, 0, 100, 30, hWnd, (HMENU)ID_RBUTTON1, GetModuleHandle(NULL), NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("Timer OFF"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		90, 40, 100, 30, hWnd, (HMENU)ID_RBUTTON2, GetModuleHandle(NULL), NULL);


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
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	// tu podajesz jaki obszar ma sie odswiezac
	RECT rect = { 500, 800, 800, 500 };


	switch (message)
	{

	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_LEFT:
		{
			steering = -1;
			choose_arm = 1;

			repaintARM(hWnd, hdc, ps, NULL);

			b0ss++;
			break;
		}
		case VK_RIGHT:
		{
			steering = +1;
			choose_arm = 1;

			repaintARM(hWnd, hdc, ps, NULL);

			b0ss++;
			break;
		}
		case VK_UP:
		{
			steering = -1;
			choose_arm = 2;

			repaintARM(hWnd, hdc, ps, NULL);

			b0ss++;

			break;
		}
		case VK_DOWN:
		{
			steering = 1;
			choose_arm = 2;

			repaintARM(hWnd, hdc, ps, NULL);

			b0ss++;

			break;
		}
		case VK_F1:
		{
			if (kulay1 + R >= floory1)
			{
				KillTimer(hWnd, TMR_1);
				break;

			}

			SetTimer(hWnd, TMR_1, 25, 0);


			break;
		}
			break;
		}


	}
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON1:
			col++;
			if (col > 10)
				col = 0;
			repaintWindow(hWnd, hdc, ps, &drawArea1);
			break;
		case ID_BUTTON2:
			repaintWindow(hWnd, hdc, ps, NULL);
			break;
		case ID_BUTTON3:

			repaintARM(hWnd, hdc, ps, NULL);

			b0ss++;

			break;
		case ID_BUTTON4:

			repaintARM(hWnd, hdc, ps, NULL);

			b0ss++;

			break;
		case ID_BUTTON5:
			SetTimer(hWnd, TMR_1, 25, 0);


			b0ss++;

			break;
		case ID_RBUTTON1:
			SetTimer(hWnd, TMR_1, 25, 0);
			break;
		case ID_RBUTTON2:
			KillTimer(hWnd, TMR_1);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here (not depend on timer, buttons)
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		switch (wParam)
		{
		case TMR_1:
			if (kulay1 + R >= floory1)
				KillTimer(hWnd, TMR_1);

			//force window to repaint
			repaintWindow(hWnd, hdc, ps, NULL);

			time++;

			break;
		}

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
