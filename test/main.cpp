#include "nocturnus/terminal.hpp"
#include "player.hpp"

void setup_map(Terminal& terminal) {
	terminal.putchars(13, 13, "--------");
	terminal.putchars(13, 14, "|......|");
	terminal.putchars(13, 15, "|......|");
	terminal.putchars(13, 16, "|.......");
	terminal.putchars(13, 17, "|......|");
	terminal.putchars(13, 18, "--------");

	// Path
	terminal.putchars(21, 16, "..........");
	terminal.putchar(30, 15, '.');
	terminal.putchar(30, 14, '.');
	terminal.putchar(30, 13, '.');
	terminal.putchars(30, 12, "..........");

	terminal.putchars(40, 2, "--------");
	terminal.putchars(40, 3, "|......|");
	terminal.putchars(40, 4, "|......|");
	terminal.putchars(40, 5, "|......|");
	terminal.putchars(40, 6, "|......|");
	terminal.putchars(40, 7, "----.---");

	terminal.putchar(44, 8, '.');
	terminal.putchar(44, 9, '.');

	terminal.putchars(40, 10, "----.-------");
	terminal.putchars(40, 11, "|..........|");
	terminal.putchars(40, 12, "...........|");
	terminal.putchars(40, 13, "|..........|");
	terminal.putchars(40, 14, "|..........|");
	terminal.putchars(40, 15, "------------");
}

void update_term(Terminal& terminal) {
	// Put on roguelike module
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


		terminal.clear_line(0, 1);
		terminal.putstring(0, 1, ("New pos char: " + std::string(1, terminal.mvinch(newpos.x, newpos.y))).c_str());

		// Collision
		switch (terminal.mvinch(newpos.x, newpos.y)) {
			case '.':
				player.move(newpos);
				break;

			// Don't move if is not walkable
			default:
				break;
		}
		// player.move(newpos);

		// TODO -- Unify both
		terminal.clear_line(0, 0);
		terminal.putstring(0, 0, ("X: " + std::to_string(player.pos.x) + " Y: " + std::to_string(player.pos.y)).c_str());

		player.draw();
	}
}


int main() {
	// Setup
	Terminal terminal = Terminal();
	terminal.hide_cursor();
	terminal.disable_echoing();

	// Init
	terminal.clear_screen();
	terminal.putstring(0, 0, "Press any key to start");
	terminal.putstring(0, 2, ("Width: " + std::to_string(terminal.term_col()) + " Height: " + std::to_string(terminal.term_row())).c_str());


	// Draw
	setup_map(terminal);
	update_term(terminal);

	// exit
	terminal.show_cursor();
	terminal.disable_echoing(false);
}
