#ifndef  _READWRITELOCK_H_
#define  _READWRITELOCK_H_

#include <mutex>
#include <condition_variable>
#include <thread>

class ReadWriteLock {
private:
	std::mutex mtx;
	std::condition_variable cv;
	int readingReaders;
	int waitingWriters;
	int writingWriters;
	bool preferWriter;

public:
	ReadWriteLock();
	~ReadWriteLock();
	void readLock();
	void readUnlock();
	void writeLock();
	void writeUnlock();
};

#endif // _READWRITELOCK_H_

