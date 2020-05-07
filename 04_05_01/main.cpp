#include <fstream>
#include "GameLib/Framework.h"
#include "game.h"

static const char* ENV_REPO_PATH = "MY_GAME_PROGRAMMER_BOOK_REPO";
static const char* STAGE_DATA_PATH = "/04_05_01/data/stage.txt";
static const char* ASSETS_DIR = "/04_05_01/assets";

caracal::game* g = 0;
bool g_notified_goal = false;

static bool initialize(void);

namespace GameLib {
	void Framework::update() {
		if (isEndRequested()) {
			delete g;
			return;
		}

		if (g == NULL) {
			bool ok = initialize();
			if (!ok) {
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

static bool initialize() {
	size_t len;
	getenv_s(&len, NULL, 0, ENV_REPO_PATH);
	if (len == 0) {
		return false;
	}

	char* p = (char*)malloc(sizeof(char) * len);
	getenv_s(&len, p, len, ENV_REPO_PATH);
	std::string s = p;
	free(p);

	std::ifstream ifs(s + STAGE_DATA_PATH, std::ifstream::in);
	g = new caracal::game(ifs, s + ASSETS_DIR);
	if (g->load_failed()) {
		return false;
	}

	return true;
}