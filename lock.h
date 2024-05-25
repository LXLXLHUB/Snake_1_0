#pragma once
#ifndef _LOCK_H_
#define _LOCK_H_

class Lock {
public:
	Lock();
	bool turn_on();
	bool turn_off();
private:
	bool lock;
};

#endif