#ifndef  _MYQUEUE_H_
#define  _MYQUEUE_H_

#include <deque>

template <typename T> class MyQueue {
private:
	std::deque<T> _deque;

public:
	MyQueue();

	bool push_back(const T& data);
	bool front(T& data);
	bool pop_front();

	bool empty() const;
	int size() const;
};

template<typename T>
inline MyQueue<T>::MyQueue()
{
}

template<typename T>
inline bool MyQueue<T>::push_back(const T & data)
{
	_deque.push_back(data);
	data->retain();		//いきなりdeleteされないためにリファレンスカウンタ+1

	return true;
}

template<typename T>
inline bool MyQueue<T>::front(T & data)
{
	if (_deque.empty()) {
		return false;
	}

	data = std::move(_deque.front());

	return true;
}

template<typename T>
inline bool MyQueue<T>::pop_front()
{
	if (_deque.empty()) {
		return false;
	}

	auto data = std::move(_deque.front());
	_deque.pop_front();

	data->release();	//リファレンスカウンタを-1

	return true;
}

template<typename T>
inline bool MyQueue<T>::empty() const
{
	return _deque.empty();
}

template<typename T>
inline int MyQueue<T>::size() const
{
	return _deque.size();
}

#endif // _MYQUEUE_H_

