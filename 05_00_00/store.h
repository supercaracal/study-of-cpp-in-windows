#pragma once

namespace caracal {
	class store {
	public:
		store(unsigned height, unsigned width);
		~store();
		void update(unsigned y, unsigned x, unsigned value);
		unsigned* data() const;
	private:
		unsigned* m_data;
		unsigned m_height;
		unsigned m_width;
		unsigned m_size;
		bool out_of_range(unsigned i) const;
		void clear();
	};
}