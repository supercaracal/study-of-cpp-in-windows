#pragma once
#include <sstream>
#include "GameLib/Framework.h"
#include "state.h"
#include "image.h"
#include "animation.h"
namespace caracal {
	class game {
	public:
		struct RenderCellException : std::exception {
			const char* what() const noexcept {
				return "Could not render image data.";
			}
		};
		game();
		virtual ~game();
		void load_stage(std::istream& is);
		void load_assets(const std::string assets_path);
		bool load_failed() const;
		void initialize_animations();
		void update();
		bool is_goal();
		void draw();
	private:
		const unsigned MAX_CELL_SIZE = 16;
		const enum COMMAND {
			UP,
			DOWN,
			RIGHT,
			LEFT,
			NONE
		};
		bool m_stage_loaded;
		bool m_assets_loaded;
		GameLib::Framework m_fw;
		bool m_last_cmd_up;
		bool m_last_cmd_down;
		bool m_last_cmd_right;
		bool m_last_cmd_left;
		state m_state;
		image* m_img_baggage;
		image* m_img_baggage_on_the_goal;
		image* m_img_goal;
		image* m_img_grass;
		image* m_img_player;
		image* m_img_player_on_the_goal;
		image* m_img_wall;
		animation* m_anime_player;
		image* load_image(const std::string assets_path, const char* file_name);
		state::pos convert_cmd_to_delta(COMMAND cmd) const;
		void drawCell(unsigned y, unsigned x, image* img);
	};
}