/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Emil Österlund, spektre
 *	Created on:	09102?
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "Unit.h"
#include "World.h"
#include "PlayerManager.h"

#include <vector>

class PlayerManager;
class Unit;

typedef std::vector<Unit>::iterator UnitIterator;

class Player {
	public:
		void initialize( World *world, PlayerManager *player_manager, int id );

		void update();

		int  get_id() { return my_id; }
		void on_begin_turn();
		void on_end_turn();

		std::vector<Unit> *get_units() { return &my_units; }
		void create_unit( int row, int col, UnitType unit_type );
		void remove_unit_at( int row, int col );
		void remove_selected_unit();

		void move_selected_unit( int direction );

		bool is_ally( int player_id ) { return false; }
		bool has_discovered_tile( int row, int col ) {
			return my_discovered_tiles[row * the_world->get_col_count() + col];
		}
		bool can_see_tile( int row, int col ) {
			return my_visible_tiles[row * the_world->get_col_count() + col];
		}

		std::vector<Unit> *get_unit_list() { return &my_units; }
		//
		// The return value of these methods must be checked from the calling 
		//  code!
		// If no unit is selected they return NULL and will crash from
		//  any further manipulation
		//
		Unit *select_next_unit();
		Unit *select_prev_unit();
		Unit *select_unit_at_tile( int row, int col );
		Unit *select_unit_at_xy( int x, int y );

		Unit *get_unit_at_tile( int row, int col );
		Unit *get_unit_at_xy( int row, int col );
		Unit *get_selected_unit();

	private:
		World			*the_world;
		PlayerManager 	*the_player_manager;
		int				my_id;
		int				my_money;

		std::vector<Unit>	my_units;
		int					my_selected_unit;

		std::vector<bool>	my_discovered_tiles;
		std::vector<bool>	my_visible_tiles;
};

#endif
