#include "animation.h"

animation::animation(element* source, element* destination)
{
	m_fw = GameLib::Framework::instance();
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
	delete m_img;
	delete m_dest;
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

void animation::draw() {
	if (is_finish()) {
		return;
	}

	unsigned windowHeight = m_fw.height();
	unsigned windowWidth = m_fw.width();

	if (m_img->height() > windowHeight || m_img->width() > windowWidth ||
		m_c_y > windowHeight || m_c_x > windowWidth) {

		m_d_y = m_c_y;
		m_d_x = m_c_x;
		return;
	}

	unsigned dot;
	unsigned alpha;
	unsigned* vram = m_fw.videoMemory();
	unsigned max_cell_height = m_img->max_height();
	unsigned max_cell_width = m_img->max_width();

	for (unsigned i = 0; i < max_cell_height; ++i) {
		for (unsigned j = 0; j < max_cell_width; ++j) {
			dot = m_img->fetch(i, j);
			alpha = (dot & 0xff000000) >> 24;
			if (alpha < 128) {
				continue;
			}
			vram[(m_c_y + i) * windowWidth + (m_c_x + j)] = dot;
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