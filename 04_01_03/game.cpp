#include "game.h"

namespace caracal {
	game::game(std::istream& is, const std::string assets_path) : m_loaded(false) {
		if (is.fail()) return;
		m_fw = GameLib::Framework::instance();
		char* stg = load_stage(is);
		std::map<char, image*> imgs = load_assets(assets_path);
		m_state = new state(stg, imgs);
		delete stg;
		m_loaded = true;
	}

	game::~game() {
		delete m_state;
	}

	char* game::load_stage(std::istream& is) {
		is.seekg(0, is.end);
		int length = static_cast<int>(is.tellg()) + 1;
		is.seekg(0, is.beg);
		char* buffer = new char[length];
		is.read(buffer, length);
		buffer[length - 1] = '\0';
		return buffer;
	}

	std::map<char, image*> game::load_assets(const std::string assets_path) {
		std::map<char, image*> imgs;
		imgs['o'] = load_image(assets_path, "baggage.dds");
		imgs['O'] = load_image(assets_path, "baggage_on_the_goal.dds");
		imgs['.'] = load_image(assets_path, "goal.dds");
		imgs[' '] = load_image(assets_path, "grass.dds");
		imgs['p'] = load_image(assets_path, "player.dds");
		imgs['P'] = load_image(assets_path, "player_on_the_goal.dds");
		imgs['#'] = load_image(assets_path, "wall.dds");
		return imgs;
	}

	image* game::load_image(const std::string assets_path, const char* file_name) {
		std::stringstream ss;
		ss << assets_path << '/' << file_name;
		return new image(ss.str().data(), MAX_CELL_SIZE);
	}

	bool game::load_failed() const {
		return !m_loaded;
	}

	void game::update() {
		COMMAND cmd;
		bool up = m_fw.isKeyOn('w');
		bool down = m_fw.isKeyOn('z');
		bool right = m_fw.isKeyOn('s');
		bool left = m_fw.isKeyOn('a');
		if (up && !m_last_cmd_up) {
			cmd = COMMAND::UP;
		} else if (down && !m_last_cmd_down) {
			cmd = COMMAND::DOWN;
		} else if (right && !m_last_cmd_right) {
			cmd = COMMAND::RIGHT;
		} else if (left && !m_last_cmd_left) {
			cmd = COMMAND::LEFT;
		} else {
			cmd = COMMAND::NONE;
		}
		state::pos delta = convert_cmd_to_delta(cmd);
		m_state->move(delta);
		m_last_cmd_up = up;
		m_last_cmd_down = down;
		m_last_cmd_right = right;
		m_last_cmd_left = left;
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

	void game::draw() {
		m_state->draw();
	}

	bool game::is_goal() {
		return m_state->is_goal();
	}
}