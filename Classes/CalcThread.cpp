#include "CalcThread.h"
#include <assert.h>

//乱数定義
namespace {
	std::mt19937 mt(static_cast<unsigned long>(time(NULL)));
	std::uniform_real_distribution<float> randX(20.0, 460.0);
	std::uniform_real_distribution<float> randY(20.0, 300.0);
}

USING_NS_CC;

void CalcThread::run()
{
	while (true) {
		{
			std::unique_lock<std::mutex> uniq_lk(_mutex);
			while (_inQueue.empty()) {
				if (_isTerminationRequested) {
					return;
				}

				_cv.wait(uniq_lk);
			}
		}

		//処理対象を取得
		Char * data;
		auto result = _inQueue.front(data);
		assert(result);

		//処理開始
		float destX = randX(mt);
		float destY = randY(mt);

		//処理時間待ち(0.5秒)
		auto wk = std::chrono::milliseconds(500);
		std::this_thread::sleep_for(wk);

		//Destinationはメインスレッドでも扱うのでロックする。
		data->getLock().writeLock();
		data->setDestination(Vec2(destX, destY));
		data->getLock().writeUnlock();

		//outに入れてからinから消す。
		//(queueに属していない瞬間があるとメインスレッドで削除される可能性あり。)
		_outQueue.push_back(data);
		_inQueue.pop_front();
	}
}

CalcThread::CalcThread()
	: _isTerminationRequested(false)
{
	t = std::thread(&CalcThread::run, this);
}

CalcThread::~CalcThread()
{
	{
		std::unique_lock<std::mutex> uniq_lk(_mutex);
		_isTerminationRequested = true;
	}

	_cv.notify_all();

	if (t.joinable()) {
		t.join();
	}
}

bool CalcThread::append(Char * data)
{
	{
		std::unique_lock<std::mutex> uniq_lk(_mutex);
		if (!_inQueue.push_back(data)) {
			return false;
		}
	}

	_cv.notify_all();
	return true;
}

int CalcThread::waitCount()
{
	return _inQueue.size();
}

bool CalcThread::hasResult()
{
	std::unique_lock<std::mutex> uniq_lk(_mutex);

	if (_outQueue.empty()) {
		return false;
	}
	else {
		return true;
	}
}

Char * CalcThread::getResult()
{
	std::unique_lock<std::mutex> uniq_lk(_mutex);

	Char * data;
	auto result = _outQueue.front(data);

	if (result) {
		_outQueue.pop_front();
		return data;
	}
	else {
		return nullptr;
	}
}
