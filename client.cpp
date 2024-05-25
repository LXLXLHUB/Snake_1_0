#include "client.h"
Client::~Client() {
	delete window;
	delete game;
	delete draw;
	delete lock;
}

bool Client::load_window_message(HINSTANCE hInstance) {
	return (*window).load_window_message(hInstance);
}

bool Client::set_wndclassex(WNDCLASSEX& wndclass, WNDPROC windowProc) {
	return (*window).set_wndclass(wndclass, windowProc);
}

HINSTANCE Client::get_hinstance() {
	return (*window).get_hinstance();
}

void Client::load_hdc(HDC hdc) { 
	return (*draw).load_hdc(hdc, (*window).get_thing_size());
}

void Client::delete_hdc() { 
	draw->delete_hdc(); 
}

void Client::window_change(HWND hwnd) {
	return (*window).window_change(hwnd);
}

void Client::draw_map(HDC hdc) {
	show_x = (*game).get_map_x();
	show_y = (*game).get_map_y();
	int width = (*window).get_width_size();
	int height = (*window).get_height_size();
	int size = (*window).get_thing_size();
	for (int i = 0; i < MAP_MAX; i++) {
		for (int j = 0; j < MAP_MAX; j++) {
			(*draw).draw_thing(hdc, width + j * size, height + i * size,(*game).get_thing(show_x,show_y,i,j));
		}
	}
}

void Client::snake_move(HWND hwnd,HDC hdc,int x, int y) {

	if ((*game).get_direction_x() + x == 0 && (*game).get_direction_y() + y == 0) {
		return;
	}

	int hx0 = (*game).get_head().x;
	int hy0 = (*game).get_head().y;
	int mx0 = (*game).get_head().map_x;
	int my0 = (*game).get_head().map_y;

	int hx1;
	int hy1;
	int mx1;
	int my1;
	
	if (hx0 + x < 0) {
		mx1 = mx0-1;
		hx1 = 9;
	}
	else if (hx0 + x > 9) {
		mx1 = mx0 + 1;
		hx1 = 0;
	}
	else {
		mx1 = mx0;
		hx1 = hx0+x;
	}
	if (mx1 < 0 ) {
		mx1 = (*game).get_max_x() - 1;
	}
	else if (mx1 >= (*game).get_max_x()) {
		mx1 = 0;
	}

	if (hy0 + y < 0) {
		my1 = my0 - 1;
		hy1 = 9;
	}
	else if (hy0 + y > 9) {
		my1 = my0 + 1;
		hy1 = 0;
	}
	else {
		my1 = my0;
		hy1 = hy0+y;
	}
	
	if (my1 < 0) {
		my1 = (*game).get_max_y() - 1;
	}
	else if (my1 >= (*game).get_max_y()) {
		my1 = 0;
	}
	(*game).set_direction(x, y);
	int width = (*window).get_width_size();
	int height = (*window).get_height_size();
	int size = (*window).get_thing_size();
	switch ((*game).get_thing(mx1, my1, hx1, hy1)) {
	case SNAKE_WALL:
		KillTimer(hwnd, TIMER_ID);
		MessageBox(hwnd, L"游戏失败", L"注意", MB_OK);
		(*game).load_check((*game).get_check());
		(*game).get_thing((*game).get_head().map_x, (*game).get_head().map_y, (*game).get_head().x, (*game).get_head().y);
		add_food();
		draw_map(hdc);
		SetTimer(hwnd, TIMER_ID, (*game).get_speed(), NULL);
		break;
	case SNAKE_BODY:
		if (!(*game).judge_tail(mx1, my1, hx1, hy1)) {
			KillTimer(hwnd, TIMER_ID);
			MessageBox(hwnd, L"游戏失败", L"注意", MB_OK);
			(*game).load_check((*game).get_check());
			add_food();
			(*game).get_thing((*game).get_head().map_x, (*game).get_head().map_y, (*game).get_head().x, (*game).get_head().y);
			draw_map(hdc);
			SetTimer(hwnd, TIMER_ID, (*game).get_speed(), NULL);
			break;
		}
	case SNAKE_NULL:

		(*game).set_thing(mx0, my0, hx0, hy0, SNAKE_BODY);
		(*game).set_thing(mx1, my1, hx1, hy1, SNAKE_HEAD);
		(*game).set_thing((*game).get_tail().map_x, (*game).get_tail().map_y, (*game).get_tail().x, (*game).get_tail().y, SNAKE_NULL);
		(*game).insert_head(mx1, my1,hx1, hy1);
		if (mx1 != show_x || my1 != show_y) {
			(*game).get_thing(mx1, my1, hx1, hy1);
			(*game).delete_tail();
			draw_map(hdc);
			return;
		}

		

		(*draw).draw_thing(hdc, width + size * hy0, height + size * hx0, SNAKE_BODY);
		if ((*game).get_tail().map_x == show_x && (*game).get_tail().map_y == show_y) {
			(*draw).draw_thing(hdc, width + size * (*game).get_tail().y, height + size * (*game).get_tail().x, SNAKE_NULL);
		}
		(*draw).draw_thing(hdc, width + size * hy1, height + size * hx1, SNAKE_HEAD);
		(*game).delete_tail();
		break;
	case SNAKE_FOOD:
		(*game).set_speed();
		(*game).insert_head(mx1, my1, hx1, hy1);
		if ((*game).get_speed() <= 0||(*game).win()) {
			if (!(*game).load_check((*game).get_check() + 1)) {
				MessageBox(hwnd, L"恭喜通过全关！！！", L"祝贺！！！", MB_OK);
				KillTimer(hwnd, TIMER_ID);
				delete_hdc();
				PostQuitMessage(0);
			}
			add_food();
			(*game).get_thing((*game).get_head().map_x, (*game).get_head().map_y, (*game).get_head().x, (*game).get_head().y);
			draw_map(hdc);
			KillTimer(hwnd, TIMER_ID);
			MessageBox(hwnd, L"恭喜通关！！！", L"祝贺！！！", MB_OK);
			SetTimer(hwnd, TIMER_ID, (*game).get_speed(), NULL);
			break;
		}
		KillTimer(hwnd, TIMER_ID);
		SetTimer(hwnd, TIMER_ID, (*game).get_speed(), NULL);
		(*game).set_thing(mx0, my0, hx0, hy0, SNAKE_BODY);
		(*game).set_thing(mx1, my1, hx1, hy1, SNAKE_HEAD);
		Snake s=add_food();
		if (mx1 != show_x || my1 != show_y) {
			(*game).get_thing(mx1, my1, hx1, hy1);
			draw_map(hdc);
			return;
		}
		(*draw).draw_thing(hdc, width + size * hy0, height + size * hx0, SNAKE_BODY);
		(*draw).draw_thing(hdc, width + size * hy1, height + size * hx1, SNAKE_HEAD);
		if (show_x == s.map_x && show_y == s.map_y) {
			(*draw).draw_thing(hdc, (*window).get_width_size() + size * s.y,
				(*window).get_height_size() + size * s.x, SNAKE_FOOD);
		}
		break;
	}
}

