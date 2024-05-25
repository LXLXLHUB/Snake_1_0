#include "draw.h"

wstring Draw::get_thing_name(Thing thing) {
	wstring ws;
	switch (thing) {
	case SNAKE_NULL:
		ws=L"null";
		break;
	case SNAKE_WALL:
		ws = L"wall";
		break;
	case SNAKE_BODY:
		ws = L"body";
		break;
	case SNAKE_HEAD:
		ws = L"head";
		break;
	case SNAKE_FOOD:
		ws = L"food";
		break;
	default:
		ws = L"null";
	}
	return ws;
}

void Draw::load_hdc(HDC hdc,int size) {
	(*this).size = size;
	for (int i = 0; i < THING_KIND; i++) {
		hdcMem[i] = CreateCompatibleDC(hdc);
		HBITMAP hbitmap;
		hbitmap = (HBITMAP)LoadImage(NULL,((wstring)L"photo/" + get_thing_name(i) + (wstring)L".bmp").c_str(), IMAGE_BITMAP, size, size, LR_LOADFROMFILE);
		SelectObject(hdcMem[i], hbitmap);
		DeleteObject(hbitmap);
	}
}

void Draw::delete_hdc() {
	for (int i = 0; i < THING_KIND; i++) {
		DeleteDC(hdcMem[i]);
	}
}

void Draw::draw_thing(HDC hdc, int x, int y, Thing thing) {
	BitBlt(hdc, x, y, size,size, hdcMem[thing], 0,0, SRCCOPY);
}
