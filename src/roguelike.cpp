#include "nocturnus/roguelike.hpp"
#include <cstring>

Roguelike::Roguelike(const Terminal& terminal, const uint32 width, const uint32 height)
	: terminal(terminal), width(width), height(height),
	  screen_buffer(std::vector<std::vector<char>>(height, std::vector<char>(width, ' '))),
	  prev_buffer(std::vector<std::vector<char>>(height, std::vector<char>(width, ' '))) {

}


void Roguelike::putchar(const uint32 x, const uint32 y, const char ch) {
	if(this->inbounds(x, y)) {
		this->screen_buffer[y][x] = ch; // This is line is causing lag on windows
	}
}

char Roguelike::getchar(const uint32 x, const uint32 y) const {
	if(this->inbounds(x, y)) {
		return this->screen_buffer[y][x];
	}
	return '\0';
}

void Roguelike::make_room(const uint32 xdest, const uint32 ydest, const uint32 width, const uint32 height, const char wall) {
	for(uint32 y = ydest; y < height; y++) {
		for(uint32 x = xdest; x < width; x++) {
			if(x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				this->putchar(x, y, wall);
			}
		}
	}
}

void Roguelike::render_map() {
	for(uint32 y = 0; y != this->height; y++) {
		for(uint32 x = 0; x != this->width; x++) {

			// if(this->screen_buffer[y][x] == this->prev_buffer[y][x]) {
			// 	continue;
			// }
			// this->terminal.move_cursor(x, y);
			// std::cout << this->screen_buffer[y][x];

			char ch = screen_buffer[y][x];
			if(ch == ' ') {
				continue;
			}
			this->terminal.putchar(x, y, ch);
		}
	}

	std::cout.flush();
	this->prev_buffer = this->screen_buffer;
}
