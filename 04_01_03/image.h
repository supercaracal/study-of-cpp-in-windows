#pragma once
class image
{
public:
	image(const char* path, unsigned max_size);
	~image();
	bool loaded() const;
	unsigned height() const;
	unsigned width() const;
	unsigned fetch(unsigned y, unsigned x) const;
private:
	image();
	unsigned m_height;
	unsigned m_width;
	unsigned m_max_size;
	unsigned* m_img;
	bool m_loaded;
	unsigned extract_data(const char* p) const;
	bool valid_format(const char* data) const;
	bool out_of_image(unsigned y, unsigned x) const;
};