/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091030
 */

#include "PlayerManager.h"

typedef std::vector<Player>::iterator PlayerIterator;

void PlayerManager::initialize( World *world, int player_count ) {
	the_world = world;

	for( int i = 0; i < player_count; ++i ) {
		my_players.push_back( Player() );
		my_players.back().initialize( world, this, i );
	}

	my_current_player = my_players.begin();
}

void PlayerManager::update() {
	for( PlayerIterator it = my_players.begin();
			it < my_players.end(); ++it )
	{
		it->update();
	}
}

void PlayerManager::end_turn() {
	my_current_player->on_end_turn();

	if( ++my_current_player == my_players.end() )
		my_current_player = my_players.begin();

	my_current_player->on_begin_turn();
}
