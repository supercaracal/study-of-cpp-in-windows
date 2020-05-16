#include <fstream>
#include "GameLib/Framework.h"
#include "image.h"

static const char* ENV_REPO_PATH = "MY_GAME_PROGRAMMER_BOOK_REPO";
static const char* IMG_FILE_PATH = "/12_04_01/phase.dds";
static const image* g_img = NULL;
static int g_count = 10;


namespace GameLib {
	void Framework::update() {
		if (g_img == NULL) {
			size_t len;
			getenv_s(&len, NULL, 0, ENV_REPO_PATH);
			if (len == 0) {
				GameLib::cout << "ENV var not found." << GameLib::endl;
				exit(EXIT_FAILURE);
			}
			char* p = (char*)malloc(sizeof(char) * len);
			getenv_s(&len, p, len, ENV_REPO_PATH);
			std::string s = p;
			free(p);
			s += IMG_FILE_PATH;
			g_img = new image(s.c_str());
			if (!g_img->loaded()) {
				GameLib::cout << "Could not load image file." << GameLib::endl;
				exit(EXIT_FAILURE);
			}
		}
		if (!g_img->loaded()) {
			GameLib::cout << "Could not load image file." << GameLib::endl;
			exit(EXIT_FAILURE);
		}
		unsigned* vram = videoMemory();
		int windowHeight = height();
		int windowWidth = width();
		int imageHeight = g_img->height();
		int imageWidth = g_img->width();
		if (imageHeight > windowHeight || imageWidth > windowWidth) {
			GameLib::cout << "Could not render image data." << GameLib::endl;
			exit(EXIT_FAILURE);
		}
		int max_length = windowHeight * windowWidth;
		for (int i = 0; i < max_length; ++i) {
			vram[i] = 0;
		}
		double per = static_cast<double>(g_count) / 100.0;
		double img_h = static_cast<double>(imageHeight) * per;
		double img_w = static_cast<double>(imageWidth) * per;
		double win_h = static_cast<double>(windowHeight);
		double win_w = static_cast<double>(windowWidth);
		int offset_idx = static_cast<int>((win_h - img_h) * 0.5) * windowWidth + static_cast<int>((win_w - img_w) * 0.5);
		int point_y, point_x;
		int idx;
		for (int y = 0; y < imageHeight; ++y) {
			point_y = static_cast<int>(static_cast<double>(y) * per);
			for (int x = 0; x < imageWidth; ++x) {
				point_x = static_cast<int>(static_cast<double>(x) * per);
				idx = point_y * windowWidth + point_x + offset_idx;
				if (idx < 0 || idx >= max_length) {
					continue;
				}
				vram[idx] = g_img->fetch(y, x);
			}
		}
		if (++g_count > 120) g_count = 10;
		sleep(100);
	}
}