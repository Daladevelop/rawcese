/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Emil Österlund, Christofer Oden
 *	Created on:	09102?
 */

#include "Unit.h"
#include "globals.h"

void Unit::initialize( World *world, PlayerManager *player_manager,
	   	int id, int owner_id, int row, int col, UnitType unit_type,
		int max_health, int max_move_count, int attack, int defense )
{
    the_world			= world;
	the_player_manager	= player_manager;
	my_id				= id;
    my_owner_id			= owner_id;
    my_row				= row;
    my_col				= col;

    my_type				= unit_type;
    my_max_health		= max_health;
    my_max_move_count	= max_move_count;
    my_attack			= attack;
    my_defense			= defense;

    my_move_count		= max_move_count;
	my_health			= max_health;
}

BattleOutcome Unit::try_move( int direction ) {
    if( my_move_count <= 0 ) {
		printf( "Out of moves\n" );
        return OUTCOME_NONE;
	}

    int row = my_row;
    int col = my_col;

    // Get coordinates of the tile we want to move to
    switch( direction ) {
        case NW:
            --row;
            --col;
            break;

        case N:
            --row;
            break;

        case NE:
            --row;
            ++col;
            break;

        case E:
            ++col;
            break;

        case SE:
            ++row;
            ++col;
            break;

        case S:
            ++row;
            break;

        case SW:
            ++row;
            --col;
            break;

        case W:
            --col;
            break;
    }
    
    // If the attempted move is illegal, discard it
    if( the_world->is_out_of_bounds( row, col ) ) { 
		printf( "Move was out of world bounds\n" );
        return OUTCOME_NONE;
	}
    
    // If it's a forbidden terrain type, discard the move
    if( the_world->get_tile_type( row, col ) == TILE_WATER ) {
		printf( "Impassable terrain\n" );
        return OUTCOME_NONE;
	}
    
    // Go through all players units to see if there is one at the new tile
	printf( "Checking if tile is occupied\n" );
	std::vector<Player> *player_list = the_player_manager->get_player_list();
	PlayerIterator it = player_list->begin();
    for( it = player_list->begin();
			it < player_list->end(); ++it )
	{
        Unit *unit = it->get_unit_at_tile( row, col );
        if( unit != NULL) {
			printf( "Tile is occupied\n" );
            if( unit->get_owner_id() == my_owner_id ) {
				printf( "Own unit here\n" );
                return OUTCOME_NONE;
            }
			printf( "Enemy unit here\n" );
            // Oh oh! Enemy unit!
            // Attack!
            BattleOutcome outcome = attack( unit );
            my_move_count = 0;

			if( outcome == OUTCOME_VICTORY ) {
				unit->do_damage( rand() % 100 );
				printf( "Enemy down to %d%% health\n", unit->get_health() );
				if( unit->get_health() <= 0 )
					it->remove_unit_at( row, col );
			}

            return outcome;
        }
    }
    // OK, let's actually move the unit...
	printf("Moving unit\n");
	--my_move_count;
    my_row = row;
    my_col = col;

    return OUTCOME_NONE;
}

BattleOutcome Unit::attack( Unit *unit ) {
    double prob_win;
    double local_def = unit->get_def();

    // Example of terrain type bonus, expand later
    if( the_world->get_tile_type( unit->get_row(), unit->get_col() )
			== TILE_TREES )
	{
        if( unit->get_type() == UNIT_INFANTRY )
            local_def *= 1.5;
    }

    prob_win = double(my_attack) / (double(my_attack) + local_def);

    if(  CHANCE(prob_win) ) {
		printf("Victory!\n");
        return OUTCOME_VICTORY;
    }
    else {
		printf("Defeat...\n");
        return OUTCOME_DEFEAT;
    }
}
