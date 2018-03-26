#pragma once
#include <sstream>
#include "../image.h"

namespace caracal {
	namespace sequences {
		class sequence {
		public:
			const char* STAGE_DATA_DIR = "C:/Users/DIO/source/repos/study-of-cpp/05_00_00/data";
			const char* ASSETS_DIR = "C:/Users/DIO/source/repos/study-of-cpp/05_00_00/assets";
			sequence();
			virtual ~sequence() {};
			virtual sequence* execute(char c) = 0;
			virtual unsigned* data() const = 0;
			virtual unsigned data_size() const = 0;
			virtual bool should_close() const;
		protected:
			image* load_image(const char* file_name, unsigned height, unsigned width) const;
			unsigned* build_data(image* img, unsigned height, unsigned width) const;
		};
	}
}