#ifndef __SYS_SYNC_H
#define __SYS_SYNC_H
class spinlock {
	int volatile *lock;
	int l;

  public:
	spinlock();
	void aquire();
	void release();
	bool check();
};
class semaphore {
	int volatile *remain;
	int r;

  public:
	semaphore(int inital);
	void aquire();
	void release();
	bool check();
};

class mutex : public semaphore {
	mutex() : semaphore(1) {}
};
class remutex {
	int volatile *count;
	int c;
	uint32_t volatile *thread_id;
	uint32_t t;

  public:
	remutex();
	void aquire();
	void release();
	bool check();
};
template <class T> class syncRAII {
	T &object;

  public:
	syncRAII(T &obj) : object(obj) { object.aquire(); }
	~syncRAII() { object.release(); }
};
#endif
