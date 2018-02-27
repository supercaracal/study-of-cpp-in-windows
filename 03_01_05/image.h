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
	char* m_data;
	int m_height;
	int m_width;
	bool m_loaded;
	unsigned extract_data(const char* p) const;
	bool valid_format() const;
};

