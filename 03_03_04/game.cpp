#include "game.h"

namespace caracal {
	game::game() : m_stage_loaded(false), m_assets_loaded(false) {
	}

	game::game(const game& g) {
		m_stage_loaded = g.m_stage_loaded;
		m_assets_loaded = g.m_assets_loaded;
		m_state = g.m_state;
	}

	game& game::operator=(const game& g) {
		m_stage_loaded = g.m_stage_loaded;
		m_assets_loaded = g.m_assets_loaded;
		m_state = g.m_state;
		return *this;
	}

	game::~game() {
		delete m_img_baggage;
		delete m_img_baggage_on_the_goal;
		delete m_img_goal;
		delete m_img_grass;
		delete m_img_player;
		delete m_img_player_on_the_goal;
		delete m_img_wall;
	}

	void game::load_stage(std::istream& is) {
		if (m_stage_loaded || is.fail()) return;

		is.seekg(0, is.end);
		int length = static_cast<int>(is.tellg());
		is.seekg(0, is.beg);
		char* buffer = new char[length];
		is.read(buffer, length);
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
		return new image(ss.str().data());
	}

	bool game::load_failed() const {
		return !m_stage_loaded || !m_assets_loaded;
	}

	void game::update(char key) {
		m_state.move(key);
	}

	bool game::is_goal() {
		return m_state.is_goal();
	}

	void game::draw() {
		unsigned y, x;
		image* img;
		std::string stage = m_state.get();
		std::string::iterator it;

		for (it = stage.begin(), y = x = 0; it != stage.end(); ++it) {
			GameLib::cout << *it;
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
				img = m_img_player_on_the_goal;
				break;
			case ' ':
				img = m_img_grass;
				break;
			case 'o':
				img = m_img_baggage;
				break;
			case 'p':
				img = m_img_player;
				break;
			default:
				continue;
				break;
			}
			drawCell(y, x, img);
			++x;
		}
		GameLib::cout << GameLib::endl << "w:up z:down a:left s:right q:end" << GameLib::endl;
	}

	void game::drawCell(unsigned y, unsigned x, image* img) {
		GameLib::Framework fw = GameLib::Framework::instance();
		unsigned* vram = fw.videoMemory();

		unsigned windowHeight = fw.height();
		unsigned windowWidth = fw.width();

		if (img->height() > windowHeight || img->width() > windowWidth ||
			y * MAX_CELL_SIZE > windowHeight || x * MAX_CELL_SIZE > windowWidth) {

			throw RenderCellException();
		}

		unsigned dot;
		unsigned bg_dot;
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