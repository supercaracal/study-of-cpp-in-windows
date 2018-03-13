#pragma once
#include <exception>
#include "GameLib/Framework.h"
#include "image.h"
class element
{
public:
	struct RenderCellException : std::exception {
		const char* what() const noexcept {
			return "Could not render image data.";
		}
	};
	element(char s, unsigned u, unsigned x);
	~element();
	char sym() const;
	image* get_fg_img() const;
	image* get_bg_img() const;
	void set_fg_img(image* img);
	void set_bg_img(image* img);
	unsigned cell_y() const;
	unsigned cell_x() const;
	bool cannot_move() const;
	bool is_linefeed() const;
	bool need_foreground() const;
	void become(char s, image* fg_img, image* bg_img);
	void draw_forground();
	void draw_background();
private:
	GameLib::Framework m_fw;
	char m_sym;
	unsigned m_cell_y;
	unsigned m_cell_x;
	image* m_fg_img;
	image* m_bg_img;
	void draw(image* img);
};