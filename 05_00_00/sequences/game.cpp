#include "game.h"

namespace caracal {
	namespace sequences {
		game::game(const char* stage_file_name) :
			m_loaded(false),
			m_data_size(HEIGHT * WIDTH),
			m_close_requested(false)
		{
			char* stg = load_stage(STAGE_DATA_DIR, stage_file_name);
			if (stg == NULL) {
				return;
			}
			std::map<char, image*> imgs = load_assets(ASSETS_DIR);
			m_state = new state(stg, imgs);
			m_store = new store(HEIGHT, WIDTH);
			delete[] stg;
			m_loaded = true;
		}

		game::~game() {
			delete m_state;
			delete m_store;
		}

		sequence* game::execute(char c) {
			COMMAND cmd;
			switch (c) {
			case 'w':
				cmd = COMMAND::UP;
				break;
			case 'z':
				cmd = COMMAND::DOWN;
				break;
			case 's':
				cmd = COMMAND::RIGHT;
				break;
			case 'a':
				cmd = COMMAND::LEFT;
				break;
			case 'q':
				m_close_requested = true;
				cmd = COMMAND::NONE;
				break;
			default:
				cmd = COMMAND::NONE;
				break;
			}
			state::pos delta = convert_cmd_to_delta(cmd);
			m_state->move(delta);
			m_state->update_data(m_store);

			if (is_goal()) {
				return new you_win();
			} else {
				return this;
			}
		}

		unsigned* game::data() const {
			return m_store->data();
		}

		unsigned game::data_size() const {
			return m_data_size;
		}

		bool game::is_goal() const {
			return m_state->is_goal();
		}

		bool game::should_close() const {
			return m_close_requested;
		}

		char* game::load_stage(const char* data_dir, const char* file_name) {
			std::stringstream ss;
			ss << data_dir << '/' << file_name;
			std::ifstream ifs(ss.str().data(), std::ifstream::in);
			if (ifs.fail()) return NULL;
			ifs.seekg(0, ifs.end);
			int length = static_cast<int>(ifs.tellg()) + 1;
			ifs.seekg(0, ifs.beg);
			char* buffer = new char[length];
			ifs.read(buffer, length);
			buffer[length - 1] = '\0';
			return buffer;
		}

		std::map<char, image*> game::load_assets(const char* assets_dir) {
			std::map<char, image*> imgs;
			imgs['o'] = load_image(assets_dir, "baggage.dds");
			imgs['O'] = load_image(assets_dir, "baggage_on_the_goal.dds");
			imgs['.'] = load_image(assets_dir, "goal.dds");
			imgs[' '] = load_image(assets_dir, "grass.dds");
			imgs['p'] = load_image(assets_dir, "player.dds");
			imgs['P'] = load_image(assets_dir, "player_on_the_goal.dds");
			imgs['#'] = load_image(assets_dir, "wall.dds");
			return imgs;
		}

		image* game::load_image(const char* assets_dir, const char* file_name) {
			std::stringstream ss;
			ss << assets_dir << '/' << file_name;
			return new image(ss.str().data(), MAX_CELL_SIZE, MAX_CELL_SIZE);
		}

		bool game::load_failed() const {
			return !m_loaded;
		}

		state::pos game::convert_cmd_to_delta(COMMAND cmd) const {
			state::pos delta = { 0, 0 };
			switch (cmd) {
			case COMMAND::UP:
				--delta.y;
				break;
			case COMMAND::DOWN:
				++delta.y;
				break;
			case COMMAND::LEFT:
				--delta.x;
				break;
			case COMMAND::RIGHT:
				++delta.x;
				break;
			default:
				break;
			}
			return delta;
		}
	}
}