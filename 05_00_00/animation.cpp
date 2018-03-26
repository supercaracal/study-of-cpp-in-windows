#include "animation.h"

namespace caracal {
	animation::animation(element* source, element* destination)
	{
		m_img = source->get_fg();
		unsigned h = m_img->max_height();
		unsigned w = m_img->max_width();
		m_c_y = source->cell_y() * h;
		m_c_x = source->cell_x() * w;
		m_d_y = destination->cell_y() * h;
		m_d_x = destination->cell_x() * w;
		m_dest = destination;
	}

	animation::~animation()
	{
	}

	bool animation::is_movable_baggage_at_finish() const
	{
		return m_dest->does_try_to_become_movable_baggage();
	}

	bool animation::is_finish() const
	{
		return m_d_y == m_c_y && m_d_x == m_c_x;
	}

	void animation::after_finish()
	{
		m_dest->become();
	}

	void animation::update_data(store* data) {
		if (is_finish()) {
			return;
		}

		unsigned dot;
		unsigned alpha;
		unsigned max_cell_height = m_img->max_height();
		unsigned max_cell_width = m_img->max_width();

		for (unsigned i = 0; i < max_cell_height; ++i) {
			for (unsigned j = 0; j < max_cell_width; ++j) {
				dot = m_img->fetch(i, j);
				alpha = (dot & 0xff000000) >> 24;
				if (alpha < 128) {
					continue;
				}
				data->update(m_c_y + i, m_c_x + j, dot);
			}
		}

		unsigned delta_y;

		if (m_c_y < m_d_y) {
			delta_y = 1;
		} else if (m_c_y > m_d_y) {
			delta_y = -1;
		} else {
			delta_y = 0;
		}

		unsigned delta_x;

		if (m_c_x < m_d_x) {
			delta_x = 1;
		} else if (m_c_x > m_d_x) {
			delta_x = -1;
		} else {
			delta_x = 0;
		}

		m_c_y += delta_y;
		m_c_x += delta_x;
	}
}