#include "stage_selection.h"

namespace caracal {
	namespace sequences {
		stage_selection::stage_selection() : m_data_size(BG_HEIGHT * BG_WIDTH) {
			image* bg = load_image(BG_IMG_NAME, BG_HEIGHT, BG_WIDTH);
			m_data = build_data(bg, BG_HEIGHT, BG_WIDTH);
			delete bg;
		}

		stage_selection::~stage_selection() {
			delete m_data;
		}

		sequence* stage_selection::execute(char c) {
			sequence* s;
			switch (c) {
			case '1':
				s = new game("stage_easy.txt");
				break;
			case '2':
				s = new game("stage_normal.txt");
				break;
			case '3':
				s = new game("stage_hard.txt");
				break;
			default:
				s = this;
				break;
			}
			return s;
		}

		unsigned* stage_selection::data() const {
			return m_data;
		}

		unsigned stage_selection::data_size() const {
			return m_data_size;
		}
	}
}