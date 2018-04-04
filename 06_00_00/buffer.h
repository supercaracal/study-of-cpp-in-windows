#pragma once

namespace caracal {
	class buffer {
	public:
		buffer(unsigned height, unsigned width);
		~buffer();
		void add(char c);
		char fetch(unsigned y, unsigned x) const;
	private:
		unsigned m_height;
		unsigned m_width;
		unsigned m_size;
		char* m_data;
		unsigned m_cur_idx;
	};
}