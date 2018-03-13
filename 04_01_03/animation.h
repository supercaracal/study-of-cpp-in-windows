#pragma once
#include "image.h"
#include "element.h"
#include "Gamelib/Framework.h"

class animation
{
public:
	animation(element* source, element* destination, char s, image* fg, image* bg);
	~animation();
	bool is_finish() const;
	void after_finish();
	void draw();
private:
	GameLib::Framework m_fw;
	unsigned m_c_y;
	unsigned m_c_x;
	unsigned m_d_y;
	unsigned m_d_x;
	image* m_img;
	element* m_dest;
	char m_dest_sym;
	image* m_dest_fg;
	image* m_dest_bg;
};