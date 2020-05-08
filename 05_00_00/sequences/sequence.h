#pragma once
#include <sstream>
#include "../image.h"

namespace caracal {
	namespace sequences {
		class sequence {
		public:
			const char* ENV_REPO_PATH = "MY_GAME_PROGRAMMER_BOOK_REPO";
			const char* STAGE_DATA_DIR = "/05_00_00/data";
			const char* ASSETS_DIR = "/05_00_00/assets";
			sequence();
			virtual ~sequence() {};
			virtual sequence* execute(char c) = 0;
			virtual unsigned* data() const = 0;
			virtual unsigned data_size() const = 0;
			virtual bool should_close() const;
		protected:
			char* repo_path;
			image* load_image(const char* file_name, unsigned height, unsigned width) const;
			unsigned* build_data(image* img, unsigned height, unsigned width) const;
		};
	}
}