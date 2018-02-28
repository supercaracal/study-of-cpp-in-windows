#include <fstream>
#include "image.h"

image::image(const char* path)
{
	std::ifstream ifs(path, std::ifstream::binary);
	ifs.seekg(0, ifs.end);
	int size = static_cast<int>(ifs.tellg());
	ifs.seekg(0, ifs.beg);
	m_data = new char[size];
	ifs.read(m_data, size);
	ifs.close();
	if (!valid_format()) {
		m_loaded = false;
		return;
	}
	m_height = extract_data(&m_data[12]);
	m_width = extract_data(&m_data[16]);
	unsigned dot_size = m_height * m_width;
	m_img = new unsigned[dot_size];
	for (unsigned i = 0; i < dot_size; ++i) {
		m_img[i] = extract_data(&m_data[128 + i * 4]);
	}
	m_loaded = true;
}

image::image()
{
}

image::~image()
{
	delete m_data;
	delete m_img;
}

bool image::loaded() const
{
	return m_loaded;
}

int image::height() const
{
	return m_height;
}

int image::width() const
{
	return m_width;
}

unsigned image::fetch(int y, int x) const
{
	return m_img[y * m_width + x];
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

bool image::valid_format() const
{
	return m_data[0] == 'D' &&
		m_data[1] == 'D' &&
		m_data[2] == 'S';
}