#include <fstream>
#include "GameLib/Framework.h"
#include "game.h"

static const char* ENV_REPO_PATH = "MY_GAME_PROGRAMMER_BOOK_REPO";
static const char* STAGE_DATA_PATH = "/04_01_03/data/stage.txt";
static const char* ASSETS_DIR = "/04_01_03/assets";

static void notify(std::string message);
static bool initialize(void);

caracal::game* g = 0;
bool g_notified_goal = false;

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
			notify("GOAL!!");
			g_notified_goal = true;
		}
	}
}

static void notify(std::string message) {
	GameLib::cout << message.data() << GameLib::endl;
}

static bool initialize() {
	size_t len;
	getenv_s(&len, NULL, 0, ENV_REPO_PATH);
	if (len == 0) {
		notify("Could not find repository path via environment variable.");
		return false;
	}

	char* p = (char*)malloc(sizeof(char) * len);
	getenv_s(&len, p, len, ENV_REPO_PATH);
	std::string s = p;
	free(p);

	std::ifstream ifs(s + STAGE_DATA_PATH, std::ifstream::in);
	g = new caracal::game(ifs, s + ASSETS_DIR);
	if (g->load_failed()) {
		notify("Could not load stage or assets.");
		return false;
	}

	return true;
}