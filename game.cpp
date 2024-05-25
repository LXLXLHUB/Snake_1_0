#include "game.h"

Game::Game() {
	check = 1;
}

Game::~Game() {
	v.clear();
}

Thing Game::get_thing(int mx, int my, int x, int y) {
	if (mx != map_x || my != map_y) {
		save_cache();
		load_cache(mx, my);
	}
	return map[x][y];
}

void Game::save_cache() {
	wstring ws, ws0;
	wstringstream wss;
	wss << map_x;
	wss >> ws0;
	wss.clear();
	ws = L"cache/cache" + ws0;
	wss << map_y;
	wss >> ws0;
	ws += L"_" + ws0 + L".map";
	wss.clear();
	ofstream out;
	out.open(ws.c_str());
	for (int i = 0; i < MAP_MAX; i++) {
		for (int j = 0; j < MAP_MAX; j++) {
			out << map[i][j]<< " ";
		}
		out << endl;
	}
	out.close();
}

void Game::load_cache(int x, int y) {
	map_x = x;
	map_y = y;
	wstring ws,ws0;
	wstringstream wss;
	wss << x;
	wss >> ws0;
	wss.clear();
	ws = L"cache/cache" + ws0;
	wss << y;
	wss >> ws0;
	ws += L"_" + ws0 + L".map";
	wss.clear();
	ifstream in;
	in.open(ws.c_str());
	for (int i = 0; i < MAP_MAX; i++) {
		for (int j = 0; j < MAP_MAX; j++) {
			in >> map[i][j];
		}
	}
	in.close();
}

bool Game::load_check(int check) {
	if (check >= max_check) {
		return false;
	}
	(*this).check = check;
	map_x = -1;
	map_y = -1;
	Snake snake;
	int num;
	ifstream in;
	ofstream out;
	wstring ws,ws0;
	wstringstream wss;
	wss << check;
	wss >> ws0;
	wss.clear();
	ws = L"check/check" + ws0 + L".map";
	in.open(ws.c_str());
	in >> max_x >> max_y;
	in >> direction_x >> direction_y;
	in >> speed >> speed_change >> max_size;
	in >> num;
	v.clear();
	for (int i = 0; i < num; i++) {
		in >> snake.map_x >> snake.map_y;
		in >> snake.x >> snake.y;
		v.push_back(snake);
	}
	for (map_x= 0; map_x < max_x; map_x++) {
		for (map_y =0; map_y < max_y; map_y++) {
			for (int n = 0; n < MAP_MAX; n++) {
				for (int m = 0; m < MAP_MAX; m++) {
					in >> map[n][m];
				}
			}
			wss << map_x;
			wss >> ws0;
			wss.clear();
			ws = L"cache/cache" + ws0;
			wss << map_y;
			wss >> ws0;
			wss.clear();
			ws += L"_" + ws0 + L".map";
			out.open(ws.c_str());
			for (int n = 0; n < MAP_MAX; n++) {
				for (int m = 0; m < MAP_MAX; m++) {
					out <<  map[n][m] << " ";
				}
				out << endl;
			}
			out.close();
		}
	}
	map_x--;
	map_y--;
	in.close();
	wchar_t c[20];
	get_thing(v[v.size() - 1].map_x, v[v.size() - 1].map_y, v[v.size() - 1].x, v[v.size() - 1].y);
	return true;
}

int Game::get_check() {
	return check;
}

int Game::get_max_check() {
	return max_check;
}

void Game::load_game() {
	ifstream in;
	in.open("lib/game.dat");
	in >> max_check;
	in.close();
}

Snake Game::get_head() {
	return v[v.size() - 1];
}

Snake Game::get_tail() {
	return v[0];
}

void Game::insert_head(int m_x, int m_y, int x, int y) {
	Snake s;
	s.map_x = m_x;
	s.map_y = m_y;
	s.x = x;
	s.y = y;
	v.push_back(s);
}

void Game::insert_tail (int m_x, int m_y, int x, int y) {
	Snake s;
	s.map_x = m_x;
	s.map_y = m_y;
	s.x = x;
	s.y = y;
	v.insert (v.begin (), s);
}

void Game::delete_tail() {
	v.erase(v.begin());
}

bool Game::judge_tail(int m_x, int m_y, int x, int y) {
	if (v[0].map_x == m_x && v[0].map_y == m_y) {
		if (v[0].x == x&& v[0].y == y) {
			return true;
		}
	}
	return false;
}

int Game::get_map_x() {
	return map_x;
}

int Game::get_map_y() {
	return map_y;
}

void Game::set_thing(int mx, int my, int x, int y, Thing thing) {
	if (mx != map_x || my != map_y) {
		save_cache();
		load_cache(mx, my);
	}
	map[x][y]=thing;
}

int Game::get_max_x() {
	return max_x;
}

int Game::get_max_y() {
	return max_y;
}

void Game::set_speed() {
	speed -= speed_change;
}

int Game::get_speed() {
	return speed;
}

void Game::set_direction(int x, int y) {
	direction_x = x;
	direction_y = y;
}

int Game::get_direction_x() {
	return direction_x;
}

int Game::get_direction_y() {
	return direction_y;
}

bool Game::win() {
	return v.size() >= max_size;
}

void Game::set_max_x (int max_x) {
	(*this).max_x = max_x;
}

void Game::set_max_y (int max_y) {
	(*this).max_y = max_y;
}

void Game::create_default_check () {
	ofstream fout;
	ifstream fin;
	string s;
	for (int i = 0; i < max_x; i++) {
		for (int j = 0; j < max_y; j++) {
			s = (string)"cache/cache" + to_string (i) + (string)"_" + to_string (j) + (string)".map";
			fout.open (s);
			for (int m = 0; m < MAP_MAX; m++) {
				for (int n = 0; n < MAP_MAX; n++) {
					fout << 0 << " ";
				}
				fout << endl;
			}
			fout.close ();
		}
	}
	map_x = 0;
	map_y = 0;
	fin.open ("cache/cache0_0.map");
	for (int i = 0; i < MAP_MAX; i++) {
		for (int j = 0; j < MAP_MAX; j++) {
			fin >> map[i][j];
		}
	}
	v.clear ();
	fin.close ();
}

int Game::get_snake_length () {
	return v.size ();
}