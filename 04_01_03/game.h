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
	private:
		const unsigned MAX_CELL_SIZE = 16;
		const enum class COMMAND {
			UP,
			DOWN,
			RIGHT,
			LEFT,
			NONE
		};
		GameLib::Framework m_fw;
		bool m_loaded;
		bool m_last_cmd_up;
		bool m_last_cmd_down;
		bool m_last_cmd_right;
		bool m_last_cmd_left;
		state* m_state;
		char* load_stage(std::istream& is);
		std::map<char, image*> load_assets(const std::string assets_path);
		image* load_image(const std::string assets_path, const char* file_name);
		state::pos convert_cmd_to_delta(COMMAND cmd) const;
	};
}