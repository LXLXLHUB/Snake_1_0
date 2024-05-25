#include "lock.h"
Lock::Lock() {
	lock = true;
}

bool Lock::turn_on() {
	if (lock==true) {
		return false;
	}
	lock = true;
	return true;
}

bool Lock::turn_off() {
	if (lock == false) {
		return false;
	}
	lock = false;
	return true;
}
