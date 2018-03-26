#pragma once
#include "image.h"
#include "store.h"

namespace caracal
{
	class element
	{
	public:
		element(char s, unsigned u, unsigned x);
		~element();
		char sym() const;
		image* get_fg() const;
		image* get_bg() const;
		void set_fg(image* img);
		void set_bg(image* img);
		unsigned cell_y() const;
		unsigned cell_x() const;
		bool can_move_to(element* dest) const;
		bool is_linefeed() const;
		bool is_movable_baggage() const;
		bool does_try_to_become_movable_baggage() const;
		bool is_player() const;
		bool is_player_on_the_goal() const;
		bool is_empty_goal() const;
		bool is_same_pos(unsigned y, unsigned x) const;
		bool is_same_wall(unsigned y, unsigned x) const;
		bool is_same_baggage(unsigned y, unsigned x) const;
		bool is_same_goal(unsigned y, unsigned x) const;
		bool need_foreground() const;
		void reserve(char s, image* fg, image* bg);
		void become();
		void update_forground(store* data) const;
		void update_background(store* data) const;
	private:
		char m_sym;
		unsigned m_cell_y;
		unsigned m_cell_x;
		image* m_fg;
		image* m_bg;
		void update_data(image* img, store* data) const;
		char m_reserved_sym;
		image* m_reserved_fg;
		image* m_reserved_bg;
	};
}