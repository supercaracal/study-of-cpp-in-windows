#include "image.h"

namespace caracal {
	image::image(const char* path, unsigned max_height, unsigned max_width) :
		m_max_height(max_height),
		m_max_width(max_width)
	{
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

	image::~image()
	{
		delete[] m_data;
	}

	bool image::loaded() const
	{
		return m_loaded;
	}

	unsigned image::height() const
	{
		return m_height;
	}

	unsigned image::width() const
	{
		return m_width;
	}

	unsigned image::max_height() const
	{
		return m_max_height;
	}

	unsigned image::max_width() const
	{
		return m_max_width;
	}

	unsigned image::fetch(unsigned y, unsigned x) const
	{
		if (y >= m_max_height || x >= m_max_width) {
			return m_data[0];
		}
		if (m_height >= m_max_height && m_width >= m_max_width) {
			return m_data[y * m_width + x];
		}
		if (out_of_image(y, x)) {
			return m_data[0];
		}
		y -= (m_max_height - m_height) / 2;
		x -= (m_max_width - m_width) / 2;
		unsigned pos = (y * m_width + x >= m_height * m_width) ? 0 : y * m_width + x;
		return m_data[pos];
	}

	unsigned image::extract_data(const char* p) const
	{
		const unsigned char* up;
		up = reinterpret_cast<const unsigned char*>(p);
		unsigned ret = up[0];
		ret |= (up[1] << 8);
		ret |= (up[2] << 16);
		ret |= (up[3] << 24);
		return ret;
	}

	bool image::valid_format(const char* data) const
	{
		return data[0] == 'D' &&
			data[1] == 'D' &&
			data[2] == 'S';
	}

	bool image::out_of_image(unsigned y, unsigned x) const
	{
		bool out_of_y = m_height < m_max_height &&
			(y * 2 < (m_max_height - m_height)) ||
			((m_max_height - y) * 2 < (m_max_height - m_height));

		bool out_of_x = m_width < m_max_width &&
			(x * 2 < (m_max_width - m_width)) ||
			((m_max_width - x) * 2 < (m_max_width - m_width));

		return out_of_y || out_of_x;
	}
}