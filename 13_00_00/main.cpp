#include <fstream>
#include "GameLib/Framework.h"
#include "image.h"

const char* IMG_PATH = "C:/Users/DIO/source/repos/study-of-cpp/13_00_00/misawa.dds";
const image& g_img = image(IMG_PATH);
GameLib::Texture* g_tt = NULL;
int g_count = 0;

namespace GameLib {
	void rotate(double* point, double sine, double cosine, double offset_x, double offset_y) {
		double x = point[0] - offset_x;
		double y = point[1] - offset_y;
		point[0] = cosine * x - sine * y + offset_x;
		point[1] = sine * x + cosine * y + offset_y;
	}
}

namespace GameLib {
	void Framework::update() {
		if (g_img.loaded() && g_tt == NULL) {
			createTexture(&g_tt, 128, 128, g_img.data(), g_img.width(), g_img.height());
		}
		if (isEndRequested()) {
			destroyTexture(&g_tt);
			return;
		}
		setFrameRate(60);
		setTexture(g_tt);
		g_count++;
		double a[2] = { 50.0, 50.0 };   // left top
		double b[2] = { 130.0, 50.0 };  // right top
		double c[2] = { 50.0, 150.0 };  // left bottom
		double d[2] = { 130.0, 150.0 }; // right bottom
		double ta[2] = { 0.0, 0.0 };
		double tb[2] = { 1.0, 0.0 };
		double tc[2] = { 0.0, 1.0 };
		double td[2] = { 1.0, 1.0 };
		double theta = static_cast<double>(g_count);
		double sine = sin(theta);
		double cosine = cos(theta);
		double offset_x = a[0] + (b[0] - a[0]) / 2.0;
		double offset_y = a[1] + (c[1] - a[1]) / 2.0;
		rotate(a, sine, cosine, offset_x, offset_y);
		rotate(b, sine, cosine, offset_x, offset_y);
		rotate(c, sine, cosine, offset_x, offset_y);
		rotate(d, sine, cosine, offset_x, offset_y);
		drawTriangle2D(a, b, c, ta, tb, tc, 0xff000000, 0xffffffff, 0xffffffff);
		drawTriangle2D(d, b, c, td, tb, tc, 0xff000000, 0xffffffff, 0xffffffff);
	}
}