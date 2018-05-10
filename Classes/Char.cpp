#pragma execution_character_set("utf-8")

#include "Char.h"

USING_NS_CC;

Char::Char()
	: _destination(cocos2d::Vec2::ZERO)
	, _speed(0.0f)
{
}

Char::~Char()
{
}

Char * Char::create(const std::string & modelPath)
{
	auto sprite = new (std::nothrow) Char();
	if (sprite && sprite->initWithFile(modelPath))
	{
		sprite->_contentSize = sprite->getBoundingBox().size;
		sprite->autorelease();
		return sprite;
	}

	CC_SAFE_DELETE(sprite);
	return nullptr;
}

ReadWriteLock & Char::getLock()
{
	return lock;
}

cocos2d::Vec2 Char::getDestination() const
{
	return _destination;
}

void Char::setDestination(cocos2d::Vec2 pos)
{
	_destination = pos;
}

float Char::getSpeed() const
{
	return _speed;
}

void Char::setSpeed(float value)
{
	_speed = value;
}

void Char::move()
{
	auto nowPos = getPosition();
	auto spd = getSpeed();

	//Destinationは別Threadで扱うのでロックする。
	this->getLock().readLock();
	auto dest = getDestination();
	this->getLock().readUnlock();

	if (nowPos != dest) {
		auto diff = nowPos - dest;
		auto moveLen = diff.getNormalized() * spd;

		if (abs(diff.x) <= abs(moveLen.x)) {
			nowPos.x = dest.x;
		}
		else {
			nowPos.x -= moveLen.x;
		}

		if (abs(diff.y) <= abs(moveLen.y)) {
			nowPos.y = dest.y;
		}
		else {
			nowPos.y -= moveLen.y;
		}

		setPosition(nowPos);
	}
}
