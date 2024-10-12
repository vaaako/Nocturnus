# Nocturnus
Inspited by the early versions of [libtcod](https://github.com/libtcod/libtcod), **Nocturnus** is a library designed for Terminal-based drawing and Terminal-based Roguelike development. Its focus is to provide a simple way to render graphics in the terminal, supporting Unix and Windows

In conjuction with this library, I am also developing a "plugin" for Terminal-based Roguelike development. Inspired by [Nethack](https://nethackwiki.com/wiki/Main_Page) and [Rogue](https://en.wikipedia.org/wiki/Rogue_(video_game)). This combination aims to provide a toolkit for Roguelike development

# Compiling
## Linux
You can compile to a static library to use on your own project by running the following command:
```sh
make
```

If you want to test the example in [`test/`](test/), you can run the following:
```sh
make dev
```

## Windows or To Windows
For compiling to windows, you need to specify what compiler you want to use. This is an example of compiling to Windows from Linux using **Mingw**
```sh
make win CXX=i686-w64-mingw32-g+
```


# Basic usage example
```cpp
#include <nocturnus/roguelike.hpp>

void draw(const Terminal& terminal, const Roguelike& rogue, const Player& player) {
	terminal.clear_screen();

	// Make room with and set walls as '#'
	roguelike.make_room(0, 0, 20, 10, '#');

	// Draw player
	terminal.set_bold(ANSIColor::YELLOW);
	terminal.putchar(player.pos.x, player.pos.y, 'C'); // Using Terminal putchar because player is not part of the map
	// NOTE -- Any item or enemy, or interactive character is part of the map, so putchar from Roguelike must be used

	// Debug player position
	terminal.putstring(0, 20, ("X: " + std::to_string(player.pos.x)  + " Y: " + std::to_string(player.pos.y)).c_str());
}

int main() {
	// Terminal realted methods
	Terminal terminal = Terminal(); // In this case used as the underlayer
	terminal.hide_cursor();

	// Roguelike related methods
	Roguelike rogue = Roguelike(terminal, 50, 60); // Used for collision and other related methods

	// Player object
	Player player;

	terminal.clear_screen();
	terminal.set_color(ANSIColor::YELLOW);
	terminal.putstring(0, 0, "Press any key to start..."); // Show a message without waiting for a key
	// terminal.show_message(x, y, message); // Show a message and waits for a key

	// Will only update if any key was pressed
	char ch;
	while((ch = terminal.getkey())) {
		// Track new collision
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

		// If next movement will not collide, update position
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
```
