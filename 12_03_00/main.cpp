#include <fstream>
#include "GameLib/Framework.h"
#include "image.h"

static const char* IMG_FILE_PATH = "C:/Users/DIO/source/repos/study-of-cpp/12_03_00/phase.dds";
static const image& g_img = image(IMG_FILE_PATH);
static int g_count = 0;

struct pos {
	double y;
	double x;
};

struct ratio {
	double y;
	double x;
};

struct distance {
	double y;
	double x;
};

struct rotation {
	pos point;
	double sine;
	double cosine;
	distance offset;
};

namespace GameLib {
	rotation rotate(rotation rot) {
		double x = rot.point.x - rot.offset.x;
		double y = rot.point.y - rot.offset.y;
		rot.point.x = rot.cosine * x - rot.sine * y + rot.offset.x;
		rot.point.y = rot.sine * x + rot.cosine * y + rot.offset.y;
		return rot;
	}
}

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
		int max_length = windowHeight * windowWidth;
		for (int i = 0; i < max_length; ++i) {
			vram[i] = 0;
		}
		int idx;
		int offset_idx = (windowHeight - imageHeight) / 2 * windowWidth + (windowWidth - imageWidth) / 2;
		double theta = static_cast<double>(g_count);
		double sine = sin(theta);
		double cosine = cos(theta);
		double img_h = static_cast<double>(imageHeight);
		double img_w = static_cast<double>(imageWidth);
		distance diff;
		distance offset = { img_h / 2.0, img_w / 2.0 };
		pos point;
		pos left_of_top = { 0.0, 0.0 };
		pos right_of_top = { 0.0, img_w };
		pos left_of_bottom = { img_h, 0.0 };
		rotation lt = { left_of_top, sine, cosine, offset };
		rotation rt = { right_of_top, sine, cosine, offset };
		rotation lb = { left_of_bottom, sine, cosine, offset };
		lt = rotate(lt);
		rt = rotate(rt);
		lb = rotate(lb);
		diff.y = lb.point.y - lt.point.y;
		diff.x = rt.point.x - lt.point.x;
		ratio rat;
		for (int y = 0; y < imageHeight; ++y) {
			rat.y = static_cast<double>(y) / img_h;
			point.y = lt.point.y + diff.y * rat.y;
			for (int x = 0; x < imageWidth; ++x) {
				rat.x = static_cast<double>(x) / img_w;
				point.x = lt.point.x + diff.x * rat.x;
				idx = static_cast<int>(point.y) * windowWidth + static_cast<int>(point.x) + offset_idx;
				if (idx < 0 || idx >= max_length) {
					continue;
				}
				vram[idx] = g_img.fetch(y, x);
			}
		}
		if (++g_count > 360) g_count = 0;
		sleep(100);
	}
}