void Client::test() {
	wchar_t wc[100];
	wsprintf(wc, L"%d:%d", (*game).get_max_x(), (*game).get_max_y());
	MessageBox(NULL, wc, NULL, MB_OK);
}

void Client::move(HWND hwnd, HDC hdc, int x, int y) {
	if ((*lock).turn_off()) {
		snake_move(hwnd, hdc, x, y);
		(*lock).turn_on();
	}
}

int Client::get_speed() {
	return (*game).get_speed();
}

int Client::get_direction_x() {
	return (*game).get_direction_x();
}

int Client::get_direction_y() {
	return (*game).get_direction_y();
}

Snake Client::add_food() {
	Snake s;
	int mx, my, hx, hy;
	int size = (*window).get_thing_size();
	while (true) {
		mx = rand() % (*game).get_max_x();
		my = rand() % (*game).get_max_y();
		hx = rand() % MAP_MAX;
		hy = rand() % MAP_MAX;
		if ((*game).get_thing(mx, my, hx, hy) == SNAKE_NULL) {
			(*game).set_thing(mx, my, hx, hy, SNAKE_FOOD);
			break;
		}
	}
	s.map_x = mx;
	s.map_y = my;
	s.x = hx;
	s.y = hy;
	return s;
}

void Client::load_game() {
	(*game).load_game();
}

int Client::get_max_x () {
	return (*game).get_max_x ();
}

int Client::get_max_y () {
	return (*game).get_max_y ();
}

void Client::set_max_x (int max_x) {
	return (*game).set_max_x (max_x);
}

void Client::set_max_y (int max_y) {
	return (*game).set_max_y (max_y);
}

void Client::create_default_check () {
	show_x;
	show_y;
	return (*game).create_default_check ();
}

void Client::draw_make_thing (HDC hdc, HWND hwnd,POINT p, Thing thing) {
	int width = (*window).get_width_size ();
	int height = (*window).get_height_size ();
	int size = (*window).get_thing_size ();
	int x = (p.y - height) / size;
	int y = (p.x - width) / size;
	if (thing == SNAKE_HEAD) {
		if ((*game).get_snake_length () == 0) {
			(*game).set_thing (show_x, show_y, x, y, thing);
			(*draw).draw_thing (hdc, width + size * y, height + size * x, thing);
			(*game).insert_head (show_x, show_y, x, y);
			(*draw).draw_thing (hdc, width + size * y, height + size * x, thing);
		} else if((*game).get_snake_length()<0){
			MessageBox (hwnd, L"注意数据异常！", L"警告！", MB_OK);
			EndDialog (hwnd, 0);
			return;
		} else {
			MessageBox (hwnd, L"蛇已经存在头部！", L"警告！", MB_OK);
			return;
		}
	} else if (thing == SNAKE_BODY) {
		if ((*game).get_snake_length () == 0) {
			MessageBox (hwnd, L"请先添加蛇的头部！", L"注意！",MB_OK);
			return;
		} else if ((*game).get_snake_length () < 0) {
			MessageBox (hwnd, L"注意数据异常！", L"警告！", MB_OK);
			EndDialog (hwnd, 0);
			return;
		} else {
			Snake s = (*game).get_tail ();
		}
	}
}
