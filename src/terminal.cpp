#include "nocturnus/terminal.hpp"
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

Terminal::Terminal(const char* title) {
	this->set_title(title);
}

vec2<uint16> Terminal::term_col_row() const {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return {
		(csbi.srWindow.Bottom - csbi.srWindow.Top) + 1,
		(csbi.srWindow.Right - csbi.srWindow.Left) + 1
	};
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return { w.ws_col, w.ws_row };
#endif
}


uint8 Terminal::getkey() const {
#ifdef _WIN32
	return _getch(); // Windows: Read a single character without echoing
#else
	// For Linux, termios can be used
	static struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt); // Save old terminal
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	uint8 ch = getchar();

	// Restore old terminal settings
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	return ch;
#endif
}




// Put a single char in some location
void Terminal::putchar(const uint32 x, const uint32 y, const char ch) {
	this->move_cursor(x, y);
	std::cout << ch << std::flush;
	this->reset_color();
}

// Show a message in some location and wait for any key
void Terminal::show_message(const uint32 x, const uint32 y, const char* message) const {
	this->move_cursor(x, y);
	std::cout << message << std::flush;
	this->reset_color();
	this->getkey();
}

// Put a string in some location
void Terminal::putstring(const uint32 x, const uint32 y, const char* message) const {
	this->move_cursor(x, y);
	std::cout << message << std::flush;
	this->reset_color();
}


void Terminal::show_warning(const std::string& message) const {
#ifdef _WIN32
	this->putstring(0, 0, (message + " --MORE--").c_str());

	while (_getch() != ' ') {
		// loop until space is pressed
	}
#else
	// For Linux, termios can be used
	static struct termios oldt, newt;
	tcgetattr(STDIN_FILENO, &oldt); // Save old terminal
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echo
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	this->putstring(0, 0, (message + " --MORE--").c_str());

	char ch;
	do {
		ch = getchar();
	} while (ch != ' '); // Wait until press space

	// Flush to prevent extra new line
	tcflush(STDIN_FILENO, TCIFLUSH);

	// Restore old terminal
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#endif

	// Hide warning
	this->move_cursor(0, 0);
	this->clear_line();
}
