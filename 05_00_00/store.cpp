#include "store.h"

namespace caracal {
	store::store(unsigned height, unsigned width) :
		m_height(height),
		m_width(width),
		m_size(height * width)
	{
		m_data = new unsigned[m_size];
		clear();
	}

	store::~store() {
		delete[] m_data;
	}

	void store::update(unsigned y, unsigned x, unsigned value) {
		unsigned index = y * m_width + x;

		if (out_of_range(index)) {
			return;
		}
		m_data[index] = value;
	}

	unsigned* store::data() const {
		return m_data;
	}

	bool store::out_of_range(unsigned i) const {
		return i >= m_size;
	}

	void store::clear() {
		for (unsigned i = 0; i < m_size; ++i) {
			m_data[i] = 0;
		}
	}
}