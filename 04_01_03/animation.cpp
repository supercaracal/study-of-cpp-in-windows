#include "animation.h"

animation::animation(unsigned cell_size) : m_cell_size(cell_size)
{
	m_fw = GameLib::Framework::instance();
	m_current = std::pair<unsigned, unsigned>(0, 0);
}

animation::~animation()
{
}

void animation::set_current(unsigned y, unsigned x) {
	m_current = std::pair<unsigned, unsigned>(y * m_cell_size, x * m_cell_size);
}

void animation::add_destination(unsigned y, unsigned x) {
	m_dests.push(std::pair<unsigned, unsigned>(y * m_cell_size, x * m_cell_size));
}

void animation::draw(image* img) {
	unsigned c_y = std::get<0>(m_current);
	unsigned c_x = std::get<1>(m_current);
	unsigned d_y;
	unsigned d_x;

	if (m_dests.empty()) {
		d_y = c_y;
		d_x = c_x;
	} else {
		std::pair<unsigned, unsigned>& next = m_dests.front();
		d_y = std::get<0>(next);
		d_x = std::get<1>(next);
		if (d_y == c_y && d_x == c_x) {
			m_dests.pop();
			m_current = next;
		}
	}

	unsigned windowHeight = m_fw.height();
	unsigned windowWidth = m_fw.width();

	if (img->height() > windowHeight || img->width() > windowWidth ||
		c_y > windowHeight || c_x > windowWidth) {

		return;
	}

	unsigned dot;
	unsigned alpha;
	unsigned* vram = m_fw.videoMemory();

	for (unsigned i = 0; i < m_cell_size; ++i) {
		for (unsigned j = 0; j < m_cell_size; ++j) {
			dot = img->fetch(i, j);
			alpha = (dot & 0xff000000) >> 24;
			if (alpha < 128) {
				continue;
			}
			vram[(c_y + i) * windowWidth + (c_x + j)] = dot;
		}
	}

	unsigned delta_y;

	if (c_y < d_y) {
		delta_y = 1;
	} else if (c_y > d_y) {
		delta_y = -1;
	} else {
		delta_y = 0;
	}

	unsigned delta_x;

	if (c_x < d_x) {
		delta_x = 1;
	} else if (c_x > d_x) {
		delta_x = -1;
	} else {
		delta_x = 0;
	}

	m_current = std::pair<unsigned, unsigned>(c_y + delta_y, c_x + delta_x);
}