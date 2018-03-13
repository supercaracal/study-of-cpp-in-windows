#pragma once
#include <vector>
#include <map>
#include <algorithm>
#include "element.h"
#include "image.h"
namespace caracal {
	class state {
	public:
		struct pos { unsigned y; unsigned x; };
		state(char* stage, std::map<char, image*> images_each_sym);
		virtual ~state();
		bool operator() (const element* a, const element* b);
		bool move(pos delta);
		void draw();
		bool is_goal();
	private:
		std::vector<element*> m_elements;
		std::map<char, image*> m_images_each_sym;
		element* find_player();
		element* find_object(pos p);
		bool on_wall(pos p);
		bool on_baggage(pos p);
		bool on_goal(pos p);
		bool move_player(element* player, element* destination);
		bool move_baggage(element* baggage, element* destination);
	};
}