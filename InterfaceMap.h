/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091030
 */

#ifndef INTERFACE_MAP_H
#define INTERFACE_MAP_H

#include "PlayerManager.h"
#include "World.h"
#include "Camera.h"

#include <SDL/SDL.h>

class InterfaceMap {
	public:
		void initialize( SDL_Rect *area, SDL_Surface *screen,
				Camera *camera, World *world,
				PlayerManager *player_manager );

		void set_zoom( int zoom ) { my_zoom = zoom; }
		void set_area( SDL_Rect *area );

		void draw();
		void draw_terrain();
		void draw_units();
		void draw_camera_rect();

		bool covers( int x, int y );
		void on_click( int x, int y );

	private:
		SDL_Surface		*the_screen;
		Camera			*the_camera;
		World			*the_world;
		PlayerManager	*the_player_manager;

		SDL_Rect	my_area;
		int			my_zoom;
};

#endif
