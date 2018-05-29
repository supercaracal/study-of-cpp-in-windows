#include <fstream>
#include "GameLib/Framework.h"
#include "image.h"

const char* IMG_PATH = "C:/Users/DIO/source/repos/study-of-cpp/13_00_00/misawa.dds";
const image& g_img = image(IMG_PATH);
GameLib::Texture* g_tt = NULL;

namespace GameLib {
	void Framework::update() {
		if (g_img.loaded() && g_tt == NULL) {
			createTexture(&g_tt, 128, 128, g_img.data(), g_img.width(), g_img.height());
		}
		if (isEndRequested()) {
			destroyTexture(&g_tt);
			return;
		}
		setTexture(g_tt);              // should call every time
		double a[2] = { 10.0, 10.0 };  // left top
		double b[2] = { 90.0, 10.0 };  // right top
		double c[2] = { 10.0, 110.0 }; // left bottom
		double d[2] = { 90.0, 110.0 }; // right bottom
		double ta[2] = { 0.0, 0.0 };
		double tb[2] = { 1.0, 0.0 };
		double tc[2] = { 0.0, 1.0 };
		double td[2] = { 1.0, 1.0 };
		drawTriangle2D(a, b, c, ta, tb, tc, 0xff000000, 0xffffffff, 0xffffffff);
		drawTriangle2D(d, b, c, td, tb, tc, 0xff000000, 0xffffffff, 0xffffffff);
	}
}