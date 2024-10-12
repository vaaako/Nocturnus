#pragma once

#include "nocturnus/terminal.hpp"
#include "nocturnus/typedef.hpp"
#include <vector>

// NOTE -- Terminal starts at 1, 1

class Roguelike {
	public:
		Roguelike(const Terminal& terminal, const uint32 width, const uint32 height);

		// Check if coordinates are inside terminal bounds
		inline bool inbounds(const uint32 x, const uint32 y) const {
			return (x >= 0 && x < this->width && y >= 0 && y < this->height);
		}

		// Put char, used for interactable characters
		void putchar(const uint32 x, const uint32 y, const char ch);

		// Get char in some location
		char getchar(const uint32 x, const uint32 y) const;

		// Make a room in X,Y with Width and Height of size
		void make_room(const uint32 xdest, const uint32 ydest, const uint32 width, const uint32 height, const char wall);

		void render_map();

		template <typename T>
		// Check if vector is colliding with some character
		bool willcollide(const vec2<T>& newpos, const char ch) const;

	private:
		Terminal terminal;
		const uint32 width;
		const uint32 height;

		// Keep track of chars in screen
		// Fill with space
		std::vector<std::vector<char>> screen_buffer;
		std::vector<std::vector<char>> prev_buffer;
};


template <typename T>
bool Roguelike::willcollide(const vec2<T>& newpos, const char ch) const {
	return this->getchar(newpos.x, newpos.y) == ch;
}
