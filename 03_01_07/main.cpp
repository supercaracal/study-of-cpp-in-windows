#include <fstream>
#include "game.h"
#include "GameLib/Framework.h"

static const char* STAGE_DATA_PATH = "C:/Users/DIO/source/repos/study-of-cpp/03_01_07/data/stage.txt";
static const char* ASSETS_DIR = "C:/Users/DIO/source/repos/study-of-cpp/03_01_07/assets";

static void initialize();
static void finish(std::string message);
static void die(std::string message);

caracal::game* g = 0;

namespace GameLib {
	void Framework::update() {
		if (g == NULL) {
			initialize();
			g->draw();
			return;
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

		g->draw();
	}
}

static void initialize() {
	g = new caracal::game();

	std::ifstream ifs(STAGE_DATA_PATH, std::ifstream::in);
	g->load_stage(ifs);
	g->load_assets(ASSETS_DIR);

	if (g->load_failed()) {
		die("Could not load stage or assets.");
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