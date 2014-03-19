/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Emil Österlund, Christofer Oden
 *	Created on:	09102?
 */

#include "Player.h"

#define NO_SELECTED_UNIT -1

void Player::initialize( World *world,
		PlayerManager *player_manager, int id )
{
	printf( "Initializing player: %d\n", id );

	the_world			= world;
	the_player_manager	= player_manager;

	my_id				= id;
	my_money			= 0;
	my_selected_unit	= NO_SELECTED_UNIT;

	for ( int i = 0;
			i < the_world->get_row_count() * the_world->get_col_count(); ++i )
	{
		my_discovered_tiles.push_back( true );
		my_visible_tiles.push_back( false );
	}
}

void Player::update() {
	for( int i = 0;
			i < the_world->get_row_count() * the_world->get_col_count(); ++i )
	{
		my_visible_tiles.at( i ) = false;
	}

	for( UnitIterator it = my_units.begin(); it < my_units.end(); ++it ) {
		int row = it->get_row();
		int col = it->get_col();

		for(int r = row - 1; r <= row + 1; ++r ) {
			for( int c = col - 1; c <= col + 1; ++c) {
				my_discovered_tiles[ r * the_world->get_col_count() + c ]
					= true;
				my_visible_tiles[ r * the_world->get_col_count() + c ]
					= true;
			}
		}
	}
}

void Player::on_begin_turn() {
	for (UnitIterator it = my_units.begin(); it < my_units.end(); ++it) {
		it->reset_moves();
	}
}

void Player::on_end_turn() {
	my_selected_unit = NO_SELECTED_UNIT;
}

void Player::create_unit( int row, int col, UnitType unit_type ) {
	if( get_unit_at_tile( row, col ) ) {
		fprintf( stderr, "Could not create unit at (%d, %d), "
				"tile already occupied\n", row, col);
		return;
	}
	if( the_world->get_tile_type( row, col ) == TILE_WATER ) {
		fprintf( stderr, "Could not create unit at (%d, %d), "
				"impassable terrain\n", row, col);
		return;
	}

	printf("Creating new unit at location (%d,%d)\n", row, col);
	int new_id = 0;
	if( my_units.empty() == false )
		new_id = my_units.back().get_id() + 1;

	my_units.push_back( Unit() );

	int max_health = 100;
	int max_moves, attack, defense;
	if( unit_type == UNIT_DRAGON ) {
		max_moves = 10;
		attack   = 5;
		defense  = 5;
	}
	else if( unit_type == UNIT_INFANTRY ) {
		max_moves = 1;
		attack   = 1;
		defense  = 1;
	}
	else if( unit_type == UNIT_ARMOR ) {
		max_moves = 2;
		attack   = 2;
		defense  = 2;
	}
	else if( unit_type == UNIT_HOWITZER ) {
		max_moves = 1;
		attack   = 3;
		defense  = 1;
	}
	else if( unit_type == UNIT_MGUNNERORC ) {
		max_moves = 6;
		attack   = 4;
		defense  = 3;
	}
	else if( unit_type == UNIT_BEHOLDER ) {
		max_moves = 10;
		attack   = 12;
		defense  = 4;
	}
	printf("Initializing new unit:\n");
	printf("ID:         %d\n", new_id);
	printf("Owner ID:   %d\n", my_id);
	printf("Health:     %d\n", max_health);
	printf("Move speed: %d\n", max_moves);
	printf("Attack:     %d\n", attack);
	printf("Defense:    %d\n", defense);
	my_units.back().initialize( the_world, the_player_manager, new_id, my_id, row, col,
			unit_type, max_health, max_moves, attack, defense );
}

void Player::remove_unit_at( int row, int col ) {
	for( UnitIterator it = my_units.begin(); it < my_units.end(); ++it ) {
		if( it->get_row() == row && it->get_col() == col ) {
			my_units.erase( it );
			my_selected_unit = NO_SELECTED_UNIT;
			return;
		}
	}
	printf("DEBUG: remove_at(%d, %d) ran but found no unit, is this right?\n", row, col);
}

