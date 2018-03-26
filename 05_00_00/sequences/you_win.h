#pragma once
#include "sequence.h"
#include "title.h"
#include "stage_selection.h"
#include "../image.h"

namespace caracal {
	namespace sequences {
		class you_win : public sequence {
		public:
			you_win();
			~you_win();
			sequence* execute(char c);
			unsigned* data() const;
			unsigned data_size() const;
			bool should_close() const;
		private:
			const char* BG_IMG_NAME = "you_win.dds";
			const unsigned BG_HEIGHT = 240;
			const unsigned BG_WIDTH = 320;
			unsigned* m_data;
			unsigned m_data_size;
			bool m_close_requested;
		};
	}
}