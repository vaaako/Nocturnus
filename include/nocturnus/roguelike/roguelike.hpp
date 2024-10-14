#pragma once

#include "nocturnus/terminal.hpp"
#include "nocturnus/typedef.hpp"
#include "nocturnus/vectors/vec2.hpp"
#include <vector>

enum class Tile : uint8 {
	EMPTY = '\0',
	PLAYER = '@',

	LRWALL = '|', // Left right wall
	TBWALL = '-', // Top bottom wall

	FLOOR = '.',
	DOOR  = '+',
	PATH  = '#'
};

struct Room {
	vec2<uint16> pos; // top-left position
	uint16 width;
	uint16 height;

	// Maximum of 4 doors
	vec2<uint16> doors[4] {
		// Defaults
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 },
		{ 0, 0 }
	};
};


// TODO -- On make_path, ignore other paths, currently is only checking for white spaces, if check for a path char, it will go around

class Roguelike {
	public:
		Roguelike(Terminal* terminal);

		// ROOM MAKER //

		// Make a room in coords and put on a vector of rooms.
		// The width and height of a room is the size of the interior, not counting the walls, so a room 5x5 counting with the walls is actually 7x7
		Room make_room(const uint16 x, const uint16 y, const uint16 width, const uint16 height);

		// Draw a room.
		// Don't use this method with draw_rooms, to avoid drawing the same rooom twice
		void draw_room(const Room& room);

		// Draw all rooms created.
		// Don't use this method with draw_room, to avoid drawing the same rooom twice
		void draw_rooms();

		// PATH MAKER //

		// Try to make a path between two locations
		// void make_path(const vec2<uint16>& start, const vec2<uint16>& target, const char pathchar = '#') const;
		void make_path(const vec2<uint16>& start, const vec2<uint16>& target, const char pathchar = '#') const;

		// Make a horizontal path between two locations.
		// x1: Where to start.
		// x2: Where to end.
		// y:  Y of the path
		void make_h_path(const uint16 x1, const uint16 x2, const uint16 y, const char pathchar = '#') const;
		// Make a Vertical path between two locations
		// y1: Where to start.
		// y2: Where to end.
		// x:  X of the path
		void make_v_path(const uint16 y1, const uint16 y2, const uint16 x, const char pathchar = '#') const;


		// UTILITY //

		// Generate a random integer between two numbers
		int randint(const int min, const int max);
		// Generate a random integer between 0 and a number
		int randint(const int max);

	private:
		Terminal* terminal;
		std::vector<Room> rooms;
};