void Player::remove_selected_unit() {
	if( my_selected_unit == NO_SELECTED_UNIT )
		return;

	my_units.erase( my_units.begin() + my_selected_unit );
	my_selected_unit = NO_SELECTED_UNIT;
}

void Player::move_selected_unit( int direction ) {
	if( my_selected_unit == NO_SELECTED_UNIT ) {
		printf( "No selected unit to move\n" );
		return;
	}

	printf( "Trying to move\n" );
	BattleOutcome outcome =
		my_units.at( my_selected_unit ).try_move( direction );
	printf( "Move finished\n" );
	if( outcome == OUTCOME_DEFEAT ) {
		my_units.at( my_selected_unit ).do_damage( rand() % 100 );
		printf( "Own unit down to %d%% health\n",
				my_units.at( my_selected_unit ).get_health() );
		if( my_units.at( my_selected_unit ).get_health() <= 0 ) {
			printf( "Removing our defeated unit\n" );
			remove_selected_unit();
		}
	}
}

Unit *Player::select_next_unit() {
	if( my_units.empty() == false ) {
		printf("Selecting next unit\n");
		if( my_selected_unit == NO_SELECTED_UNIT ) {
			my_selected_unit = 0;
		}
		else if( unsigned(++my_selected_unit) == my_units.size() ) {
			my_selected_unit = 0;
		}
		printf( "Unit id: %d\n",
				my_units.at( my_selected_unit ).get_id() );
		return get_selected_unit();
	}
	printf( "No units to select\n" );
	return NULL;
}

Unit *Player::select_prev_unit() {
	if( my_units.empty() == false ) {
		printf("Selecting previous unit\n");
		if( my_selected_unit == NO_SELECTED_UNIT ) {
			my_selected_unit = my_units.size() - 1;
		}
		else if( --my_selected_unit < 0 ) {
			my_selected_unit = my_units.size() - 1;
		}
		printf( "Unit id: %d\n",
				my_units.at( my_selected_unit ).get_id() );
		return get_selected_unit();
	}
	printf( "No units to select\n" );
	return NULL;
}

Unit *Player::select_unit_at_tile( int row, int col ) {
	my_selected_unit = NO_SELECTED_UNIT;

	for( UnitIterator it = my_units.begin(); it < my_units.end(); ++it ) {
		if ( it->get_row() == row && it->get_col() == col ) {
			my_selected_unit = int(it - my_units.begin());
			break;
		}
	}
	return get_selected_unit();
}

Unit *Player::select_unit_at_xy( int x, int y ) {
	my_selected_unit = NO_SELECTED_UNIT;

	for( UnitIterator it = my_units.begin(); it < my_units.end(); ++it ) {
		if ( it->get_row() == y / TILE_SIZE
				&& it->get_col() == x / TILE_SIZE )
		{
			my_selected_unit = int(it - my_units.begin());
			break;
		}
	}
	return get_selected_unit();
}

Unit *Player::get_unit_at_tile( int row, int col ) {
	for( UnitIterator it = my_units.begin(); it < my_units.end(); ++it ) {
		if( it->get_row() == row && it->get_col() == col ) {
			return &(*it);
		}
	}
	return NULL;
}

Unit *Player::get_unit_at_xy( int x, int y ) {
	for( UnitIterator it = my_units.begin(); it < my_units.end(); ++it ) {
		if( it->get_row() == y / TILE_SIZE
				&& it->get_col() == x / TILE_SIZE )
		{
			return &(*it);
		}
	}
	return NULL;
}

Unit *Player::get_selected_unit() {
	if( my_selected_unit == NO_SELECTED_UNIT )
		return NULL;
	else
		return &my_units.at( my_selected_unit );
}
