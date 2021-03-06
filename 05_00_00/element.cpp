#include "element.h"

namespace caracal {
	element::element(char s, unsigned y, unsigned x) :
		m_sym(s), m_cell_y(y), m_cell_x(x),
		m_fg(NULL), m_bg(NULL),
		m_reserved_sym(s), m_reserved_fg(NULL), m_reserved_bg(NULL)
	{
	}

	element::~element() {
	}

	char element::sym() const {
		return m_sym;
	}

	image * element::get_fg() const {
		return m_fg;
	}

	image* element::get_bg() const {
		return m_bg;
	}

	void element::set_fg(image* img) {
		m_fg = img;
	}

	void element::set_bg(image* img) {
		m_bg = img;
	}

	unsigned element::cell_y() const {
		return m_cell_y;
	}

	unsigned element::cell_x() const {
		return m_cell_x;
	}

	bool element::can_move_to(element* dest) const {
		char s = dest->sym();

		if (is_player()) {
			return s != '#' && s != 'O';
		}
		else if (is_movable_baggage()) {
			return s != '#' && s != 'O' && s != 'o';
		}
		else {
			return false;
		}
	}

	bool element::is_linefeed() const {
		return m_sym == '\n';
	}

	bool element::is_movable_baggage() const {
		return m_sym == 'o';
	}

	bool element::does_try_to_become_movable_baggage() const {
		return m_sym != 'o' && m_reserved_sym == 'o';
	}

	bool element::is_player() const {
		return m_sym == 'p' || m_sym == 'P';
	}

	bool element::is_player_on_the_goal() const {
		return m_sym == 'P';
	}

	bool element::is_empty_goal() const {
		return m_sym == '.';
	}

	bool element::is_same_pos(unsigned y, unsigned x) const {
		return m_cell_y == y && m_cell_x == x;
	}

	bool element::is_same_wall(unsigned y, unsigned x) const {
		return m_sym == '#' && is_same_pos(y, x);
	}

	bool element::is_same_baggage(unsigned y, unsigned x) const {
		return (m_sym == 'o' || m_sym == 'O') && is_same_pos(y, x);
	}

	bool element::is_same_goal(unsigned y, unsigned x) const {
		return (m_sym == '.' || m_sym == 'O' || m_sym == 'P') && is_same_pos(y, x);
	}

	bool element::need_foreground() const {
		return m_sym == 'o' || m_sym == 'O' ||
			m_sym == 'p' || m_sym == 'P' ||
			m_sym == '.';
	}

	void element::reserve(char s, image* fg, image* bg) {
		m_reserved_sym = s;
		m_reserved_fg = fg;
		m_reserved_bg = bg;
	}

	void element::become() {
		if (m_reserved_sym == m_sym) {
			return;
		}

		m_sym = m_reserved_sym;
		m_fg = m_reserved_fg;
		m_bg = m_reserved_bg;
		m_reserved_fg = NULL;
		m_reserved_bg = NULL;
	}

	void element::update_forground(store* data) const {
		update_data(m_fg, data);
	}

	void element::update_background(store* data) const {
		update_data(m_bg, data);
	}

	void element::update_data(image* img, store* data) const {
		if (img == NULL) {
			return;
		}

		unsigned maxHeight = img->max_height();
		unsigned maxWidth = img->max_width();

		unsigned dot;
		int alpha;
		for (unsigned i = 0; i < maxHeight; ++i) {
			for (unsigned j = 0; j < maxWidth; ++j) {
				dot = img->fetch(i, j);
				alpha = (dot & 0xff000000) >> 24;
				if (alpha < 128) {
					continue;
				}
				data->update(m_cell_y * maxHeight + i, m_cell_x * maxWidth + j, dot);
			}
		}
	}
}