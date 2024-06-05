#pragma once

namespace utils {
	template <typename T> int sign(T a) {
		return (T(0) < a) - (a < T(0));
	}
}