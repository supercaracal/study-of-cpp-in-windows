#include <fstream>
#include "GameLib/Framework.h"
#include "image.h"

const char* IMG_PATH = "C:/Users/DIO/source/repos/study-of-cpp/13_00_00/misawa.dds";
const image& g_img = image(IMG_PATH);
GameLib::Texture* g_tt = NULL;
int g_theta = 0;
int g_size_per = 0;
bool g_scale_up = true;

namespace GameLib {
	void scale(double* point, double per) {
		point[0] *= per;
		point[1] *= per;
	}

	void rotate(double* point, double sine, double cosine, double offset_x, double offset_y) {
		double x = point[0] - offset_x;
		double y = point[1] - offset_y;
		point[0] = cosine * x - sine * y + offset_x;
		point[1] = sine * x + cosine * y + offset_y;
	}

	void center(double* point, double offset_x, double offset_y) {
		point[0] += offset_x;
		point[1] += offset_y;
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

		++g_theta;
		if (g_scale_up) {
			++g_size_per;
			if (g_size_per >= 300) g_scale_up = false;
		} else {
			--g_size_per;
			if (g_size_per <= 0) g_scale_up = true;
		}

		double img_w = static_cast<double>(g_img.width());
		double img_h = static_cast<double>(g_img.height());
		double win_w = width();
		double win_h = height();

		double a[2] = { 0.0,   0.0   }; // left top
		double b[2] = { img_w, 0.0   }; // right top
		double c[2] = { 0.0,   img_h }; // left bottom
		double d[2] = { img_w, img_h }; // right bottom

		double ta[2] = { 0.0, 0.0 };
		double tb[2] = { 1.0, 0.0 };
		double tc[2] = { 0.0, 1.0 };
		double td[2] = { 1.0, 1.0 };

		double per = static_cast<double>(g_size_per) / 100.0;
		scale(a, per);
		scale(b, per);
		scale(c, per);
		scale(d, per);

		double c_offset_x = win_w / 2.0 - (b[0] - a[0]) / 2.0;
		double c_offset_y = win_h / 2.0 - (c[1] - a[1]) / 2.0;

		double theta = static_cast<double>(g_theta);
		double sine = sin(theta);
		double cosine = cos(theta);
		double r_offset_x = a[0] + (b[0] - a[0]) / 2.0;
		double r_offset_y = a[1] + (c[1] - a[1]) / 2.0;
		rotate(a, sine, cosine, r_offset_x, r_offset_y);
		rotate(b, sine, cosine, r_offset_x, r_offset_y);
		rotate(c, sine, cosine, r_offset_x, r_offset_y);
		rotate(d, sine, cosine, r_offset_x, r_offset_y);

		center(a, c_offset_x, c_offset_y);
		center(b, c_offset_x, c_offset_y);
		center(c, c_offset_x, c_offset_y);
		center(d, c_offset_x, c_offset_y);

		drawTriangle2D(a, b, c, ta, tb, tc, 0xff000000, 0xffffffff, 0xffffffff);
		drawTriangle2D(d, b, c, td, tb, tc, 0xff000000, 0xffffffff, 0xffffffff);
	}
}