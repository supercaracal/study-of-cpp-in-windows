#include "element.h"

element::element(char s, unsigned y, unsigned x) :
	m_sym(s), m_cell_y(y), m_cell_x(x)
{
	m_fw = GameLib::Framework::instance();
}

element::~element()
{
	delete m_fg_img;
	delete m_bg_img;
}

char element::sym() const
{
	return m_sym;
}

image * element::get_fg_img() const
{
	return m_fg_img;
}

image* element::get_bg_img() const
{
	return m_bg_img;
}

void element::set_fg_img(image* img)
{
	m_fg_img = img;
}

void element::set_bg_img(image* img)
{
	m_bg_img = img;
}

unsigned element::cell_y() const
{
	return m_cell_y;
}

unsigned element::cell_x() const
{
	return m_cell_x;
}

bool element::cannot_move() const
{
	return m_sym == '#' || m_sym == 'O';
}

bool element::is_linefeed() const
{
	return m_sym == '\n';
}

bool element::need_foreground() const
{
	return m_sym == 'o' || m_sym == 'O' ||
		m_sym == 'p' || m_sym == 'P' || m_sym == '.';
}

void element::become(char s, image* fg_img, image* bg_img)
{
	m_sym = s;
	m_fg_img = fg_img;
	m_bg_img = bg_img;
}

void element::draw_forground()
{
	draw(m_fg_img);
}

void element::draw_background()
{
	draw(m_bg_img);
}

void element::draw(image* img)
{
	if (img == NULL) {
		return;
	}

	unsigned* vram = m_fw.videoMemory();

	unsigned windowHeight = m_fw.height();
	unsigned windowWidth = m_fw.width();

	unsigned maxHeight = img->max_height();
	unsigned maxWidth = img->max_width();

	if (img->height() > windowHeight ||
		img->width() > windowWidth ||
		m_cell_y * maxHeight > windowHeight ||
		m_cell_x * maxWidth > windowWidth) {

		throw RenderCellException();
	}

	unsigned dot;
	unsigned idx;
	int alpha;
	for (unsigned i = 0; i < maxHeight; ++i) {
		for (unsigned j = 0; j < maxWidth; ++j) {
			dot = img->fetch(i, j);
			alpha = (dot & 0xff000000) >> 24;
			if (alpha < 128) {
				continue;
			}
			idx = (m_cell_y * maxHeight + i) * windowWidth + (m_cell_x * maxWidth + j);
			vram[idx] = dot;
		}
	}
}