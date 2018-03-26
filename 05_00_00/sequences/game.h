#pragma once
#include <fstream>
#include <sstream>
#include "sequence.h"
#include "you_win.h"
#include "../state.h"
#include "../store.h"
#include "../image.h"

namespace caracal {
	namespace sequences {
		class game : public sequence {
		public:
			game(const char* stage_file_name);
			~game();
			sequence* execute(char c);
			unsigned* data() const;
			unsigned data_size() const;
			bool load_failed() const;
			bool is_goal() const;
			bool should_close() const;
		private:
			const unsigned MAX_CELL_SIZE = 16;
			const unsigned HEIGHT = 240;
			const unsigned WIDTH = 320;
			const enum COMMAND {
				UP,
				DOWN,
				RIGHT,
				LEFT,
				NONE
			};
			bool m_loaded;
			state* m_state;
			store* m_store;
			unsigned m_data_size;
			bool m_close_requested;
			char* load_stage(const char* data_dir, const char* file_name);
			std::map<char, image*> load_assets(const char* assets_dir);
			image* load_image(const char* assets_dir, const char* file_name);
			state::pos convert_cmd_to_delta(COMMAND cmd) const;
		};
	}
}