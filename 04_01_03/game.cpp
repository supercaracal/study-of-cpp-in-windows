#include "game.h"

namespace caracal {
	game::game() : m_stage_loaded(false), m_assets_loaded(false) {
		m_fw = GameLib::Framework::instance();
	}

	game::~game() {
		delete m_img_baggage;
		delete m_img_baggage_on_the_goal;
		delete m_img_goal;
		delete m_img_grass;
		delete m_img_player;
		delete m_img_player_on_the_goal;
		delete m_img_wall;
		delete m_anime_player;
	}

	void game::load_stage(std::istream& is) {
		if (m_stage_loaded || is.fail()) return;

		is.seekg(0, is.end);
		int length = static_cast<int>(is.tellg()) + 1;
		is.seekg(0, is.beg);
		char* buffer = new char[length];
		is.read(buffer, length);
		buffer[length - 1] = '\0';
		m_state.set(buffer);
		m_stage_loaded = true;
		delete buffer;
	}

	void game::load_assets(const std::string assets_path) {
		if (m_assets_loaded) return;

		// TODO: Use enum
		m_img_baggage = load_image(assets_path, "baggage.dds");
		if (!m_img_baggage->loaded()) return;
		m_img_baggage_on_the_goal = load_image(assets_path, "baggage_on_the_goal.dds");
		if (!m_img_baggage_on_the_goal->loaded()) return;
		m_img_goal = load_image(assets_path, "goal.dds");
		if (!m_img_goal->loaded()) return;
		m_img_grass = load_image(assets_path, "grass.dds");
		if (!m_img_grass->loaded()) return;
		m_img_player = load_image(assets_path, "player.dds");
		if (!m_img_player->loaded()) return;
		m_img_player_on_the_goal = load_image(assets_path, "player_on_the_goal.dds");
		if (!m_img_player_on_the_goal->loaded()) return;
		m_img_wall = load_image(assets_path, "wall.dds");
		if (!m_img_wall->loaded()) return;
		m_assets_loaded = true;
	}

	image* game::load_image(const std::string assets_path, const char* file_name) {
		std::stringstream ss;
		ss << assets_path << '/' << file_name;
		return new image(ss.str().data(), MAX_CELL_SIZE);
	}

	void game::initialize_animations() {
		m_anime_player = new animation(MAX_CELL_SIZE);
		state::pos pp = m_state.get_player_pos();
		m_anime_player->set_current(pp.y, pp.x);
	}

	bool game::load_failed() const {
		return !m_stage_loaded || !m_assets_loaded;
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
		if (m_state.move(delta)) {
			state::pos pp = m_state.get_player_pos();
			m_anime_player->add_destination(pp.y, pp.x);
		}
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

	bool game::is_goal() {
		return m_state.is_goal();
	}

	void game::draw() {
		unsigned y, x;
		image* img;
		image* animated_player_img = NULL;
		std::string stage = m_state.get();
		std::string::iterator it;

		for (it = stage.begin(), y = x = 0; it != stage.end(); ++it) {
			if (*it == '\n') {
				++y;
				x = 0;
				continue;
			}
			switch (*it) {
			case '.':
				img = m_img_goal;
				break;
			case '#':
				img = m_img_wall;
				break;
			case 'O':
				img = m_img_baggage_on_the_goal;
				break;
			case 'P':
				img = m_img_grass;
				animated_player_img = m_img_player_on_the_goal;
				break;
			case ' ':
				img = m_img_grass;
				break;
			case 'o':
				img = m_img_baggage;
				break;
			case 'p':
				img = m_img_grass;
				animated_player_img = m_img_player;
				break;
			default:
				continue;
				break;
			}
			drawCell(y, x, img);
			++x;
		}

		if (animated_player_img != NULL) {
			m_anime_player->draw(animated_player_img);
		}
	}

	void game::drawCell(unsigned y, unsigned x, image* img) {
		unsigned* vram = m_fw.videoMemory();

		unsigned windowHeight = m_fw.height();
		unsigned windowWidth = m_fw.width();

		if (img->height() > windowHeight || img->width() > windowWidth ||
			y * MAX_CELL_SIZE > windowHeight || x * MAX_CELL_SIZE > windowWidth) {

			throw RenderCellException();
		}

		unsigned dot;
		int alpha;
		for (unsigned i = 0; i < MAX_CELL_SIZE; ++i) {
			for (unsigned j = 0; j < MAX_CELL_SIZE; ++j) {
				dot = img->fetch(i, j);
				alpha = (dot & 0xff000000) >> 24;
				if (alpha < 128) {
					dot = m_img_grass->fetch(i, j);
				}
				vram[(y * MAX_CELL_SIZE + i) * windowWidth + (x * MAX_CELL_SIZE + j)] = dot;
			}
		}
	}
}