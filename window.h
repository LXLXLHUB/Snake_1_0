#pragma once
#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <Windows.h>
#include <fstream>
#include <string>
#include "define.h"
using namespace std;

class Window {
public:
	bool load_window_message(HINSTANCE hInstance);
	bool set_wndclass(WNDCLASSEX& wndclass, WNDPROC windowProc);
	void window_change(HWND hwnd);
	HINSTANCE get_hinstance();
	int get_thing_size();
	int get_width_size();
	int get_height_size();
private:

	int width_size;
	int height_size;
	int thing_size;

	wstring class_name;
	wstring app_name;
	int window_background[3];
	HINSTANCE hinstance;
};

#endif