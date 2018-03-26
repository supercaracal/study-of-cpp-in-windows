#include "GameLib/Framework.h"
#include "sequences/sequence.h"
#include "sequences/title.h"

caracal::sequences::sequence* g_seq;

namespace GameLib {
	void Framework::update() {
		if (g_seq == NULL) {
			g_seq = new caracal::sequences::title();
		}

		if (isEndRequested()) {
			delete g_seq;
			return;
		}

		char c = '?';
		if (isKeyTriggered('q')) {
			c = 'q';
		} else if (isKeyTriggered(' ')) {
			c = ' ';
		} else if (isKeyOn('w')) {
			c = 'w';
		} else if (isKeyOn('z')) {
			c = 'z';
		} else if (isKeyOn('a')) {
			c = 'a';
		} else if (isKeyOn('s')) {
			c = 's';
		} else if (isKeyTriggered('1')) {
			c = '1';
		} else if (isKeyTriggered('2')) {
			c = '2';
		} else if (isKeyTriggered('3')) {
			c = '3';
		}

		caracal::sequences::sequence* next_seq = g_seq->execute(c);
		if (next_seq != g_seq) {
			delete g_seq;
			g_seq = next_seq;
		}

		unsigned* data = g_seq->data();
		unsigned* vram = videoMemory();
		for (unsigned i = 0, max = g_seq->data_size(); i < max; ++i) {
			vram[i] = data[i];
		}

		if (g_seq->should_close()) {
			requestEnd();
		}

		setFrameRate(60);
	}
}