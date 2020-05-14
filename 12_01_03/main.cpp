#include <fstream>
#include <sstream>
#include "GameLib/Framework.h"
#include "image.h"

static const char* ENV_REPO_PATH = "MY_GAME_PROGRAMMER_BOOK_REPO";
static const char* IMG_FILE_PATH = "/12_01_03/phase.dds";
static const image* g_img = NULL;
static int g_count = 0;

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
		unsigned* vram = videoMemory();
		int windowHeight = height();
		int windowWidth = width();
		int imageHeight = g_img->height();
		int imageWidth = g_img->width();
		if (imageHeight > windowHeight || imageWidth > windowWidth) {
			GameLib::cout << "Could not render image data." << GameLib::endl;
			exit(EXIT_FAILURE);
		}
		int idx;
		int offset_idx = (windowHeight - imageHeight) / 2 * windowWidth + (windowWidth - imageWidth) / 2;
		int max_length = windowHeight * windowWidth;
		double offset_y = imageHeight / 2.0;
		double offset_x = imageWidth / 2.0;
		double pos_y, pos_x, rot_y, rot_x;
		double theta = static_cast<double>(g_count);
		double sine = sin(theta);
		double cosine = cos(theta);
		for (int i = 0; i < max_length; ++i) {
			vram[i] = 0;
		}
		for (int y = 0; y < imageHeight; ++y) {
			for (int x = 0; x < imageWidth; ++x) {
				pos_y = static_cast<double>(y) - offset_y;
				pos_x = static_cast<double>(x) - offset_x;
				rot_x = cosine * pos_x - sine * pos_y + offset_x;
				rot_y = sine * pos_x + cosine * pos_y + offset_y;
				idx = static_cast<int>(rot_y) * windowWidth + static_cast<int>(rot_x) + offset_idx;
				if (idx < 0 || idx >= max_length) {
					continue;
				}
				vram[idx] = g_img->fetch(y, x);
			}
		}
		++g_count;
		sleep(100);
	}
}