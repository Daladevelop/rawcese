/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091031
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "InterfaceMap.h"
#include "PlayerManager.h"
#include "World.h"
#include "Camera.h"

#include <SDL/SDL.h>

class Interface {
	public:
		void initialize( SDL_Surface *screen, Camera *camera,
				World *world, PlayerManager *player_manager );

		void draw();

		SDL_Rect *get_rect() { return &my_rect; }

		void on_click( int x, int y );
		bool covers( int x, int y );

	private:
		SDL_Surface		*the_screen;
		Camera			*the_camera;
		World			*the_world;
		PlayerManager	*the_player_manager;

		SDL_Rect		my_rect;
		InterfaceMap	my_interface_map;
};

#endif
