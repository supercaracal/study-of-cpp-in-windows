#pragma once
#include <sstream>
#include "GameLib/Framework.h"
#include "state.h"
#include "image.h"
namespace caracal {
	class game {
	public:
		game(std::istream& is, const std::string assets_path);
		virtual ~game();
		bool load_failed() const;
		void update();
		void draw();
		bool is_goal();
		unsigned calc_frame_rate();
		bool can_output_frame_rate();
		void wait_if_needed();
	private:
		const unsigned MAX_CELL_SIZE = 16;
		const unsigned FR_ADJUSTMENT_DELAY_TIME = 11;
		const enum COMMAND {
			UP,
			DOWN,
			RIGHT,
			LEFT,
			NONE
		};
		GameLib::Framework m_fw;
		bool m_loaded;
		state* m_state;
		unsigned m_proc_times[10];
		unsigned m_fr_output_delay_count;
		char* load_stage(std::istream& is);
		std::map<char, image*> load_assets(const std::string assets_path);
		image* load_image(const std::string assets_path, const char* file_name);
		state::pos convert_cmd_to_delta(COMMAND cmd) const;
	};
}