#pragma once
class image
{
public:
	image();
	~image();
	bool loaded() const;
	int height() const;
	int width() const;
	unsigned fetch(int y, int x) const;
private:
	const char* ENV_REPO_PATH = "MY_GAME_PROGRAMMER_BOOK_REPO";
	const char* IMG_FILE_PATH_POSTFIX = "/03_01_05/phase.dds";

	char* m_data;
	int m_height;
	int m_width;
	unsigned* m_img;
	bool m_loaded;
	unsigned extract_data(const char* p) const;
	bool valid_format() const;
};

