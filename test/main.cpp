#include "nocturnus/roguelike.hpp"
#include "nocturnus/terminal.hpp"
#include "player.hpp"

void setup_map(Roguelike& rogue) {
	rogue.make_room(13, 13, 6, 4);
	rogue.make_room(40, 10, 10, 4);
	rogue.make_room(40, 2, 6, 4);

	rogue.draw_rooms();
}

void update_term(Terminal& terminal) {
	// Create player
	Player player = Player(terminal, { 14, 14 });

	char ch;
	while((ch = terminal.getkey()) != 'q') {
		vec2<uint16> newpos = player.pos;
		switch (ch) {
			case 'w':
				newpos.y -= 1;
				break;
			case 'a':
				newpos.x -= 1;
				break;
			case 's':
				newpos.y += 1;
				break;
			case 'd':
				newpos.x += 1;
				break;
		}


		// terminal.putstring(0, 1, ("New pos char: " + std::string(1, terminal.mvinch(newpos.x, newpos.y))).c_str());

		// Collision
		switch (terminal.mvinch(newpos.x, newpos.y)) {
			// Walkable, move
			case '.':
			case '#':
			case '+':
				player.move(newpos);
				break;

			// Don't move if is not walkable
			default:
				break;
		}

		terminal.putstring(0, 0, ("X: " + std::to_string(player.pos.x) + " Y: " + std::to_string(player.pos.y)).c_str());

		player.draw();
	}
}


int main() {
	// Setup
	Terminal terminal = Terminal();
	terminal.hide_cursor();
	terminal.disable_echoing();

	Roguelike rogue = Roguelike(&terminal);

	// terminal.waitkey(); // view compiling info

	// Init
	terminal.clear_screen();
	terminal.putstring(0, 0, "Press any key to start");

	// Draw
	setup_map(rogue);
	update_term(terminal);

	// exit
	terminal.show_cursor();
	terminal.disable_echoing(false);
}
