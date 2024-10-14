#include "nocturnus/terminal.hpp"
#include <cstring>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

Terminal::Terminal(const char* title) : width(this->term_col()), height(this->term_row()) {
	this->buffer = new uint8[this->width * this->height];
	this->set_title(title);
}

Terminal::~Terminal() {
	delete this->buffer;
}

// META

uint16 Terminal::term_col() const {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return static_cast<uint16>((csbi.srWindow.Right - csbi.srWindow.Left) + 1);
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_col;
#endif
}

uint16 Terminal::term_row() const {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	return static_cast<uint16>((csbi.srWindow.Bottom - csbi.srWindow.Top) + 1);
#else
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
	return w.ws_row;
#endif
}

void Terminal::disable_echoing(const bool enable) const {
#ifdef _WIN32
	return;
#else
	struct termios term;
	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~(ICANON | ECHO); // Disable line buffering and echo

	if(enable) {
		term.c_lflag &= ~(ICANON | ECHO);
	} else {
		term.c_lflag |= (ICANON | ECHO);
	}

	tcsetattr(STDIN_FILENO, TCSANOW, &term);
#endif
}


// Hides the cursor
void Terminal::hide_cursor() const {
#ifdef _WIN32
	static HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorinfo;

	// Get the current cursor info first
	if(!GetConsoleCursorInfo(stdhandle, &cursorinfo)) {
		return;
	}

	cursorinfo.bVisible = FALSE;
	SetConsoleCursorInfo(stdhandle, &cursorinfo);
#else
	std::cout << "\033[?25l" << std::flush;
#endif
}

// Shows the cursor
void Terminal::show_cursor() const {
#ifdef _WIN32
	static HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorinfo;

	// Get the current cursor info first
	if(!GetConsoleCursorInfo(stdhandle, &cursorinfo)) {
		return;
	}

	cursorinfo.bVisible = TRUE;
	SetConsoleCursorInfo(stdhandle, &cursorinfo);
#else
	std::cout << "\033[?25h" << std::flush;
#endif
}



// CLEAR

void Terminal::clear_screen() const {
#ifdef _WIN32
	static const HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	const COORD topleft = {0, 0};
	DWORD charswritten;

	// Clear buffer
	std::cout.flush();

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if(!GetConsoleScreenBufferInfo(stdhandle, &csbi)) {
		return;
	}

	const DWORD length = csbi.dwSize.X * csbi.dwSize.Y;

	// Spam space
	FillConsoleOutputCharacter(stdhandle, (TCHAR)' ', length, topleft, &charswritten);

	// Reset every char (clear background)
	FillConsoleOutputAttribute(stdhandle, csbi.wAttributes, length, topleft, &charswritten);

	// Move cursor back to origin
	SetConsoleCursorPosition(stdhandle, topleft);
#else
	std::cout << "\033[2J\033[1;1H" << std::flush;
#endif
}

void Terminal::clear_line(const uint16 x, const uint16 y) const {
	this->move_cursor(x, y);

#ifdef _WIN32
	static const HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Get console screen
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(stdhandle, &csbi)) {
		return; // Error, ignore
	}

	// Get nuumber of columns
	DWORD chars_written;
	COORD startPos = csbi.dwCursorPosition;
	startPos.X = 0; // Start clearing from the line beginning

	// Fill the line with spaces to clear it
	FillConsoleOutputCharacter(stdhandle, ' ', csbi.dwSize.X, startPos, &chars_written);

	// Set the cursor back to the start of the line
	SetConsoleCursorPosition(stdhandle, startPos);
#else
	std::cout << "\033[2K" << std::flush;
#endif
}

void Terminal::move_cursor(const uint16 x, const uint16 y) const {
	if(!this->inbounds(x, y)) {
		return;
	}

#ifdef _WIN32
	static const HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();

	SetConsoleCursorPosition(stdhandle, { (SHORT)x, (SHORT)y });
#else
	std::cout << "\033[" << y + 1 << ";" << x + 1 << "H"; // Linux terminal init offscreen
#endif
}


// COLORS

void Terminal::set_color(const ANSIColor color) const {
#ifdef _WIN32
	static const HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(stdhandle, &csbi);
	SetConsoleTextAttribute(stdhandle, (csbi.wAttributes & 0xFFF0) | (WORD)static_cast<uint8>(color));
#else
	std::cout << "\033[0;" << static_cast<int>(color) << "m";
#endif
}


void Terminal::set_bold(const ANSIColor color) const {
#ifdef _WIN32
	HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(stdhandle, &csbi);
	SetConsoleTextAttribute(stdhandle, (csbi.wAttributes & 0xFFF0) | (WORD)static_cast<uint8>(color));
#else
	std::cout << "\033[1;" << static_cast<int>(color) << "m";
#endif
}


void Terminal::set_background(const ANSIBackground color) const {
#ifdef _WIN32
	HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(stdhandle, &csbi);
	SetConsoleTextAttribute(stdhandle, (csbi.wAttributes & 0xFFF0) | (WORD)static_cast<uint8>(color));
#else
	std::cout << "\033[" << static_cast<int>(color) << "m";
#endif
}


void Terminal::reset_color() const {
#ifdef _WIN32

	static const HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(stdhandle, &csbi);
	SetConsoleTextAttribute(stdhandle, (csbi.wAttributes & 0xFFF0) | (WORD)7);
#else
	std::cout << "\033[0;0m";
#endif
}



// DRAWING

void Terminal::waitkey() const {
#ifdef _WIN32
	_getch(); // Read character without echoing
#else
	// Assuming echoing is disabled
	getchar();
#endif
}

uint8 Terminal::getkey() const {
#ifdef _WIN32
	return _getch(); // Read character without echoing
#else
	// Assuming echoing is disabled
	uint8 ch = getchar();
	return ch;
#endif
}


// Put a single char in some location
void Terminal::putchar(const uint16 x, const uint16 y, const char ch) {
	if(this->inbounds(x, y)) {
		this->buffer[x+y*this->width] = ch;
		this->move_cursor(x, y);
		std::cout << ch;
	}
	// this->reset_color();
}

void Terminal::putchars(const uint16 x, const uint16 y, const char* chars) {
	int mlength = std::strlen(chars);

	for(int i = 0; i < mlength; i++) {
		int nx = x + i;
		this->putchar(nx, y, chars[i]);
	}
}

char Terminal::mvinch(const uint16 x, const uint16 y) const {
	return this->buffer[x + y * this->width];
}


// Show a message in some location and wait for any key
void Terminal::show_message(const uint16 x, const uint16 y, const char* message) const {
	this->move_cursor(x, y);
	std::cout << message << std::flush;
	// this->reset_color();
	this->waitkey();
}

// Put a string in some location
void Terminal::putstring(const uint16 x, const uint16 y, const char* message) const {
	this->move_cursor(x, y);
	this->clear_line(x, y); // Clear whatever is in this line before
	std::cout << message << std::flush;
	// this->reset_color();
}


void Terminal::show_warning(const std::string& message) const {
#ifdef _WIN32
	this->putstring(0, 0, (message + " --MORE--").c_str());

	while(_getch() != ' ') {
		// loop until space is pressed
	}
#else
	// Assuming echoing is disabled
	this->putstring(0, 0, (message + " --MORE--").c_str());

	char ch;
	do {
		ch = getchar();
	} while (ch != ' '); // Wait until space is pressed
#endif

	// Hide warning
	this->clear_line(0, 0);
}
