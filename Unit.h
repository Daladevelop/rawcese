/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Emil Österlund, Christofer Oden
 *	Created on:	09102?
 */

#ifndef UNIT_H
#define UNIT_H

#include "Player.h"
#include "World.h"
#include "PlayerManager.h"

#include <vector>

class PlayerManager;

class Unit {
	public:
		void initialize( World *world, PlayerManager *player_manager,
				int id, int owner, int row, int col,
				UnitType unit_type, int max_health, int max_moves,
				int attack, int defense );

		BattleOutcome	try_move( int direction );
		BattleOutcome	attack( Unit *unit );

		void set_moves(int count)	{ my_move_count	= count; }
		void reset_moves()			{ my_move_count	= my_max_move_count; }
		void do_damage(int damage)	{ my_health		-= damage; }
		void reset_health()			{ my_health		= my_max_health; }

		UnitType get_type()			{ return my_type; }
		int get_id()				{ return my_id; }
		int get_owner_id()			{ return my_owner_id; }
		int get_row()				{ return my_row; }
		int get_col()				{ return my_col; }
		int get_moves()				{ return my_move_count; }
		int get_health()			{ return my_health; }
		int get_atk()				{ return my_attack; }
		int get_def()				{ return my_defense; }
		int get_maxhealth()     {return my_max_health;}

	private:
		World			*the_world;
		PlayerManager	*the_player_manager;
		UnitType		my_type;
		int				my_id;
		int				my_owner_id;
		int				my_row, my_col;
		int				my_max_health, my_health;
		int				my_max_move_count, my_move_count;
		int				my_attack, my_defense;
};

#endif
