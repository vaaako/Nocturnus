#include "nocturnus/roguelike.hpp"
#include <algorithm>
#include <cstdlib>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <unordered_set>

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

	Room first = this->rooms[0];
	Room second = this->rooms[1];
	Room third  = this->rooms[2];


	// DEBUG
	
	vec2<uint16> door1 = first.doors[1];
	vec2<uint16> door2 = second.doors[3];
	vec2<uint16> door3 = second.doors[0];
	vec2<uint16> door4 = third.doors[1];

	// Calculate where is the front of each door
	door1.x += 1;
	door2.x -= 1;
	door3.y -= 1;
	door4.x += 1;
	this->make_path(door1, door2);
	this->make_path(door3, door4);

	// Could randomly choose which orientation to start
	// this->make_h_path(door1.x, door2.x, door1.y);
	// this->make_v_path(door2.y, door1.y, door2.x);
	//
	// this->make_h_path(door3.x, door4.x, door3.y);
	// this->make_v_path(door4.y, door3.y, door4.x);
}

void Roguelike::make_path(const vec2<uint16>& start, const vec2<uint16>& target, const char pathchar) const {
	if(start == target) {
		return;
	}

	vec2<uint16> pathpos = start;
	vec2<uint16> previous = pathpos;

	bool backtrack = false;

	// Directions (dx, dy)
	std::array<vec2<int16>, 4> directions = {
		// Path priority
		vec2<int16>{ -1,  0 }, // Left
		vec2<int16>{  1,  0 }, // Right
		vec2<int16>{  0, -1 }, // Up
		vec2<int16>{  0,  1 }  // Down
	};

	// Randomize the direction priority
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(directions.begin(), directions.end(), g); // Shuffle array

	// First path in front of the door
	this->terminal->putchar(pathpos.x, pathpos.y, pathchar);

	// Start path
	while(pathpos != target) {
		bool moved = false;

		// Check all directions
		for(const vec2<int16>& dir : directions) {
			vec2<uint16> newpos = {
				static_cast<uint16>(pathpos.x + dir.x),
				static_cast<uint16>(pathpos.y + dir.y)
			};

			// Check if newpos is an empty space
			if(this->terminal->mvinch(newpos.x, newpos.y) != ' ') {
				continue;
			}

			// Check newpos is closer to the target
			if(std::abs(newpos.x - target.x) + std::abs(newpos.y - target.y)
				< std::abs(pathpos.x - target.x) + std::abs(pathpos.y - target.y)) {

				// Move
				previous = pathpos; // Store previous position for backtrack
				pathpos = newpos;

				moved = true;
				break;
			}
		}

		// If no valid move was found
		if(!moved) {
			// Try to backtrack
			if(!backtrack) {
				// Go back and try again
				pathpos = previous;
				backtrack = true;
				continue;
			}

			// No possible direction
			break;
		}

		// Draw the path
		this->terminal->putchar(pathpos.x, pathpos.y, pathchar);
		// this->terminal->waitkey(); // DEBUG
	}

	// Reached target or no possible directions
}




void Roguelike::make_h_path(const uint16 x1, const uint16 x2, const uint16 y, const char pathchar) const {
	// min and amx are used in case x1 > x2
	for (int x = std::min(x1, x2); x < std::max(x1, x2); ++x) {
		this->terminal->putchar(x, y, pathchar);
	}
}


void Roguelike::make_v_path(const uint16 y1, const uint16 y2, const uint16 x, const char pathchar) const {
	// +1 Go until reach the horizontal path (if have it)
	for (int y = std::min(y1, y2); y < std::max(y1, y2) + 1; ++y) {
		this->terminal->putchar(x, y, pathchar);
	}
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
