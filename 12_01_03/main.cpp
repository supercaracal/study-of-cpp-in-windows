#include <fstream>
#include <sstream>
#include "GameLib/Framework.h"
#include "image.h"

static const char* IMG_FILE_PATH = "C:/Users/DIO/source/repos/study-of-cpp/12_01_03/phase.dds";
static const image& g_img = image(IMG_FILE_PATH);
static int g_count = 0;
static int g_wait = 0;

namespace GameLib {
	void Framework::update() {
		if (!g_img.loaded()) {
			GameLib::cout << "Could not load image file." << GameLib::endl;
			exit(EXIT_FAILURE);
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
		if (--g_wait > 0) {
			return;
		} else {
			g_wait = 100;
		}
		int diff_idx_of_centering = (windowHeight - imageHeight) / 2 * windowWidth + (windowWidth - imageWidth) / 2;
		int idx;
		int max_idx = windowHeight * windowWidth;
		double offset_y = imageHeight / 2.0;
		double offset_x = imageWidth / 2.0;
		double pos_y, pos_x, rot_y, rot_x;
		double theta = static_cast<double>(g_count);
		double sine = sin(theta);
		double cosine = cos(theta);
		// double r;
		// double angle;
		for (int i = 0; i < windowHeight * windowWidth; ++i) {
			vram[i] = 0;
		}
		for (int y = 0; y < imageHeight; ++y) {
			for (int x = 0; x < imageWidth; ++x) {
				pos_y = static_cast<double>(y) - offset_y;
				pos_x = static_cast<double>(x) - offset_x;
				// r = sqrt(pos_y * pos_y + pos_x * pos_x);
				// angle = atan2(pos_y, pos_x) + g_theta;
				// sine = sin(angle);
				// cosine = cos(angle);
				// pos_y = r * cosine + offset_y;
				// pos_x = r * sine + offset_x;
				// idx = static_cast<int>(pos_y) * windowWidth + static_cast<int>(pos_x) + diff_idx_of_centering;
				rot_y = cosine * pos_x - sine * pos_y + offset_y;
				rot_x = sine * pos_x + cosine * pos_y + offset_x;
				idx = static_cast<int>(rot_y) * windowWidth + static_cast<int>(rot_x) + diff_idx_of_centering;
				if (idx < 0 || idx > max_idx) {
					continue;
				}
				vram[idx] = g_img.fetch(y, x);
			}
		}
		++g_count;

		std::ostringstream oss;
		oss << frameRate();
		drawDebugString(0, 0, oss.str().c_str());
	}
}