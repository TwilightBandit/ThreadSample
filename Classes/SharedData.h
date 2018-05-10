#ifndef  _SHAREDDATA_H_
#define  _SHAREDDATA_H_

#include <mutex>
#include <condition_variable>

#include "ReadWriteLock.h"

class SharedData {
private:
	ReadWriteLock lock;

	int dataInt;
	std::string dataString;

public:
	SharedData();
	~SharedData();

	ReadWriteLock & getLock();

	int getDataInt();
	void setDataInt(int value);

};

#endif // _SHAREDDATA_H_

