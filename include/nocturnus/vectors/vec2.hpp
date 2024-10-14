#pragma once

#include <tuple>
#include <utility>

template <typename T>
struct vec2 {
	T x;
	T y;

	vec2() = default;
	vec2(T x, T y) : x(x), y(y) {}
	vec2(const vec2& other) = default;
	vec2(vec2&& other) = default;

	vec2<T>& operator+(const vec2<T>& other) const noexcept {
		return { this->x + other.x, this->y + other.y };
	}

	vec2<T>& operator+=(const vec2<T>& other) noexcept {
		this->x += other.x;
		this->y += other.y;
		return { this->x + other.x, this->y + other.y };
	}

	bool operator==(const vec2<T>& other) const noexcept {
		return (this->x == other.x) && (this->y == other.y);
	}

	bool operator!=(const vec2<T>& other) const noexcept {
		return !(*this == other);
	}


	bool operator<(const vec2& other) const {
		return std::tie(x, y) < std::tie(other.x, other.y);
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
		}
		return *this;
	}
};

