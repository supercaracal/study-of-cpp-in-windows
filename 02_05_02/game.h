#ifndef SRC_01_03_05_GAME_H_
#define SRC_01_03_05_GAME_H_
#include <sstream>
#include "state.h"
namespace caracal {
	class game {
	public:
		game();
		game(const game& g);
		game& operator=(const game& g);
		virtual ~game();
		void load_stage(std::istream& is);
		bool load_failed() const;
		void update(char key);
		std::string get_stage();
		bool is_goal();
	private:
		bool m_loaded;
		state m_state;
	};
}
#endif  // SRC_01_03_05_GAME_H_