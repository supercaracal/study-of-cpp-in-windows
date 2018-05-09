#pragma once
class image
{
public:
	image(const char* path);
	~image();
	bool loaded() const;
	int height() const;
	int width() const;
	unsigned fetch(int y, int x) const;
private:
	image();
	int m_height;
	int m_width;
	unsigned* m_img;
	bool m_loaded;
	unsigned extract_data(const char* p) const;
	bool valid_format(const char* data) const;
};

