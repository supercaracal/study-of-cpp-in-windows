#pragma once
#include <fstream>

namespace caracal {
	class image
	{
	public:
		image(const char* path, unsigned max_height, unsigned max_width);
		~image();
		bool loaded() const;
		unsigned height() const;
		unsigned width() const;
		unsigned max_height() const;
		unsigned max_width() const;
		unsigned fetch(unsigned y, unsigned x) const;
	private:
		unsigned m_height;
		unsigned m_width;
		unsigned m_max_height;
		unsigned m_max_width;
		unsigned* m_data;
		bool m_loaded;
		unsigned extract_data(const char* p) const;
		bool valid_format(const char* data) const;
		bool out_of_image(unsigned y, unsigned x) const;
	};
}