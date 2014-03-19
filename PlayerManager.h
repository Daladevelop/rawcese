/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091030
 */

#ifndef PLAYER_LIST_H
#define PLAYER_LIST_H

#include "Player.h"
#include "World.h"

#include <vector>

class Player;

typedef std::vector<Player>::iterator PlayerIterator;

class PlayerManager {
	public:
		void initialize( World *world, int player_count );

		void update();

		void end_turn();
		Player *get_current_player() { return &(*my_current_player); }
		std::vector<Player> *get_player_list() { return &my_players; }
		
	private:
		World *the_world;

		std::vector<Player> my_players;
		std::vector<Player>::iterator my_current_player;
};

#endif
