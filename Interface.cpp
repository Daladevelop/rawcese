/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091031
 */

#include "Interface.h"

void Interface::initialize( SDL_Surface *screen, Camera *camera,
		World *world, PlayerManager *player_manager )
{
	the_screen			= screen;
	the_camera			= camera;
	the_world			= world;
	the_player_manager	= player_manager;

	SDL_Rect map_rect = { SCREEN_WIDTH - the_world->get_col_count() - 16,
		16, the_world->get_col_count(), the_world->get_row_count() };
	my_interface_map.initialize( &map_rect, the_screen, the_camera,
			the_world, the_player_manager );


	my_rect.h = SCREEN_HEIGHT;
	my_rect.w = 100;
	my_rect.x = SCREEN_WIDTH - my_rect.w;
	my_rect.y = 0;

	
}

void Interface::draw() {
	SDL_FillRect( the_screen, &my_rect,
			SDL_MapRGB( the_screen->format,
				0x10, 0x00, 0x00 ) );
	my_interface_map.draw();
}

void Interface::on_click( int x, int y ) {
	printf( "Interface->Click!\n" );
	if( my_interface_map.covers( x, y ) == true ) {
		my_interface_map.on_click( x, y );
	}
}

bool Interface::covers( int x, int y ) {
	if( my_interface_map.covers( x, y ) == true )
		return true;

	if( x < my_rect.x || x > my_rect.x + my_rect.w
		|| y < my_rect.y || y > my_rect.y + my_rect.h )
	{
		return false;
	}

	return true;
}

