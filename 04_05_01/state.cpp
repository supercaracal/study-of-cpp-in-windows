#include "state.h"

namespace caracal {
	state::state(char* stage, std::map<char, image*> images_each_sym) :
		m_images_each_sym(images_each_sym)
	{
		char* p;
		int y;
		int x;
		for (p = stage, y = 0, x = 0; *p != '\0'; ++p) {
			element* e = new element(*p, y, x);
			if (e->need_foreground()) {
				e->set_fg(m_images_each_sym[*p]);
				e->set_bg(m_images_each_sym[' ']);
			} else {
				e->set_fg(NULL);
				e->set_bg(m_images_each_sym[*p]);
			}
			m_elements.push_back(e);
			++x;
			if (e->is_linefeed()) {
				++y;
				x = 0;
			}
		}
	}

	state::~state() {
		std::vector<element*>::iterator it;
		element* elm;
		for (it = m_elements.begin(); it != m_elements.end(); ++it) {
			elm = static_cast<element*>(*it);
			delete elm;
		}
		animation* ani;
		for (int i = 0, q_size = m_animations.size(); i < q_size; ++i) {
			ani = m_animations.front();
			delete ani;
		}
		std::map<char, image*>::iterator itt;
		image* img;
		for (itt = m_images_each_sym.begin(); itt != m_images_each_sym.end(); ++itt) {
			img = static_cast<image*>(itt->second);
			delete img;
		}
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
		animation* ani;
		for (int i = 0, q_size = m_animations.size(); i < q_size; ++i) {
			ani = m_animations.front();
			m_animations.push(ani);
			m_animations.pop();
			if (ani->is_movable_baggage_at_finish()) {
				return false;
			}
		}
		std::vector<element*>::iterator it;
		element* elm;
		for (it = m_elements.begin(); it != m_elements.end(); ++it) {
			elm = static_cast<element*>(*it);
			if (elm->is_movable_baggage()) {
				return false;
			}
		}
		return true;
	}

	void state::draw() {
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

		animation* ani;
		for (int i = 0, q_size = m_animations.size(); i < q_size; ++i) {
			ani = m_animations.front();
			m_animations.pop();
			if (ani->is_finish()) {
				ani->after_finish();
				delete ani;
			} else {
				ani->draw();
				m_animations.push(ani);
			}
		}
	}

	element* state::find_player() {
		std::vector<element*>::iterator it;
		element* elm = NULL;
		for (it = m_elements.begin(); it != m_elements.end(); ++it) {
			elm = static_cast<element *>(*it);
			if (elm->is_player()) {
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
			if (elm->is_same_pos(p.y, p.x)) {
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
			if (elm->is_same_wall(p.y, p.x)) {
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
			if (elm->is_same_baggage(p.y, p.x)) {
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
			if (elm->is_same_goal(p.y, p.x)) {
				return true;
			}
		}
		return false;
	}

	bool state::move_player(element* player, element* destination) {
		if (!player->can_move_to(destination)) {
			return false;
		}

		if (destination->is_empty_goal()) {
			destination->reserve('P', m_images_each_sym['P'], m_images_each_sym[' ']);
		} else {
			destination->reserve('p', m_images_each_sym['p'], m_images_each_sym[' ']);
		}
		animation* ani = new animation(player, destination);
		m_animations.push(ani);

		if (player->is_player_on_the_goal()) {
			player->reserve('.', m_images_each_sym['.'], m_images_each_sym[' ']);
		}
		else {
			player->reserve(' ', NULL, m_images_each_sym[' ']);
		}
		player->become();

		return true;
	}

	bool state::move_baggage(element* baggage, element* destination) {
		if (!baggage->can_move_to(destination)) {
			return false;
		}

		if (destination->is_empty_goal()) {
			destination->reserve('O', m_images_each_sym['O'], m_images_each_sym[' ']);
		} else {
			destination->reserve('o', m_images_each_sym['o'], m_images_each_sym[' ']);
		}
		animation* ani = new animation(baggage, destination);
		m_animations.push(ani);

		baggage->reserve(' ', NULL, m_images_each_sym[' ']);
		baggage->become();

		return true;
	}
}