#pragma once
#include <vector>
#include <tuple>
#include <algorithm>
#include <sstream>
#include <string>
#include <utility>
namespace caracal {
	class state {
	public:
		struct pos { int y; int x; };
		state();
		state(const state& s);
		virtual ~state();
		state& operator=(const state& s);
		bool operator() (const std::tuple<char, int, int> a,
			const std::tuple<char, int, int> b);
		void set(char* data);
		std::string get();
		bool move(pos delta);
		pos get_player_pos();
		bool is_goal();
	private:
		std::vector<std::tuple<char, int, int>> m_objects;
		std::tuple<char, int, int>& find_player();
		std::tuple<char, int, int>& find_object(pos p);
		bool on_wall(pos p);
		bool on_baggage(pos p);
		bool on_goal(pos p);
		bool move_player(std::tuple<char, int, int>& player,
			std::tuple<char, int, int>& destination);
		bool move_baggage(std::tuple<char, int, int>& baggage,
			std::tuple<char, int, int>& destination);
	};
}