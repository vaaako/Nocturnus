#pragma once

#include "nocturnus/terminal.hpp"
#include "nocturnus/typedef.hpp"

struct Player {
	Player(Terminal& terminal, const vec2<uint16>& pos);
	vec2<uint16> pos;

	inline void draw() {
		terminal.putchar(this->pos.x, this->pos.y, '@');
	}

	inline void move(const vec2<uint16>& newpos, const char footprint = '.') {
		terminal.putchar(this->pos.x, this->pos.y, footprint);
		this->pos = newpos;
		terminal.putchar(this->pos.x, this->pos.y, '@');
	}

	private:
		Terminal& terminal;
};
