#pragma once
class spinlock {
	int volatile *lock;
public:
	spinlock();
	void aquire();
	void release();
	bool check();
};
