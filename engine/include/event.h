#pragma once

#include <vector>
#include <functional>

template<typename T>
class EventDispatcher {
private:
	std::vector<std::function<void(const T&)>> callbacks;
public:
	void dispatch(const T& eventData) {
		for (auto listener : this->callbacks) {
			listener(eventData);
		}
	}
	void addListener(std::function<void(const T&)> callback) { callbacks.push_back(callback); };
};