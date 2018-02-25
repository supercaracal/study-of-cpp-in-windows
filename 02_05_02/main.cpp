#include <fstream>
#include "game.h"
#include "GameLib/Framework.h"

static void initialize();
static void draw();
static void drawCell(int y, int x, unsigned color);
static void finish(std::string message);
static void die(std::string message);

caracal::game* g = 0;

namespace GameLib {
	void Framework::update() {
		if (g == NULL) {
			initialize();
			draw();
		}

		if (g->is_goal()) {
			finish("GOAL!!");
		}

		char key;
		GameLib::cin >> key;
		if (key == 'q') {
			finish("");
		} else {
			g->update(key);
		}

		draw();
	}
}

static void initialize() {
	g = new caracal::game();

	std::ifstream ifs("C:/Users/DIO/source/repos/study-of-cpp/02_05_02/data.txt", std::ifstream::in);
	g->load_stage(ifs);

	if (g->load_failed()) {
		die("Could not load stage.");
	}
}

static void draw() {
	int y, x;
	unsigned color;
	std::string stage = g->get_stage();
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
			color = 0x0000ff;
			break;
		case '#':
			color = 0xffffff;
			break;
		case 'O':
			color = 0xff00ff;
			break;
		case 'P':
			color = 0x00ffff;
			break;
		case ' ':
			color = 0x000000;
			break;
		case 'o':
			color = 0xff0000;
			break;
		case 'p':
			color = 0x00ff00;
			break;
		}
		drawCell(y, x, color);
		++x;
	}
	GameLib::cout << GameLib::endl << "w:up z:down a:left s:right q:end" << GameLib::endl;
}

static void drawCell(int y, int x, unsigned color) {
	GameLib::Framework fw = GameLib::Framework::instance();
	unsigned* vram = fw.videoMemory();
	int windowWidth = fw.width();

	for (int i = 0; i < 16; ++i) {
		for (int j = 0; j < 16; ++j) {
			vram[(y * 16 + i) * windowWidth + (x * 16 + j)] = color;
		}
	}
}

static void finish(std::string message) {
	delete g;
	GameLib::cout << message.data() << GameLib::endl;
	exit(EXIT_SUCCESS);
}

static void die(std::string message) {
	delete g;
	GameLib::cout << message.data() << GameLib::endl;
	exit(EXIT_FAILURE);
}