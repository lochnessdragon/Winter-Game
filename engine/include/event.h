#pragma once

#include <vector>
#include <functional>

template<typename T>
class EventDispatcher {
private:
	typedef bool CallbackFunc(const T&);
	std::vector<std::function<CallbackFunc>> callbacks;
public:

	void dispatch(const T& eventData) {
		bool handled = false;
		size_t idx = 0;
		while (!handled && idx < this->callbacks.size()) {
			auto listener = this->callbacks[idx];
			handled = listener(eventData);
			idx++;
		}
	}
	void addListener(std::function<CallbackFunc> callback) { callbacks.push_back(callback); };
};