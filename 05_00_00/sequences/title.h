#pragma once
#include "sequence.h"
#include "stage_selection.h"
#include "../image.h"

namespace caracal {
	namespace sequences {
		class title : public sequence {
		public:
			title();
			~title();
			sequence* execute(char c);
			unsigned* data() const;
			unsigned data_size() const;
		private:
			const char* BG_IMG_NAME = "title.dds";
			const unsigned BG_HEIGHT = 240;
			const unsigned BG_WIDTH = 320;
			unsigned* m_data;
			unsigned m_data_size;
		};
	}
}