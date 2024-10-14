#pragma once

#include "nocturnus/terminal.hpp"
#include "nocturnus/typedef.hpp"
#include <vector>

enum Block : uint8 {
	EMPTY = ' ',
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
	// Default to fill later
	vec2<uint16> doors[4];
};

// Instead of one class for roguelike, make some namespaces
// namespace pathfind {
//
// };

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

		// Try to make a path between two locations
		void make_path(const vec2<uint16>& start, const vec2<uint16>& target) const;
		bool make_path_recursive(const vec2<uint16>& pathpos, const vec2<uint16>& target) const;

		// Generate a random integer between two numbers
		int randint(const int min, const int max);

		// Generate a random integer between 0 and a number
		int randint(const int max);

		void create_h_tunnel(int x1, int x2, int y);
		void create_v_tunnel(int y1, int y2, int x);

	private:
		Terminal* terminal;
		std::vector<Room> rooms;
};

