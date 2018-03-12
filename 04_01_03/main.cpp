#include <fstream>
#include "game.h"
#include "GameLib/Framework.h"

static const char* STAGE_DATA_PATH = "C:/Users/DIO/source/repos/study-of-cpp/04_01_03/data/stage.txt";
static const char* ASSETS_DIR = "C:/Users/DIO/source/repos/study-of-cpp/04_01_03/assets";

static void initialize();
static void notify(std::string message);

caracal::game* g = 0;
bool g_notified_goal = false;

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

		if (isKeyOn('q')) {
			requestEnd();
			return;
		} else {
			g->update();
		}

		g->draw();

		if (g->is_goal() && !g_notified_goal) {
			notify("GOAL!!");
			g_notified_goal = true;
		}
	}
}

static void initialize() {
	g = new caracal::game();
	std::ifstream ifs(STAGE_DATA_PATH, std::ifstream::in);
	g->load_stage(ifs);
	g->load_assets(ASSETS_DIR);
	g->initialize_animations();
}

static void notify(std::string message) {
	GameLib::cout << message.data() << GameLib::endl;
}