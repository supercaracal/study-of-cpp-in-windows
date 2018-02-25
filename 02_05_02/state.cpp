#include "state.h"

namespace caracal {
	state::state() {
	}

	state::state(const state& s) {
	}

	state::~state() {
	}

	state& state::operator=(const state& s) {
		return *this;
	}

	bool state::operator() (const std::tuple<char, int, int> a,
		const std::tuple<char, int, int> b) {
		return (std::get<1>(a) < std::get<1>(b))
			|| (std::get<1>(a) == std::get<1>(b) && std::get<2>(a) < std::get<2>(b));
	}

	void state::set(char* data) {
		char* p;
		int y;
		int x;
		for (p = data, y = 0, x = 0; *p != '\0'; ++p) {
			m_objects.push_back(std::make_tuple(*p, y, x));
			++x;
			if (*p == '\n') {
				++y;
				x = 0;
			}
		}
	}

	std::string state::get() {
		std::sort(m_objects.begin(), m_objects.end(), *this);
		std::stringstream ss;
		std::vector<std::tuple<char, int, int>>::iterator it;
		for (it = m_objects.begin(); it != m_objects.end(); ++it) {
			ss << std::get<0>(*it);
		}
		return ss.str();
	}

	void state::move(char key) {
		pos delta = convert_key_to_delta(key);
		std::tuple<char, int, int>& player = find_player();
		pos expected_pp = { std::get<1>(player) + delta.y,
			std::get<2>(player) + delta.x };

		if (on_wall(expected_pp)) {
			return;
		}

		if (on_baggage(expected_pp)) {
			std::tuple<char, int, int>& baggage = find_object(expected_pp);
			pos expected_bp = { std::get<1>(baggage) + delta.y,
				std::get<2>(baggage) + delta.x };
			std::tuple<char, int, int>& dest_b = find_object(expected_bp);
			bool moved = move_baggage(baggage, dest_b);
			if (!moved) return;
		}

		std::tuple<char, int, int>& dest_p = find_object(expected_pp);
		move_player(player, dest_p);
	}

	bool state::is_goal() {
		std::vector<std::tuple<char, int, int>>::iterator it;
		for (it = m_objects.begin(); it != m_objects.end(); ++it) {
			if (std::get<0>(*it) == 'o') {
				return false;
			}
		}
		return true;
	}

	state::pos state::convert_key_to_delta(char key) {
		pos delta = { 0, 0 };
		switch (key) {
		case 'w':
			--delta.y;
			break;
		case 'z':
			++delta.y;
			break;
		case 'a':
			--delta.x;
			break;
		case 's':
			++delta.x;
			break;
		}
		return delta;
	}

	std::tuple<char, int, int>& state::find_player() {
		std::vector<std::tuple<char, int, int>>::iterator it;
		for (it = m_objects.begin(); it != m_objects.end(); ++it) {
			if (std::get<0>(*it) == 'p' || std::get<0>(*it) == 'P') {
				break;
			}
		}
		return *it;
	}

	std::tuple<char, int, int>& state::find_object(pos p) {
		std::vector<std::tuple<char, int, int>>::iterator it;
		for (it = m_objects.begin(); it != m_objects.end(); ++it) {
			if (std::get<1>(*it) == p.y && std::get<2>(*it) == p.x) {
				break;
			}
		}
		return *it;
	}

	bool state::on_wall(pos p) {
		std::vector<std::tuple<char, int, int>>::iterator it;
		for (it = m_objects.begin(); it != m_objects.end(); ++it) {
			if (std::get<0>(*it) == '#' && std::get<1>(*it) == p.y
				&& std::get<2>(*it) == p.x) {
				return true;
			}
		}
		return false;
	}

	bool state::on_baggage(pos p) {
		std::vector<std::tuple<char, int, int>>::iterator it;
		for (it = m_objects.begin(); it != m_objects.end(); ++it) {
			if ((std::get<0>(*it) == 'o' || std::get<0>(*it) == 'O')
				&& std::get<1>(*it) == p.y && std::get<2>(*it) == p.x) {
				return true;
			}
		}
		return false;
	}

	bool state::on_goal(pos p) {
		std::vector<std::tuple<char, int, int>>::iterator it;
		for (it = m_objects.begin(); it != m_objects.end(); ++it) {
			if ((std::get<0>(*it) == '.' || std::get<0>(*it) == 'O'
				|| std::get<0>(*it) == 'P')
				&& std::get<1>(*it) == p.y && std::get<2>(*it) == p.x) {
				return true;
			}
		}
		return false;
	}

	void state::move_player(std::tuple<char, int, int>& player,
		std::tuple<char, int, int>& destination) {
		char& player_sym = std::get<0>(player);
		char& dest_sym = std::get<0>(destination);
		if (dest_sym == '#' || dest_sym == 'O') {
			return;
		}
		if (player_sym == 'P') {
			player_sym = '.';
		}
		else {
			player_sym = ' ';
		}
		if (dest_sym == '.') {
			dest_sym = 'P';
		}
		else {
			dest_sym = 'p';
		}
	}

	bool state::move_baggage(std::tuple<char, int, int>& baggage,
		std::tuple<char, int, int>& destination) {
		char& baggage_sym = std::get<0>(baggage);
		char& dest_sym = std::get<0>(destination);
		if (dest_sym == '#' || dest_sym == 'O' || baggage_sym == 'O') {
			return false;
		}
		if (dest_sym == '.') {
			dest_sym = 'O';
		}
		else {
			dest_sym = 'o';
		}
		return true;
	}
}