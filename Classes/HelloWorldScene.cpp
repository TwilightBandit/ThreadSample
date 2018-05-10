#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

//#include <thread>
//#include <iostream>
//
//#include "ReadWriteLock.h"
//#include "ReadThread.h"
//#include "WriteThread.h"
//#include "MyData2.h"

#include "Char.h"

USING_NS_CC;

//乱数定義
namespace {
	std::mt19937 mt(static_cast<unsigned long>(time(NULL)));
	std::uniform_real_distribution<float> randX(20.0, 460.0);
	std::uniform_real_distribution<float> randY(20.0, 300.0);
	std::uniform_real_distribution<float> randSpeed(0.1, 1.0);
}

HelloWorld::~HelloWorld()
{
	if (_calcThread) {
		delete _calcThread;
		_calcThread = nullptr;
	}
}

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	// Char Add Button
	auto addItem = MenuItemImage::create(
		"AddButton_off.png",
		"AddButton_on.png",
		CC_CALLBACK_1(HelloWorld::menuAddCallback, this));

	if (addItem == nullptr ||
		addItem->getContentSize().width <= 0 ||
		addItem->getContentSize().height <= 0)
	{
		problemLoading("'AddButton_off.png' and 'AddButton_on.png'");
	}
	else
	{
		float x = origin.x + addItem->getContentSize().width / 2;
		float y = origin.y + visibleSize.height - addItem->getContentSize().height / 2;
		addItem->setPosition(Vec2(x, y));
	}

	auto menuAdd = Menu::create(addItem, NULL);
	menuAdd->setPosition(Vec2::ZERO);
	this->addChild(menuAdd, 1);

	// Char Del Button
	auto delItem = MenuItemImage::create(
		"DelButton_off.png",
		"DelButton_on.png",
		CC_CALLBACK_1(HelloWorld::menuDelCallback, this));

	if (delItem == nullptr ||
		delItem->getContentSize().width <= 0 ||
		delItem->getContentSize().height <= 0)
	{
		problemLoading("'DelButton_off.png' and 'DelButton_on.png'");
	}
	else
	{
		float x = addItem->getPosition().x;
		float y = addItem->getPosition().y - delItem->getContentSize().height * 2;
		delItem->setPosition(Vec2(x, y));
	}

	auto menuDel = Menu::create(delItem, NULL);
	menuDel->setPosition(Vec2::ZERO);
	this->addChild(menuDel, 1);

	// Calclate Button
	auto calcItem = MenuItemImage::create(
		"CaclButton_off.png",
		"CaclButton_on.png",
		CC_CALLBACK_1(HelloWorld::menuCalcCallback, this));

	if (calcItem == nullptr ||
		calcItem->getContentSize().width <= 0 ||
		calcItem->getContentSize().height <= 0)
	{
		problemLoading("'CalcButton_off.png' and 'CalcButton_on.png'");
	}
	else
	{
		float x = addItem->getPosition().x;
		float y = addItem->getPosition().y - calcItem->getContentSize().height * 4;
		calcItem->setPosition(Vec2(x, y));
	}

	auto menuCalc = Menu::create(calcItem, NULL);
	menuCalc->setPosition(Vec2::ZERO);
	this->addChild(menuCalc, 1);

	// CalcThreadでの計算中ラベル
	_label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
	_label->setPosition(Vec2(addItem->getPosition().x, addItem->getPosition().y - calcItem->getContentSize().height * 5));

	this->addChild(_label, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

	_tagCount = 0;
	_charList.clear();

	_calcThread = new CalcThread();

	this->scheduleUpdate();

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::menuAddCallback(cocos2d::Ref * pSender)
{
	CCLOG("HelloWorld::menuAddCallback start");

	float stX = randX(mt);
	float stY = randY(mt);
	float destX = randX(mt);
	float destY = randY(mt);
	float spd = randSpeed(mt);

	auto wkChar = Char::create("char.png");

	wkChar->setPosition(Vec2(stX, stY));
	wkChar->setDestination(Vec2(destX, destY));
	wkChar->setSpeed(spd);

	this->addChild(wkChar, 1);
	_charList.pushBack(wkChar);

	auto wkLine = DrawNode::create();
	wkLine->drawSegment(Vec2(stX, stY), Vec2(destX, destY), 1.0f, Color4F::RED);
	this->addChild(wkLine, 2);
	//_lineList.pushBack(wkLine);

	_tagCount++;
	wkChar->setTag(_tagCount);
	wkLine->setTag(_tagCount + 2000);

	CCLOG("HelloWorld::menuAddCallback end");
}

void HelloWorld::menuDelCallback(cocos2d::Ref * pSender)
{
	if (_charList.size() > 0) {
		auto wkChar = _charList.at(0);
		auto wkLine = this->getChildByTag<DrawNode *>(wkChar->getTag() + 2000);

		this->removeChild(wkChar);
		_charList.erase(0);

		this->removeChild(wkLine);
	}
}

void HelloWorld::menuCalcCallback(cocos2d::Ref * pSender)
{
	CCLOG("HelloWorld::menuCalcCallback start");
	clock_t start = clock();

	for (auto wkChar : _charList) {
		_calcThread->append(wkChar);
	}

	CCLOG("HelloWorld::menuCalcCallback end time=%f sec", (double)(clock() - start) / CLOCKS_PER_SEC);
}

void HelloWorld::update(float delta)
{
	//待ち数
	auto cnt = _calcThread->waitCount();
	if (cnt) {
		_label->setString("Wait : " + StringUtils::toString(cnt));
	}
	else {
		_label->setString("");
	}

	//calcThreadでの計算結果が存在する場合の処理
	while (_calcThread->hasResult()) {

		//calcThreadでの結果を取得する。
		auto wkChar = _calcThread->getResult();

		auto itr = _charList.find(wkChar);
		auto i = std::distance(_charList.begin(), itr);

		//calcThreadで計算中に、Char Delされている場合には、何もしない。
		//(calcThreadで処理している間、Charクラスは自動削除されない。
		// ただし、Char DELボタンによって_charListからは削除されている可能性あり。)

		if (i < _charList.size()) {
			//まだ存在しているので処理を行う。

			//Destinationは別Threadでも扱うのでロックする。
			wkChar->getLock().readLock();
			auto wkDest = wkChar->getDestination();
			wkChar->getLock().readUnlock();

			auto wkLine = this->getChildByTag<DrawNode *>(wkChar->getTag() + 2000);
            this->removeChild(wkLine);

			auto wkLine2 = DrawNode::create();
			wkLine2->drawSegment(wkChar->getPosition(), wkDest, 1.0f, Color4F::RED);
			wkLine2->setTag(wkChar->getTag() + 2000);
            this->addChild(wkLine2, 2);
		}
	}

	for (auto wkChar : _charList) {
		wkChar->move();
	}
}
