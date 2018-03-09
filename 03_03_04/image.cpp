#include <fstream>
#include "image.h"

image::image(const char* path)
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
	unsigned dot_size = m_height * m_width;
	m_img = new unsigned[dot_size];
	for (unsigned i = 0; i < dot_size; ++i) {
		m_img[i] = extract_data(data + (128 + i * 4));
	}
	delete[] data;
	m_loaded = true;
}

image::image()
{
}

image::~image()
{
	delete[] m_img;
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

unsigned image::fetch(unsigned y, unsigned x) const
{
	if (y >= m_height || x >= m_width) {
		return m_img[0];
	}
	else {
		return m_img[y * m_width + x];
	}
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