#include "nocturnus/roguelike.hpp"
#include <csignal>
#include <cstdio>

struct Player {
	vec2<uint16> pos = { 5, 5 };
};

struct Enemy {
	vec2<uint16> pos = { 16, 1 };
};

Enemy enemy;

void draw(Terminal& terminal, Roguelike& rogue, Player& player) {
	terminal.clear_screen();

	uint8 width = 20;
	uint8 height = 10;
	for(int x = 0; x < width; x++) {
		for(int y = 0; y < height; y++) {
			if(x == 0 || x == width - 1 || y == 0 || y == height - 1) {
				rogue.putchar(x, y, '#');
			}
		}
	}

	terminal.set_bold(ANSIColor::YELLOW);
	terminal.putchar(player.pos.x, player.pos.y, 'C');

	terminal.set_bold(ANSIColor::RED);
	terminal.putchar(enemy.pos.x, enemy.pos.y, 'R');

	if(enemy.pos.x > player.pos.x) {
		enemy.pos.x -= 1;
	} else {
		enemy.pos.x += 1;
	}

	if(enemy.pos.y > player.pos.y) {
		enemy.pos.y -= 1;
	} else {
		enemy.pos.y += 1;
	}

	terminal.putstring(0, 20, ("X: " + std::to_string(player.pos.x)  + " Y: " + std::to_string(player.pos.y)).c_str());
	terminal.putstring(0, 21, ("Under character: '" + std::string(1, rogue.getchar(player.pos.x, player.pos.y)) + "'").c_str());
}

/**
 * BUGS
 * - Collision not working on windows (top and left only)
 * - Windows terminal blinking when moving
 *
 * NEW STUFF TO ADD
 * - Add real time turn (sleep on while loop end)
 * - Windows background
 * - Windows bold
 * 
 * 
 * */


int main() {
	// Terminal realted methods
	Terminal terminal = Terminal(); // In this case used as the underlayer
	terminal.hide_cursor();

	// Roguelike related methods
	Roguelike rogue = Roguelike(terminal, 50, 60); // Used for collision

	Player player;


	vec2 termsize = terminal.term_col_row();
	std::printf("Term size - rows: %d / columns: %d\n", termsize.x, termsize.y);

	terminal.clear_screen();

	terminal.set_color(ANSIColor::YELLOW);
	terminal.putstring(0, 0, "Press any key to start...");


	char ch;
	while((ch = terminal.getkey())) {
		vec2<uint16> newpos = player.pos;
		if(ch == 'w') {
			newpos.y -= 1;
		} else if(ch == 's') {
			newpos.y += 1;
		}

		if(ch == 'a') {
			newpos.x -= 1;
		} else if(ch == 'd') {
			newpos.x += 1;
		}

		// If not colliding with a wall
		if(!rogue.willcollide(newpos, '#')) {
			player.pos = newpos;
		}

		// Debug warning
		if(ch == 'e') {
			terminal.show_warning("Testing warning");
		}

		// Update screen
		draw(terminal, rogue, player);
	}
}


