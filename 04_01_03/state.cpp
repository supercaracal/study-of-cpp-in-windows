#include "state.h"

namespace caracal {
	state::state(char* stage, std::map<char, image*> images_each_sym) : m_images_each_sym(images_each_sym) {
		char* p;
		int y;
		int x;
		for (p = stage, y = 0, x = 0; *p != '\0'; ++p) {
			element* e = new element(*p, y, x);
			if (e->need_foreground()) {
				e->set_fg_img(m_images_each_sym[*p]);
				e->set_bg_img(m_images_each_sym[' ']);
			} else {
				e->set_fg_img(NULL);
				e->set_bg_img(m_images_each_sym[*p]);
			}
			m_elements.push_back(e);
			++x;
			if (*p == '\n') {
				++y;
				x = 0;
			}
		}
	}

	state::~state() {
	}

	bool state::operator() (const element* a, const element* b) {
		return (a->cell_y() < b->cell_y())
			|| (a->cell_y() == b->cell_y() && a->cell_x() < b->cell_x());
	}

	bool state::move(pos delta) {
		if (delta.y == 0 && delta.x == 0) {
			return false;
		}
		element* player = find_player();
		pos expected_pp = { player->cell_y() + delta.y,
			player->cell_x() + delta.x };

		if (on_wall(expected_pp)) {
			return false;
		}

		if (on_baggage(expected_pp)) {
			element* baggage = find_object(expected_pp);
			pos expected_bp = { baggage->cell_y() + delta.y,
				baggage->cell_x() + delta.x };
			element* dest_b = find_object(expected_bp);
			bool moved = move_baggage(baggage, dest_b);
			if (!moved) return false;
		}

		element* dest_p = find_object(expected_pp);
		return move_player(player, dest_p);
	}

	bool state::is_goal() {
		std::vector<element*>::iterator it;
		element* elm;
		for (it = m_elements.begin(); it != m_elements.end(); ++it) {
			elm = static_cast<element*>(*it);
			if (elm->sym() == 'o') {
				return false;
			}
		}
		return true;
	}

	void state::draw() {
		std::sort(m_elements.begin(), m_elements.end(), *this);
		unsigned y, x;
		std::vector<element*>::iterator it;
		element* elm;

		for (it = m_elements.begin(), y = x = 0; it != m_elements.end(); ++it) {
			elm = static_cast<element*>(*it);
			if (elm->is_linefeed()) {
				++y;
				x = 0;
				continue;
			}
			elm->draw_background();
			++x;
		}

		for (it = m_elements.begin(), y = x = 0; it != m_elements.end(); ++it) {
			elm = static_cast<element*>(*it);
			if (elm->is_linefeed()) {
				++y;
				x = 0;
				continue;
			}
			elm->draw_forground();
			++x;
		}
	}

	element* state::find_player() {
		std::vector<element*>::iterator it;
		element* elm = NULL;
		for (it = m_elements.begin(); it != m_elements.end(); ++it) {
			elm = static_cast<element *>(*it);
			if (elm->sym() == 'p' || elm->sym() == 'P') {
				break;
			}
		}
		return elm;
	}

	element* state::find_object(pos p) {
		std::vector<element*>::iterator it;
		element* elm = NULL;
		for (it = m_elements.begin(); it != m_elements.end(); ++it) {
			elm = static_cast<element *>(*it);
			if (elm->cell_y() == p.y && elm->cell_x() == p.x) {
				break;
			}
		}
		return elm;
	}

	bool state::on_wall(pos p) {
		std::vector<element *>::iterator it;
		element* elm;
		for (it = m_elements.begin(); it != m_elements.end(); ++it) {
			elm = static_cast<element *>(*it);
			if (elm->sym() == '#' && elm->cell_y() == p.y
				&& elm->cell_x() == p.x) {
				return true;
			}
		}
		return false;
	}

	bool state::on_baggage(pos p) {
		std::vector<element*>::iterator it;
		element* elm;
		for (it = m_elements.begin(); it != m_elements.end(); ++it) {
			elm = static_cast<element*>(*it);
			if ((elm->sym() == 'o' || elm->sym() == 'O')
				&& elm->cell_y() == p.y && elm->cell_x() == p.x) {
				return true;
			}
		}
		return false;
	}

	bool state::on_goal(pos p) {
		std::vector<element*>::iterator it;
		element* elm;
		for (it = m_elements.begin(); it != m_elements.end(); ++it) {
			elm = static_cast<element*>(*it);
			if ((elm->sym() == '.' || elm->sym() == 'O' || elm->sym() == 'P')
				&& elm->cell_y() == p.y && elm->cell_x() == p.x) {
				return true;
			}
		}
		return false;
	}

	bool state::move_player(element* player, element* destination) {
		if (destination->cannot_move()) {
			return false;
		}
		char player_sym = player->sym();
		char dest_sym = destination->sym();
		if (player_sym == 'P') {
			player->become('.', NULL, m_images_each_sym['.']);
		}
		else {
			player->become(' ', NULL, m_images_each_sym[' ']);
		}

		if (dest_sym == '.') {
			destination->become('P', m_images_each_sym['P'], m_images_each_sym[' ']);
		}
		else {
			destination->become('p', m_images_each_sym['p'], m_images_each_sym[' ']);
		}

		return true;
	}

	bool state::move_baggage(element* baggage, element* destination) {
		if (baggage->cannot_move() || destination->cannot_move()) {
			return false;
		}

		if (destination->sym() == '.') {
			destination->become('O', m_images_each_sym['O'], m_images_each_sym[' ']);
		}
		else {
			destination->become('o', m_images_each_sym['o'], m_images_each_sym[' ']);
		}

		return true;
	}
}