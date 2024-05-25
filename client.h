#pragma once
#ifndef _CLIENT_H_
#define _CLIENT_H_

#include "window.h"
#include "game.h"
#include "draw.h"
#include "lock.h"

class Client {
public:
	~Client();
	bool load_window_message(HINSTANCE hInsatnce);
	bool set_wndclassex(WNDCLASSEX& wndclass, WNDPROC windowProc);
	HINSTANCE get_hinstance();
	void test();
	void load_hdc(HDC hdc);
	void delete_hdc();
	void draw_map(HDC hdc);
	void window_change(HWND hwnd);
	bool load_check(int i) { return (*game).load_check(i); }
	void move(HWND hwnd, HDC hdc, int x, int y);
	int get_speed();
	int get_direction_x();
	int get_direction_y();
	Snake add_food();
	void load_game();
	int get_max_x ();
	int get_max_y ();
	void set_max_x (int max_x);
	void set_max_y (int max_y);
	void create_default_check ();
	void draw_make_thing (HDC hdc,HWND hwnd,POINT p, Thing thing);
private:

	void snake_move(HWND hwnd, HDC hdc, int x, int y);

	Window* window = new Window();
	Game* game = new Game();
	Draw* draw = new Draw();
	Lock* lock = new Lock();
	int show_x;
	int show_y;
};

#endif