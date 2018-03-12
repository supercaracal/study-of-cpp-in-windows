#pragma once
#include <queue>
#include <utility>
#include "image.h"
#include "Gamelib/Framework.h"

class animation
{
public:
	explicit animation(unsigned cell_size);
	~animation();
	void set_current(unsigned y, unsigned x);
	void add_destination(unsigned y, unsigned x);
	void draw(image* img);
private:
	GameLib::Framework m_fw;
	unsigned m_cell_size;
	std::queue<std::pair<unsigned, unsigned>> m_dests;
	std::pair<unsigned, unsigned> m_current;
};