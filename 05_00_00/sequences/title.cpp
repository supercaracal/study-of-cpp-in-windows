#include "title.h"

namespace caracal {
	namespace sequences {
		title::title() : m_data_size(BG_HEIGHT * BG_WIDTH) {
			image* bg = load_image(BG_IMG_NAME, BG_HEIGHT, BG_WIDTH);
			m_data = build_data(bg, BG_HEIGHT, BG_WIDTH);
			delete bg;
		}

		title::~title() {
			delete m_data;
		}

		sequence* title::execute(char c) {
			if (c == ' ') {
				return new stage_selection();
			} else {
				return this;
			}
		}

		unsigned* title::data() const {
			return m_data;
		}

		unsigned title::data_size() const {
			return m_data_size;
		}
	}
}