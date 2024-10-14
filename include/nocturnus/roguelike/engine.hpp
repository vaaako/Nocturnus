#pragma once

// Just a base to what i want to do

#include "nocturnus/roguelike/entity.hpp"
#include <vector>

// https://www.roguebasin.com/index.php/Complete_roguelike_tutorial_using_C%2B%2B_and_libtcod_-_part_2:_map_and_actors
class Engine {
	public:
		Engine();
		~Engine();
		void update();
		void render();

	private:
		// Pointer so all childs is acceptable
		std::vector<Entity*> entities;
		Entity player;
};
