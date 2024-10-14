#pragma once

#include "nocturnus/terminal.hpp"
#include "nocturnus/typedef.hpp"
#include "nocturnus/vectors/vec2.hpp"

struct Entity {
	vec2<uint16> pos;
	char ch;
	ANSIColor color = ANSIColor::WHITE;

	// virtual void draw() const;
};
