#pragma once
#include "sequence.h"
#include "game.h"
#include "../image.h"

namespace caracal {
	namespace sequences {
		class stage_selection : public sequence {
		public:
			stage_selection();
			~stage_selection();
			sequence* execute(char c);
			unsigned* data() const;
			unsigned data_size() const;
		private:
			const char* BG_IMG_NAME = "select_stage.dds";
			const unsigned BG_HEIGHT = 240;
			const unsigned BG_WIDTH = 320;
			unsigned* m_data;
			unsigned m_data_size;
		};
	}
}