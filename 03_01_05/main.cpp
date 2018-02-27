#include <fstream>
#include "GameLib/Framework.h"
#include "image.h"

static const char* IMG_FILE_PATH = "C:/Users/DIO/source/repos/study-of-cpp/03_01_05/phase.dds";
static const image& g_img = image(IMG_FILE_PATH);
static bool rendered = false;

namespace GameLib {
	void Framework::update() {
		if (!g_img.loaded()) {
			GameLib::cout << "Could not load image file." << GameLib::endl;
			exit(EXIT_FAILURE);
		}
		if (rendered) {
			return;
		}
		unsigned* vram = videoMemory();
		int windowHeight = height();
		int windowWidth = width();
		int imageHeight = g_img.height();
		int imageWidth = g_img.width();
		if (imageHeight > windowHeight || imageWidth > windowWidth) {
			GameLib::cout << "Could not render image data." << GameLib::endl;
			exit(EXIT_FAILURE);
		}
		for (int y = 0; y < imageHeight; ++y) {
			for (int x = 0; x < imageWidth; ++x) {
				vram[y * windowWidth + x] = g_img.fetch(y, x);
			}
		}
		rendered = true;
	}
}