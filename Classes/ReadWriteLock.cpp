#include "ReadWriteLock.h"

ReadWriteLock::ReadWriteLock()
	: readingReaders(0)
	, waitingWriters(0)
	, writingWriters(0)
	, preferWriter(true)
{
}

ReadWriteLock::~ReadWriteLock()
{
}

void ReadWriteLock::readLock()
{
	{
		std::unique_lock<std::mutex> uniq_lk(mtx);
		while (writingWriters > 0 || (preferWriter && waitingWriters > 0)) {
			cv.wait(uniq_lk);
		}
		this->readingReaders++;
	}
}

void ReadWriteLock::readUnlock()
{
	{
		std::unique_lock<std::mutex> uniq_lk(mtx);
		this->readingReaders--;
		this->preferWriter = true;
		cv.notify_all();
	}
}

void ReadWriteLock::writeLock()
{
	{
		std::unique_lock<std::mutex> uniq_lk(mtx);
		this->waitingWriters++;
		while (readingReaders > 0 || writingWriters > 0) {
			cv.wait(uniq_lk);
		}
		this->waitingWriters--;
		this->writingWriters++;
	}
}

void ReadWriteLock::writeUnlock()
{
	{
		std::unique_lock<std::mutex> uniq_lk(mtx);
		this->writingWriters--;
		this->preferWriter = false;
		cv.notify_all();
	}
}
