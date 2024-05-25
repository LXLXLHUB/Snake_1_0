#include "window.h"
bool Window::load_window_message (HINSTANCE hInstance) {
	wifstream win;
	wstring ws;
	win.open (L"lib/window.dat");
	try {
		win >> ws;
		if (ws != L"#lpszClassName") {
			throw (wstring)L"读取窗口类名信息失败！";
		}
		win >> class_name;
		win >> ws;
		if (ws != L"#lpszAppName") {
			throw (wstring)L"读取窗口标题信息失败！";
		}
		win >> class_name;
		win >> ws;
		if (ws != L"#hbrBackground") {
			throw (wstring)L"读取窗口背景颜色信息失败！";
		}
	} catch (wstring s) {
		win.close ();
		MessageBox (NULL,s.c_str(), L"错误", MB_OK);
		return false;
	}
	
	win.close ();
	hinstance = hInstance;
	return true;
}

bool Window::set_wndclass (WNDCLASSEX& wndclass, WNDPROC windowProc) {
	wndclass.cbSize = sizeof (WNDCLASSEX);
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.cbWndExtra = 0;
	wndclass.cbClsExtra = 0;
	wndclass.hIcon = LoadIcon (hinstance, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor (hinstance, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject (WHITE_BRUSH);
	wndclass.hInstance = hinstance;
	wndclass.lpszClassName = L"MyWindow";
	wndclass.lpszMenuName = NULL;
	wndclass.lpfnWndProc = windowProc;
	wndclass.hIconSm = NULL;
	return true;
}

HINSTANCE Window::get_hinstance () {
	return hinstance;
}

void Window::window_change (HWND hwnd) {
	RECT rect;
	GetClientRect (hwnd, &rect);
	if ((rect.bottom - rect.top) < (rect.right - rect.left)) {
		thing_size = (rect.bottom - rect.top) / (MAP_MAX + 2);
	} else {
		thing_size = (rect.right - rect.left) / (MAP_MAX + 2);
	}
	height_size = ((rect.bottom - rect.top) - thing_size * MAP_MAX) / 2;
	width_size = ((rect.right - rect.left) - thing_size * MAP_MAX) / 2;
}

int Window::get_thing_size () {
	return thing_size;
}

int Window::get_width_size () {
	return width_size;
}

int Window::get_height_size () {
	return height_size;
}
