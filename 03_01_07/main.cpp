#include <fstream>
#include <cstdlib>
#include "game.h"
#include "GameLib/Framework.h"

static const char* ENV_REPO_PATH = "MY_GAME_PROGRAMMER_BOOK_REPO";
static const char* STAGE_DATA_PATH = "/03_01_07/data/stage.txt";
static const char* ASSETS_DIR = "/03_01_07/assets";

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
	size_t len;
	getenv_s(&len, NULL, 0, ENV_REPO_PATH);
	if (len == 0) {
		notify("Could not find repository path via environment variable.");
		return;
	}

	char* p = (char*)malloc(sizeof(char) * len);
	getenv_s(&len, p, len, ENV_REPO_PATH);
	std::string s = p;
	free(p);

	std::ifstream ifs(s + STAGE_DATA_PATH, std::ifstream::in);
	g = new caracal::game();
	g->load_stage(ifs);
	g->load_assets(s + ASSETS_DIR);
}

static void notify(std::string message) {
	GameLib::cout << message.data() << GameLib::endl;
}