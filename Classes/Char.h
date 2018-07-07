#pragma execution_character_set("utf-8")
//
// Created by twilight_bandit on 2017/09/16.
//

#ifndef CHAR_H
#define CHAR_H

#include "cocos2d.h"
#include "ReadWriteLock.h"

class Char  : public cocos2d::Sprite {

protected:
	Char();
	virtual ~Char();

	ReadWriteLock lock;

//操作
public:
	static Char * create(const std::string & modelPath);
	
	ReadWriteLock & getLock();

	cocos2d::Vec2 getDestination() const;
	void setDestination(cocos2d::Vec2 pos);

	float getSpeed() const;
	void setSpeed(float value);

	void move();

protected:

//変数
private:
	cocos2d::Vec2 _destination;
	float _speed;
};


#endif //CHAR_H
