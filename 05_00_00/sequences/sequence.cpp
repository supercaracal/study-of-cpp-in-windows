#include "sequence.h"

namespace caracal {
	namespace sequences {
		sequence::sequence() : m_fw(GameLib::Framework::instance()) {
		}

		bool sequence::should_close() const {
			return false;
		}

		image* sequence::load_image(const char* file_name, unsigned height, unsigned width) const {
			std::stringstream ss;
			ss << ASSETS_DIR << '/' << file_name;
			return new image(ss.str().data(), height, width);
		}

		unsigned* sequence::build_data(image* img, unsigned height, unsigned width) const {
			unsigned* d = new unsigned[height * width];
			for (unsigned i = 0; i < height; ++i) {
				for (unsigned j = 0; j < width; ++j) {
					d[i * width + j] = img->fetch(i, j);
				}
			}
			return d;
		}
	}
}