#pragma once

#include "nocturnus/typedef.hpp"
#include "nocturnus/vectors/vec2.hpp"
#include <functional>
#include <iostream>
#include <ostream>
#include <string>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif



// NOTE -- Terminal starts at 1, 1

#ifdef _WIN32
	enum class ANSIColor : uint8 {
		BLACK        = 0,
		DARK_RED     = 4, // Foreground = Esc[30m / Background = Esc[40
		DARK_GREEN   = 2,
		DARK_YELLOW  = 6,
		DARK_BLUE    = 1, 
		DARK_MAGENTA = 5,
		DARK_CYAN    = 3,
		DARK_WHITE   = 7, // Foreground = Esc[37m / Background = Esc[47m

		GRAY    = 8,  // Foreground = Esc[90m / Background = Esc[100m
		RED     = 12,
		GREEN   = 10,
		YELLOW  = 14,
		BLUE    = 9,
		MAGENTA = 13,
		CYAN    = 11,
		WHITE   = 15  // Foreground = Esc[97m / Background = Esc[107m


		// Bold         = Esc[1m
		// Underline    = Esc[4m
		// No underline = Esc[24m
		// Reverse text = Esc[7m
		// Not reversed = Esc[27m
	};

	enum class ANSIBackground : uint8 {
		BLACK        = 40,
		DARK_RED     = 41,
		DARK_GREEN   = 42,
		DARK_YELLOW  = 43,
		DARK_BLUE    = 44, 
		DARK_MAGENTA = 45,
		DARK_CYAN    = 46,
		DARK_WHITE   = 47,

		GRAY    = 100,
		RED     = 101,
		GREEN   = 102,
		YELLOW  = 103,
		BLUE    = 104,
		MAGENTA = 105,
		CYAN    = 106,
		WHITE   = 107  // Foreground = Esc[97m / Background = Esc[107m
	};
#else
	enum class ANSIColor {
		BLACK  = 30,
		DARK_RED     = 31,
		DARK_GREEN   = 32,
		DARK_YELLOW  = 33,
		DARK_BLUE    = 34,
		DARK_MAGENTA = 35,
		DARK_CYAN    = 36,
		DARK_WHITE   = 37,

		GRAY    = 90,
		RED     = 91,
		GREEN   = 92,
		YELLOW  = 93,
		BLUE    = 94,
		MAGENTA = 95,
		CYAN    = 96,
		WHITE   = 97,
	};

	enum class ANSIBackground : uint8 {
		BLACK  = 40,
		RED    = 41,
		GREEN  = 42,
		YELLOW = 43,
		BLUE   = 44,
		PURPLE = 45,
		CYAN   = 46,
		WHITE  = 47
	};
#endif


// NOTE -- change enum to char* later maybe
// color     \033[COLORm
// reset     \033[0m
// bold      \033[1;COLORm
// underline \033[4;COLORm

// struct TerminalConf {
// 	char* title = (char*)"Terminal";
// 	// std::function<void(Terminal&)> draw_func;
// };

struct Terminal {
	Terminal(const char* title = "Terminal");

	// inline void get_cursor_position() {
	// 	// std::cout << "\e[6n" << std::flush;
	// 	// \e[ROW;COLUMNR
	// }


	// SETTERS //

	// Changes the terminal title
	inline void set_title(const char* title) const {
	#ifdef _WIN32
		SetConsoleTitleA(title);
	#else
		std::cout << "\033]0;" << title << "\007" << std::flush;
	#endif
	}

	// META //

	// Hides the cursor
	inline void hide_cursor() const {
	#ifdef _WIN32
		HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursorinfo;
		GetConsoleCursorInfo(stdhandle, &cursorinfo); // Get current cursor size
		cursorinfo.bVisible = FALSE;
		SetConsoleCursorInfo(stdhandle, &cursorinfo);
	#else
		std::cout << "\033[?25l" << std::flush;
	#endif
	}

	// Shows the cursor
	inline void show_cursor() const {
	#ifdef _WIN32
		HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursorinfo;
		GetConsoleCursorInfo(stdhandle, &cursorinfo);
		cursorinfo.bVisible = TRUE;
		SetConsoleCursorInfo(stdhandle, &cursorinfo);
	#else
		std::cout << "\033[?25h" << std::flush;
	#endif
	}


	// Move cursor to some location on terminal.
	// This is used before drawing something on a location
	inline void move_cursor(const uint32 x, const uint32 y) const {
	#ifdef _WIN32
		COORD position = { (SHORT)x + 1, (SHORT)y + 1 };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	#else
		std::cout << "\033[" << y + 1 << ";" << x + 1 << "H";
	#endif
	}


	// CLEAR AND COLOR //

	// Clear the screen entirely
	inline void clear_screen() const {
	#ifdef _WIN32
		system("cls");
	#else
		// system("clear");
		std::cout << "\033[2J\033[1;1H" << std::flush;
	#endif
	}

	// Clear the line under the cursor
	inline void clear_line() const {
	#ifdef _WIN32
		HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);

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

	// Set color for next draw
	inline void set_color(const ANSIColor color) const {
	#ifdef _WIN32
		HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(stdhandle, &csbi);
		SetConsoleTextAttribute(stdhandle, (csbi.wAttributes & 0xFFF0) | (WORD)static_cast<uint8>(color));
	#else
		std::cout << "\033[0;" << static_cast<int>(color) << "m";
	#endif
	}

	// Set color for next draw
	inline void set_bold(const ANSIColor color) const {
	#ifdef _WIN32
		HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(stdhandle, &csbi);
		SetConsoleTextAttribute(stdhandle, (csbi.wAttributes & 0xFFF0) | (WORD)static_cast<uint8>(color));
	#else
		std::cout << "\033[1;" << static_cast<int>(color) << "m";
	#endif
	}

	// Set color for next draw
	inline void set_background(const ANSIBackground color) const {
	#ifdef _WIN32
		HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(stdhandle, &csbi);
		SetConsoleTextAttribute(stdhandle, (csbi.wAttributes & 0xFFF0) | (WORD)static_cast<uint8>(color));
	#else
		std::cout << "\033[" << static_cast<int>(color) << "m";
	#endif
	}


	// Reset the color for next draw
	inline void reset_color() const {
	#ifdef _WIN32
		HANDLE stdhandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(stdhandle, &csbi);
		SetConsoleTextAttribute(stdhandle, (csbi.wAttributes & 0xFFF0) | (WORD)7);
	#else
		std::cout << "\033[0;0m";
	#endif
	}


	// DRAWING //

	// Put a single char in some location
	virtual void putchar(const uint32 x, const uint32 y, const char ch);

	// Show a message in some location and wait for any key
	void show_message(const uint32 x, const uint32 y, const char* message) const;

	// Put a string in some location
	void putstring(const uint32 x, const uint32 y, const char* message) const ;

	// Wait for key without echoing and return it
	uint8 getkey() const;

	// Show a message at top that only hides if press space
	void show_warning(const std::string& message) const;

	vec2<uint16> term_col_row() const;
};


