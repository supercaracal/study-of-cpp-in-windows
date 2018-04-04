#pragma once
#include <fstream>

namespace caracal {
	class font
	{
	public:
		font(const char* path);
		~font();
		bool loaded() const;
		unsigned* fetch(char c) const;
	private:
		unsigned m_height;
		unsigned m_width;
		unsigned* m_data;
		bool m_loaded;
		unsigned extract_data(const char* p) const;
		bool valid_format(const char* data) const;
	};
}