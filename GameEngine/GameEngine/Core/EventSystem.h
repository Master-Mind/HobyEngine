#pragma once
#include <unordered_map>
#include "Delegate.h"
#include <typeinfo>
typedef Delegate<void(Event *)> EventHandler;
class EventSystem
{
public:
	template<typename T>
	void Subscribe(EventHandler *func)
	{
		_subscribers[typeid(T).hash_code()].push_back(func);
	}

	template<typename T>
	void UnSubscribe(const void *object)
	{
		for (auto sub = _subscribers[typeid(T).hash_code()].begin(); sub != _subscribers[typeid(T).hash_code()].end(); sub++)
		{
			if((*sub)->HasObj(object))
			{
				delete *sub;
				_subscribers[typeid(T).hash_code()].erase(sub);
				break;
			}
		}
	}

	template<typename T>
	void Register(T *event)
	{
		_eventCopies[typeid(T).hash_code()] = event;
	}

	template<typename T>
	void Trigger()
	{
		for (auto sub = _subscribers[typeid(T).hash_code()].begin(); sub != _subscribers[typeid(T).hash_code()].end(); sub++)
		{
			(*sub)->invoke(_eventCopies[typeid(T).hash_code()]);
		}
	}
private:
	std::unordered_map<size_t, std::list<EventHandler*>> _subscribers;
	std::unordered_map<size_t, Event *> _eventCopies;
};
