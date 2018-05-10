#include "SharedData.h"

SharedData::SharedData()
	: dataInt(0)
	, dataString("")
{
}

SharedData::~SharedData()
{
}

ReadWriteLock & SharedData::getLock()
{
	return lock;
}

int SharedData::getDataInt()
{
	return dataInt;
}

void SharedData::setDataInt(int value)
{
	dataInt = value;
}
