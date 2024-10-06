#pragma once

#include "nocturnus/terminal.hpp"
#include "nocturnus/typedef.hpp"
#include <vector>

// NOTE -- Terminal starts at 1, 1

class Roguelike {
	public:
		Roguelike(const Terminal& terminal, const uint32 width, const uint32 height);

		inline bool inbounds(const uint32 x, const uint32 y) const {
			return (x >= 0 && x < this->width && y >= 0 && y < this->height);
		}

		void putchar(const uint32 x, const uint32 y, const char ch);
		char getchar(const uint32 x, const uint32 y) const;

		template <typename T>
		bool willcollide(const vec2<T>& newpos, const char ch) const;

	private:
		Terminal terminal;
		const uint32 width;
		const uint32 height;

		// Keep track of chars in screen
		// Fill with space
		std::vector<std::vector<char>> screen_buffer;
};


template <typename T>
bool Roguelike::willcollide(const vec2<T>& newpos, const char ch) const {
	return this->getchar(newpos.x, newpos.y) == ch;
}
