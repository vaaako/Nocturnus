#include "nocturnus/roguelike.hpp"

Roguelike::Roguelike(const Terminal& terminal, const uint32 width, const uint32 height)
	: terminal(terminal), width(width), height(height),
	  screen_buffer(std::vector<std::vector<char>>(height, std::vector<char>(width, ' '))) {
}


void Roguelike::putchar(const uint32 x, const uint32 y, const char ch) {
	if(this->inbounds(x, y)) {
		this->screen_buffer[y + 1][x + 1] = ch;
		this->terminal.move_cursor(x, y);
		std::cout << ch << std::flush;
	}
}

char Roguelike::getchar(const uint32 x, const uint32 y) const {
	if(this->inbounds(x, y)) {
		return this->screen_buffer[y + 1][x + 1];
	}
	return '\0';
}

