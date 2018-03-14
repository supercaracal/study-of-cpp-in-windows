#include <fstream>
#include "GameLib/Framework.h"
#include "game.h"

static const char* STAGE_DATA_PATH = "C:/Users/DIO/source/repos/study-of-cpp/04_05_01/data/stage.txt";
static const char* ASSETS_DIR = "C:/Users/DIO/source/repos/study-of-cpp/04_05_01/assets";

caracal::game* g = 0;
bool g_notified_goal = false;

namespace GameLib {
	void Framework::update() {
		if (isEndRequested()) {
			delete g;
			return;
		}

		if (g == NULL) {
			std::ifstream ifs(STAGE_DATA_PATH, std::ifstream::in);
			g = new caracal::game(ifs, ASSETS_DIR);
			if (g->load_failed()) {
				GameLib::cout << "Could not load stage or assets." << GameLib::endl;
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
			GameLib::cout << "GOAL!!" << GameLib::endl;
			g_notified_goal = true;
		}

		unsigned fr = g->calc_frame_rate();
		if (g->can_output_frame_rate()) {
			GameLib::cout << fr << GameLib::endl;
		}
		g->wait_if_needed();
	}
}