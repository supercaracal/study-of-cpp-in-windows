#include "buffer.h"

namespace caracal {
	buffer::buffer(unsigned height, unsigned width) :
		m_height(height),
		m_width(width),
		m_size(height * width),
		m_cur_idx(0)
	{
		m_data = new char[height * width + 1];
		for (unsigned i = 0; i < m_size; ++i) {
			m_data[i] = ' ';
		}
		m_data[m_size] = '\0';
	}

	buffer::~buffer()
	{
		delete[] m_data;
	}

	void buffer::add(char c) {
		if (m_cur_idx == m_size) {
			m_cur_idx = 0;
		}

		m_data[m_cur_idx] = c;
		++m_cur_idx;
	}

	char buffer::fetch(unsigned y, unsigned x) const {
		unsigned i = y * m_width + x;

		if (i >= m_size) {
			return ' ';
		}

		return m_data[i];
	}
}