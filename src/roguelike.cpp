#include "nocturnus/roguelike.hpp"
#include <random>

Roguelike::Roguelike(Terminal* terminal)
	: terminal(terminal) {

}

Room Roguelike::make_room(const uint16 x, const uint16 y, const uint16 width, const uint16 height) {
	Room room = {
		{ x, y },
		width, height,

		// Doors
		{
			// Top door (random x within room's width, fixed y at the top of the room)
			{ static_cast<uint16>((x + 1) + this->randint(width - 1)), y },

			// Right door (fixed x at the right edge of the room, random y within room's height)
			// x: width + 1 fit with the draw_room
			{ static_cast<uint16>(x + width + 1), static_cast<uint16>((y + 1) + this->randint(height - 1)) },

			// Bottom door (random x within room's width, fixed y at the bottom of the room)
			// y: height + 1 fit with the draw_room
			{ static_cast<uint16>((x + 1) + this->randint(width - 1)), static_cast<uint16>(y + height + 1) },

			// Left door (fixed x at the left edge of the room, random y within room's height)
			{ x, static_cast<uint16>((y + 1) + this->randint(height - 1)) }

			// Not commented operations are for preventing doors for generating on the edges
		}
	};


	this->rooms.push_back(room);
	return room;
}


void connect_doors(const vec2<uint16>& door1, const vec2<uint16> door2) {

}

void Roguelike::draw_room(const Room& room) {
	// +2 fit with the right wall
	// +1 go one more to the right and make the inside the right size
	for(int x = room.pos.x; x < (room.pos.x + room.width) + 2; x++) {
		this->terminal->putchar(x, room.pos.y, '-'); // Top
		this->terminal->putchar(x, (room.pos.y + room.height) + 1, '-'); // Bottom
	}

	// +1 start under the top wall
	// +1 go one more below and make the inside the right size
	// +1 properly fill
	for(int y = room.pos.y + 1; y < (room.pos.y + room.height) + 1; y++) {
		this->terminal->putchar(room.pos.x, y, '|'); // Left
		this->terminal->putchar((room.pos.x + room.width) + 1, y, '|'); // Right

		// +1 start below the top wall
		// +1 stop above the bottom wall
		for(int x = room.pos.x + 1; x < (room.pos.x + room.width) + 1; x++) {
			this->terminal->putchar(x, y, '.');
		}
	}

	// Draw rooms
	// Top
	this->terminal->putchar(room.doors[0].x, room.doors[0].y, '+');
	// Right
	this->terminal->putchar(room.doors[1].x, room.doors[1].y, '+');
	// Bottom
	this->terminal->putchar(room.doors[2].x, room.doors[2].y, '+');
	// Left
	this->terminal->putchar(room.doors[3].x, room.doors[3].y, '+');
}


void Roguelike::draw_rooms() {
	for(Room& room : this->rooms) {
		this->draw_room(room);
	}

	// debug
}



int Roguelike::randint(const int min, const int max) {
	if(min > max) {
		return -1;
	}

	std::random_device rd;
	std::mt19937 gen = std::mt19937(rd()); // Use seed to generate a number

	std::uniform_int_distribution<> distrib(min, max);
	return distrib(gen);
}

int Roguelike::randint(const int max) {
	return this->randint(0, max);
}
