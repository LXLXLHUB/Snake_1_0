#pragma once
#ifndef _GAME_H_
#define _GAME_H_

#include <Windows.h>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
#include "define.h"
#include "struct.h"

using namespace std;

class Game {
public:
	Game();
	~Game();
	Thing get_thing(int mx, int my, int x, int y);
	void set_thing(int mx, int my, int x, int y,Thing thing);
	bool load_check(int i);
	int get_check();
	int get_max_check();
	void load_game();
	Snake get_head();
	Snake get_tail();
	bool judge_tail(int m_x, int m_y, int x, int y);
	void insert_head(int m_x, int m_y, int x, int y);
	void insert_tail (int m_x, int m_y, int x, int y);
	void delete_tail();
	int get_map_x();
	int get_map_y();
	int get_max_x();
	int get_max_y();
	void set_speed();
	int get_speed();
	void set_direction(int x, int y);
	int get_direction_x();
	int get_direction_y();
	bool win();
	void set_max_x (int max_x);
	void set_max_y (int max_y);
	void create_default_check ();
	int get_snake_length ();
private:
	void save_cache();
	void load_cache(int x, int y);
	Thing map[MAP_MAX][MAP_MAX];
	int map_x;
	int map_y;
	int check;
	int max_check;
	int max_size;
	vector<Snake> v;
	int speed;
	int speed_change;
	int max_x;
	int max_y;
	int direction_x;
	int direction_y;
};

#endif 