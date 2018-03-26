#include "you_win.h"

namespace caracal {
	namespace sequences {
		you_win::you_win() :
			m_data_size(BG_HEIGHT * BG_WIDTH),
			m_close_requested(false)
		{
			image* bg = load_image(BG_IMG_NAME, BG_HEIGHT, BG_WIDTH);
			m_data = build_data(bg, BG_HEIGHT, BG_WIDTH);
			delete bg;
		}

		you_win::~you_win() {
			delete m_data;
		}

		sequence* you_win::execute(char c) {
			sequence* s = this;
			switch (c) {
			case '1':
				s = new title();
				break;
			case '2':
				s = new stage_selection();
				break;
			case '3':
				m_close_requested = true;
				break;
			}
			return s;
		}

		unsigned* you_win::data() const {
			return m_data;
		}

		unsigned you_win::data_size() const {
			return m_data_size;
		}

		bool you_win::should_close() const {
			return m_close_requested;
		}
	}
}