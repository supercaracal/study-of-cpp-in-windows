#include "GameLib/Framework.h"
#include "font.h"
#include "buffer.h"

static const char* ENV_REPO_PATH = "MY_GAME_PROGRAMMER_BOOK_REPO";
static const char* FONT_IMG_PATH = "/06_00_00/my_fonts.dds";

bool g_loaded = false;
caracal::font* g_f = 0;
caracal::buffer* g_buf = 0;
unsigned g_fr_cnt = 0;

namespace GameLib {
	void Framework::update() {
		if (isEndRequested()) {
			delete g_f;
			delete g_buf;
			return;
		}

		if (!g_loaded) {
			size_t len;
			getenv_s(&len, NULL, 0, ENV_REPO_PATH);
			if (len == 0) {
				return;
			}
			char* p = (char*)malloc(sizeof(char) * len);
			getenv_s(&len, p, len, ENV_REPO_PATH);
			std::string s = p;
			free(p);
			s += FONT_IMG_PATH;
			g_f = new caracal::font(s.c_str());
			g_buf = new caracal::buffer(24, 32);
			g_loaded = true;
		}

		char ic;
		for (int i = 32; i < 127; ++i) {
			ic = static_cast<char>(i);
			if (isKeyTriggered(ic)) {
				g_buf->add(ic);
				break;
			}
		}

		char oc;
		unsigned* vram = videoMemory();
		unsigned* font;
		unsigned dot;
		for (unsigned i = 0; i < 24; ++i) {
			for (unsigned j = 0; j < 32; ++j) {
				oc = g_buf->fetch(i, j);
				font = g_f->fetch(oc);
				for (unsigned m = 0; m < 10; ++m) {
					for (unsigned n = 0; n < 10; ++n) {
						dot = font[m * 10 + n];
						int alpha = (dot & 0xff000000) >> 24;
						if (alpha < 128) {
							dot = 0x00000000;
						}
						vram[(i * 10 + m) * 320 + j * 10 + n] = dot;
					}
				}
				delete[] font;
			}
		}

		setFrameRate(60);
		if (++g_fr_cnt % 60 == 0) {
			g_fr_cnt = 0;
			cout << frameRate() << endl;
		}
	}
}