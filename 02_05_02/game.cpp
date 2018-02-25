#include "game.h"

namespace caracal {
	game::game() : m_loaded(false) {
	}

	game::game(const game& g) {
		m_loaded = g.m_loaded;
		m_state = g.m_state;
	}

	game& game::operator=(const game& g) {
		m_loaded = g.m_loaded;
		m_state = g.m_state;
		return *this;
	}

	game::~game() {
	}

	void game::load_stage(std::istream& is) {
		if (m_loaded || is.fail()) return;

		is.seekg(0, is.end);
		int length = static_cast<int>(is.tellg());
		is.seekg(0, is.beg);
		char* buffer = new char[length];
		is.read(buffer, length);
		m_state.set(buffer);
		m_loaded = true;
		delete buffer;
	}

	bool game::load_failed() const {
		return !m_loaded;
	}

	void game::update(char key) {
		m_state.move(key);
	}

	std::string game::get_stage() {
		return m_state.get();
	}

	bool game::is_goal() {
		return m_state.is_goal();
	}
}