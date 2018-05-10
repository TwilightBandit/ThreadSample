#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
//#include "ui/CocosGUI.h"

#include "Char.h"

#include "CalcThread.h"

class HelloWorld : public cocos2d::Scene
{
public:
	struct Data
	{
		int data;
		std::mutex mtx;
	};

	~HelloWorld();

    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	void menuAddCallback(cocos2d::Ref* pSender);
	void menuDelCallback(cocos2d::Ref* pSender);
	void menuCalcCallback(cocos2d::Ref* pSender);

	void update(float delta);

private:
	cocos2d::Label * _label;
	cocos2d::Vector<Char *> _charList;
	int _tagCount;

	CalcThread * _calcThread;
};

#endif // __HELLOWORLD_SCENE_H__
