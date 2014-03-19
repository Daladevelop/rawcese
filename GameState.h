/*
 *  Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091030
 */

#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "State.h"
#include "Interface.h"
#include "Camera.h"
#include "World.h"
#include "PlayerManager.h"

#include <SDL/SDL.h>

class GameState : public State {
	public:
		void initialize( SDL_Surface *screen );

		void handle_events();
		void update();
		void draw();

		void handle_keyboard_event( SDL_Event *event );
		void handle_mouse_event( SDL_Event *event );

		void draw_tiles();
		void draw_static_entities();
		void draw_mobile_entities();
		void draw_interface();
		void draw_health_bar( SDL_Rect *box, int health, int max_health );

		void draw_unit_sprite( int x, int y, UnitType unit_type,
				int player_id );
		void draw_water_sprites( int row, int col );
		void draw_tree_sprites( int row, int col );

		bool get_is_running() { return i_am_running; }
		void set_is_running( bool is_running ) { i_am_running = is_running; }

	private:
		SDL_Surface		*the_screen;
		Camera			my_camera;
		World			my_world;
		PlayerManager	my_player_manager;
		Interface		my_interface;

		SDL_Surface		*my_sprite_map;
		SDL_Surface		*my_font_map;
		bool			i_am_running;
};

#endif
