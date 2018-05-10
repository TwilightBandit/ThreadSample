#ifndef  _HEAVYTHREAD_H_
#define  _HEAVYTHREAD_H_

#include <mutex>
#include <condition_variable>
#include <thread>

#include "MyQueue.h"
#include "Char.h"

class CalcThread {
private:
	MyQueue<Char *> _inQueue;
	MyQueue<Char *> _outQueue;
	std::mutex _mutex;
	std::condition_variable _cv;

	bool _isTerminationRequested;

protected:
	std::thread t;
	void run();

public:
	CalcThread();
	virtual ~CalcThread();

	bool append(Char * data);
	int waitCount();
	bool hasResult();
	Char * getResult();
};

#endif // _HEAVYTHREAD_H_

