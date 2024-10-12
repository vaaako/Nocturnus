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
	};

	// Bold         = Esc[1m
	// Underline    = Esc[4m
	// No underline = Esc[24m
	// Reverse text = Esc[7m
	// Not reversed = Esc[27m

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

// enum class Key : uint8 {
// 	A = 'a',
// 	a = 'a',
// 	B = 'B'
// };


// NOTE -- change enum to char* later maybe
// color     \033[COLORm
// reset     \033[0m
// bold      \033[1;COLORm
// underline \033[4;COLORm


struct Terminal {
	uint16 width;
	uint16 height;

	Terminal(const char* title = "Terminal");

	// SETTERS //

	// Changes the terminal title
	inline void set_title(const char* title) const {
	#ifdef _WIN32
		SetConsoleTitleA(title);
	#else
		std::cout << "\033]0;" << title << "\007" << std::flush;
	#endif
	}

	inline void set_width(const uint16 width) {
		this->width = width;
	}

	inline void set_height(const uint16 height) {
		this->height = height;
	}

	// META //

	// Get terminal width
	uint16 term_col() const;

	// Get terminal height
	uint16 term_row() const;

	inline bool inbounds(const uint16 x, const uint16 y) const {
		return (x < this->width && y < this->height);
	}

	// Disable echoing the key pressed
	void disable_echoing(const bool enable = true) const;

	// Hide cursor for being draw
	void hide_cursor() const;

	// Show cursor
	void show_cursor() const;

	// Move cursor to some location on terminal.
	// This is used before drawing something on a location
	void move_cursor(const uint16 x, const uint16 y) const;



	// CLEAR AND COLOR //

	// Clear the screen entirely
	void clear_screen() const;

	// Clear the line under the cursor
	void clear_line(const uint16 x, const uint16 y) const;

	// Set color for next draw
	void set_color(const ANSIColor color) const;

	// Set color for next draw
	void set_bold(const ANSIColor color) const;

	// Set color for next draw
	void set_background(const ANSIBackground color) const;

	// Reset the color for next draw
	void reset_color() const;


	// DRAWING //

	// Wait for key without echoing
	void waitkey() const;

	// Wait for key without echoing and return it
	uint8 getkey() const;

	// Put a single char in some location
	void putchar(const uint16 x, const uint16 y, const char ch);

	// Put a list of chars in a row.
	// This is not a HUD and characters are actually placed on buffer
	void putchars(const uint16 x, const uint16 y, const char* chars);

	// Get character in some location
	char mvinch(const uint16 x, const uint16 y) const;

	// Put a string in some location.
	// This is like a HUD and the string is not placed on the buffer
	void putstring(const uint16 x, const uint16 y, const char* message) const;

	// Show a message in some location and wait for any key
	void show_message(const uint16 x, const uint16 y, const char* message) const;



	// Show a message at top that only hides if press space
	void show_warning(const std::string& message) const;

	private:
		std::vector<std::vector<char>> term_buffer;
};


