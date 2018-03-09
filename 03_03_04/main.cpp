#include <fstream>
#include "game.h"
#include "GameLib/Framework.h"

static const char* STAGE_DATA_PATH = "C:/Users/DIO/source/repos/study-of-cpp/03_03_04/data/stage.txt";
static const char* ASSETS_DIR = "C:/Users/DIO/source/repos/study-of-cpp/03_03_04/assets";

static void initialize();
static void notify(std::string message);

caracal::game* g = 0;

namespace GameLib {
	void Framework::update() {
		if (isEndRequested()) {
			delete g;
			return;
		}

		if (g == NULL) {
			initialize();
			if (g->load_failed()) {
				notify("Could not load stage or assets.");
				requestEnd();
				return;
			}
			g->draw();
			return;
		}

		char key;
		GameLib::cin >> key;

		if (key == 'q') {
			requestEnd();
			return;
		} else {
			g->update(key);
		}

		g->draw();

		if (g->is_goal()) {
			notify("GOAL!!");
		}
	}
}

static void initialize() {
	g = new caracal::game();
	std::ifstream ifs(STAGE_DATA_PATH, std::ifstream::in);
	g->load_stage(ifs);
	g->load_assets(ASSETS_DIR);
}

static void notify(std::string message) {
	GameLib::cout << message.data() << GameLib::endl;
}