#pragma once

#include <utility>

template <typename T>
struct vec2 {
	T x;
	T y;

	vec2() = default;
	vec2(T x, T y) : x(x), y(y) {}
	vec2(const vec2& other) = default;
	vec2(vec2&& other) = default;

	vec2<T>& operator+(const vec2<T>& other) noexcept {
		return { this->x + other.x, this->y + other.y };
	}

	vec2<T>& operator+=(const vec2<T>& other) noexcept {
		this->x += other.x;
		this->y += other.y;
		return { this->x + other.x, this->y + other.y };
	}

	vec2<T>& operator=(const vec2<T>& other) noexcept {
		if(this != &other) {
			this->x = other.x;
			this->y = other.y;
		}
		return *this;
	}

	vec2<T>& operator=(const T other) noexcept {
		this->x = other;
		this->y = other;
		return *this;
	}

	vec2<T>& operator=(vec2<T>&& other) noexcept {
		if (this != &other) {
			this->x = std::move(other.x);
			this->y = std::move(other.y);
			other.clear();
		}
		return *this;
	}
};
