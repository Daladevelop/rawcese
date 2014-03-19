/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on: 091031
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "World.h"

#include <SDL/SDL.h>

class Camera {
	public:
		void initialize( World *world, SDL_Rect *rect );

		void update();

		SDL_Rect	*get_rect() { return &my_rect; }	
		int			get_x() { return my_rect.x; }
		int			get_y() { return my_rect.y; }
		int			get_w() { return my_rect.w; }
		int			get_h() { return my_rect.h; }
		bool		tile_is_in_scope( int row, int col );
		bool		xy_is_in_scope( int x, int y );

		void 		set_movement_x( int x );
		void 		set_movement_y( int y );
		void 		stop_movement_x();
		void 		stop_movement_y();
		void 		center_at_xy( int x, int y );
		void 		center_at_tile( int row, int col );

	private:
		World		*the_world;

		SDL_Rect	my_rect;
		int			my_movement_x;
		int			my_movement_y;
};

#endif
