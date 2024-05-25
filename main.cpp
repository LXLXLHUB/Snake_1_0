#include <tchar.h>
#include "data.h"
#include "resource.h"

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK selectProc (HWND select, UINT uMsg, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK makeProc (HWND select, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrecInstance, LPSTR lpCmdLine, int iCmdShow) {
	srand((unsigned)time(NULL));
	(*client).load_game();
	if (!(*client).load_check(1)) {
		return 0;
	}
	(*client).add_food();
	if (!(*client).load_window_message(hInstance)) {
		delete client;
		return 0;
	}

	HWND hwnd;
	WNDCLASSEX wndclass;
	MSG msg;


	(*client).set_wndclassex(wndclass, windowProc);

	RegisterClassEx(&wndclass);
	hwnd = CreateWindowEx(0, L"MyWindow", L"贪吃蛇1.0", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
		NULL, NULL, (*client).get_hinstance(), NULL);
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	delete client;
	return 0;
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	HDC hdc;
	PAINTSTRUCT ps;
	HMENU menu;
	switch (uMsg) {
	case WM_CREATE:{
		menu = LoadMenu ((*client).get_hinstance (), MAKEINTRESOURCE (ID_MENU1));
		SetMenu (hwnd, menu);
		SetTimer (hwnd, TIMER_ID, (*client).get_speed (), NULL);
		return 0; 
	}
	case WM_PAINT:{
		hdc = BeginPaint (hwnd, &ps);
		(*client).window_change (hwnd);
		(*client).delete_hdc ();
		(*client).load_hdc (hdc);
		(*client).draw_map (hdc);
		EndPaint (hwnd, &ps);
		return 0; 
	}
	case WM_KEYDOWN:{
		hdc = GetDC (hwnd);
		switch (wParam) {
		case 'w':
		case 'W':
		case VK_UP:
			(*client).move (hwnd, hdc, -1, 0);
			break;
		case 's':
		case 'S':
		case VK_DOWN:
			(*client).move (hwnd, hdc, 1, 0);
			break;
		case 'd':
		case 'D':
		case VK_RIGHT:
			(*client).move (hwnd, hdc, 0, 1);
			break;
		case 'a':
		case 'A':
		case VK_LEFT:
			(*client).move (hwnd, hdc, 0, -1);
			break;
		}
		ReleaseDC (hwnd, hdc);
		return 0;
	}
	case WM_COMMAND: {
		KillTimer (hwnd, TIMER_ID);
		(*client).delete_hdc ();
		hdc = GetDC (hwnd);
		switch (LOWORD (wParam)) {
		case IM_MAKE: {
			DialogBox ((*client).get_hinstance (), MAKEINTRESOURCE (DIALOG_MAKE_SELECT), hwnd, selectProc);
			break;
		}
		}
		(*client).delete_hdc ();
		(*client).load_hdc (hdc);
		(*client).window_change (hwnd);
		(*client).load_game ();
		(*client).load_check (1);
		(*client).add_food ();
		InvalidateRect (hwnd, NULL, TRUE);
		ReleaseDC (hwnd, hdc);
		SetTimer (hwnd, TIMER_ID, (*client).get_speed (), NULL);
		return 0;
	}
	case WM_TIMER: {
		hdc = GetDC (hwnd);
		switch (wParam) {
		case TIMER_ID:
			(*client).move (hwnd, hdc, (*client).get_direction_x (), (*client).get_direction_y ());
			break;
		}
		ReleaseDC (hwnd, hdc);
		return 0;
	}
	case WM_DESTROY: {
		KillTimer (hwnd, TIMER_ID);
		(*client).delete_hdc ();
		PostQuitMessage (0);
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

INT_PTR CALLBACK selectProc (HWND select, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	int width_size=0, height_size=0;
	wchar_t wc1[100],wc2[100];
	static HWND width,height;
	HWND state,button;
	switch (uMsg) {
	case WM_INITDIALOG: {
		state = CreateWindowEx (0, L"Static", L"宽度：", WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 10, 60, 20,
			select, (HMENU)(0), (*client).get_hinstance (), NULL);
		width = CreateWindowEx (0, L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			70, 10, 100, 20,
			select, (HMENU)(0), (*client).get_hinstance (), NULL);
		state = CreateWindowEx (0, L"Static", L"高度：", WS_CHILD | WS_VISIBLE | WS_BORDER,
			10, 40, 60, 20,
			select, (HMENU)(0), (*client).get_hinstance (), NULL);
		height = CreateWindowEx (0, L"Edit", NULL, WS_CHILD | WS_VISIBLE | WS_BORDER,
			70, 40, 100, 20,
			select, (HMENU)(0), (*client).get_hinstance (), NULL);
		button = CreateWindowEx (0, L"Button", L"创建地图", WS_CHILD | WS_VISIBLE | WS_BORDER,
			50, 70, 100, 20,
			select, (HMENU)(1), (*client).get_hinstance (), NULL);
		state = CreateWindowEx (0, L"Static", L"注意：实际的地图尺寸是填的数据的10倍（即1为10格），同时填写的数字过大可能会导致电脑速度下降，性能消耗过大！！！", WS_CHILD | WS_VISIBLE | WS_BORDER,
			180,10, 220, 80,
			select, (HMENU)(0), (*client).get_hinstance (), NULL);
		return true;
	}
	case WM_COMMAND: {
		switch (LOWORD  (wParam)) {
		case 1: {
			GetWindowText (width, wc1,100);
			GetWindowText (height, wc2, 100);
			if (wcslen (wc1) <= 0 || wcslen (wc2) <= 0) {
				MessageBox (select, L"请先填写数据！", L"注意", MB_OK);
				return true;
			}
			width_size = _ttoi (wc1);
			height_size = _ttoi (wc2);
			if (width_size > 0 && height_size > 0) {
				if (width_size >= 10 || height_size >= 10) {
					if (MessageBox (select, L"地图尺寸过大，如果继续请点击确定。", L"注意", MB_YESNO) == IDNO) {
						EndDialog (select, 0);
						return true;
					}
				}
				(*client).set_max_x (height_size);
				(*client).set_max_y (width_size);
				(*client).create_default_check ();
				EndDialog (select, 0);
				DialogBox ((*client).get_hinstance (), MAKEINTRESOURCE (DIALOG_MAKE_SELECT), select, makeProc);
			} else {
				MessageBox (select, L"地图尺寸填写异常！", L"注意", MB_OK);
			}
			break;
		}
		}
		return true;
	}
	case WM_CLOSE: {
		EndDialog (select, 0);
		return true;
	}
	}
	return false;
}


INT_PTR CALLBACK makeProc (HWND make, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	static Thing thing=SNAKE_NULL;
	HMENU hmenu;
	HDC hdc;
	PAINTSTRUCT ps;
	POINT p;
	switch (uMsg) {
	case WM_INITDIALOG: {
		hmenu=LoadMenu ((*client).get_hinstance (), MAKEINTRESOURCE(ID_MENU2));
		SetMenu (make, hmenu);
		return true;
	}
	case WM_PAINT: {
		hdc = BeginPaint (make, &ps);
		(*client).window_change (make);
		(*client).load_hdc (hdc);
		(*client).draw_map (hdc);
		EndPaint (make, &ps);
		return true;
	}
	case WM_COMMAND: {
		thing = LOWORD (wParam);
		return true;
	}
	case WM_LBUTTONDOWN: {
		hdc = GetDC (make);
		GetCursorPos (&p);
		ScreenToClient (make, &p);
		(*client).draw_make_thing (hdc,make,p,thing);
		ReleaseDC (make, hdc);
		return true;
	}
	case WM_CLOSE: {
		EndDialog (make, 0);
		return true;
	}
	}
	return false;
}