#include "game.h"

namespace caracal {
	game::game(std::istream& is, const std::string assets_path) :
		m_loaded(false), m_fr_output_delay_count(0)
	{
		if (is.fail()) return;
		m_fw = GameLib::Framework::instance();
		char* stg = load_stage(is);
		std::map<char, image*> imgs = load_assets(assets_path);
		m_state = new state(stg, imgs);
		delete[] stg;
		m_loaded = true;
		for (unsigned i = 0; i < 10; ++i) {
			m_proc_times[i] = m_fw.time();
		}
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
		if (m_fw.isKeyOn('w')) {
			cmd = COMMAND::UP;
		} else if (m_fw.isKeyOn('z')) {
			cmd = COMMAND::DOWN;
		} else if (m_fw.isKeyOn('s')) {
			cmd = COMMAND::RIGHT;
		} else if (m_fw.isKeyOn('a')) {
			cmd = COMMAND::LEFT;
		} else {
			cmd = COMMAND::NONE;
		}
		state::pos delta = convert_cmd_to_delta(cmd);
		m_state->move(delta);
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

	unsigned game::calc_frame_rate() {
		unsigned cur_t = m_fw.time();
		unsigned prev_t = m_proc_times[0];
		unsigned diff = cur_t - prev_t;
		for (unsigned i = 0, max = 10 - 1; i < max; ++i) {
			m_proc_times[i] = m_proc_times[i + 1];
		}
		m_proc_times[10 - 1] = cur_t;
		return 1000 * 10 / diff;
	}

	bool game::can_output_frame_rate() {
		if (++m_fr_output_delay_count > 100) {
			m_fr_output_delay_count = 0;
			return true;
		} else {
			return false;
		}
	}

	void game::wait_if_needed() {
		while ((m_fw.time() - m_proc_times[10 - 1]) < FR_ADJUSTMENT_DELAY_TIME) {
			m_fw.sleep(1);
		}
	}
}