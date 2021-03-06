#pragma once
#include "image.h"
#include "element.h"
#include "store.h"

namespace caracal {
	class animation
	{
	public:
		animation(element* source, element* destination);
		~animation();
		bool is_movable_baggage_at_finish() const;
		bool is_finish() const;
		void after_finish();
		void update_data(store* data);
	private:
		unsigned m_c_y;
		unsigned m_c_x;
		unsigned m_d_y;
		unsigned m_d_x;
		image* m_img;
		element* m_dest;
	};
}