#pragma once
#ifndef _DRAW_H_
#define _DRAW_H_

#include <Windows.h>
#include <string>
#include "struct.h"
#include "define.h"

using namespace std;

class Draw {
public:
	void load_hdc(HDC hdc,int size);
	void delete_hdc();
	void draw_thing(HDC hdc,int x,int y,Thing thing);
private:

	wstring get_thing_name(Thing thing);

	HDC hdcMem[THING_KIND];
	int size;
};

#endif