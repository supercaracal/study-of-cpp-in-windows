#include "font.h"

namespace caracal {
	font::font(const char* path) {
		std::ifstream ifs(path, std::ifstream::binary);
		ifs.seekg(0, ifs.end);
		int size = static_cast<int>(ifs.tellg());
		ifs.seekg(0, ifs.beg);
		char* data = new char[size];
		ifs.read(data, size);
		ifs.close();
		if (!valid_format(data)) {
			m_loaded = false;
			return;
		}
		m_height = extract_data(data + 12);
		m_width = extract_data(data + 16);
		unsigned real_size = m_height * m_width;
		m_data = new unsigned[real_size];
		for (unsigned i = 0; i < real_size; ++i) {
			m_data[i] = extract_data(data + (128 + i * 4));
		}
		delete[] data;
		m_loaded = true;
	}

	font::~font()
	{
		delete[] m_data;
	}

	bool font::loaded() const
	{
		return m_loaded;
	}

	unsigned* font::fetch(char c) const
	{
		unsigned* d = new unsigned[100];
		unsigned num = static_cast<unsigned>(c);
		unsigned y = (num - 32) / 16;
		unsigned x = (num - 32) % 16;
		unsigned cur = 0;
		for (unsigned i = y * 10, max_y = y * 10 + 10; i < max_y; ++i) {
			for (unsigned j = x * 10, max_x = x * 10 + 10; j < max_x; ++j) {
				d[cur] = m_data[i * m_width + j];
				++cur;
			}
		}
		return d;
	}

	unsigned font::extract_data(const char* p) const
	{
		const unsigned char* up;
		up = reinterpret_cast<const unsigned char*>(p);
		unsigned ret = up[0];
		ret |= (up[1] << 8);
		ret |= (up[2] << 16);
		ret |= (up[3] << 24);
		return ret;
	}

	bool font::valid_format(const char* data) const
	{
		return data[0] == 'D' &&
			data[1] == 'D' &&
			data[2] == 'S';
	}
}