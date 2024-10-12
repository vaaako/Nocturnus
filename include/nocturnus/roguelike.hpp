#pragma once

#include "nocturnus/terminal.hpp"
#include "nocturnus/typedef.hpp"
#include <vector>


struct Room {
	vec2<uint16> pos; // top-left position
	uint16 width;
	uint16 height;

	// Maximum of 4 doors
	// Default to fill later
	vec2<uint16> doors[4];
};

class Roguelike {
	public:
		Roguelike(Terminal* terminal);

		// Make a room in coords and put on a vector of rooms.
		// The width and height of a room is the size of the interior, not counting the walls, so a room 5x5 counting with the walls is actually 7x7
		Room make_room(const uint16 x, const uint16 y, const uint16 width, const uint16 height);

		// Draw a room.
		// Don't use this method with draw_rooms, to avoid drawing the same rooom twice
		void draw_room(const Room& room);

		// Draw all rooms created.
		// Don't use this method with draw_room, to avoid drawing the same rooom twice
		void draw_rooms();

		// 
		void connect_doors(const vec2<uint16>& door1, const vec2<uint16> door2);

		// Generate a random integer between two numbers
		int randint(const int min, const int max);

		// Generate a random integer between 0 and a number
		int randint(const int max);

	private:
		Terminal* terminal;
		std::vector<Room> rooms;
};

