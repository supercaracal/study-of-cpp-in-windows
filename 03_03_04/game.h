#pragma once
#include <sstream>
#include "GameLib/Framework.h"
#include "state.h"
#include "image.h"
namespace caracal {
	class game {
	public:
		const unsigned MAX_CELL_SIZE = 16;
		struct RenderCellException : std::exception {
			const char* what() const noexcept {
				return "Could not render image data.";
			}
		};
		game();
		game(const game& g);
		game& operator=(const game& g);
		virtual ~game();
		void load_stage(std::istream& is);
		void load_assets(const std::string assets_path);
		bool load_failed() const;
		void update(char key);
		bool is_goal();
		void draw();
	private:
		bool m_stage_loaded;
		bool m_assets_loaded;
		state m_state;
		image* m_img_baggage;
		image* m_img_baggage_on_the_goal;
		image* m_img_goal;
		image* m_img_grass;
		image* m_img_player;
		image* m_img_player_on_the_goal;
		image* m_img_wall;
		image* load_image(const std::string assets_path, const char* file_name);
		void drawCell(unsigned y, unsigned x, image* img);
	};
}