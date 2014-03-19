/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091030
 */

#include "InterfaceMap.h"
#include "globals.h"

void InterfaceMap::initialize( SDL_Rect *area,
		SDL_Surface *screen, Camera *camera,
		World *world, PlayerManager *player_manager )
{
	the_screen			= screen;
	the_camera			= camera;
	the_world			= world;
	the_player_manager	= player_manager;

	set_area( area );
	set_zoom( 1 );
}

void InterfaceMap::set_area( SDL_Rect *area ) {
	my_area.x = area->x;
	my_area.y = area->y;
	my_area.w = area->w;
	my_area.h = area->h;
}

void InterfaceMap::draw() {
	//printf(" *** Draw mini map terrain ***\n" );
	draw_terrain();
	//printf(" *** Draw mini map units ***\n" );
	draw_units();
	//printf(" *** Draw mini map camera ***\n" );
	draw_camera_rect();
}

void InterfaceMap::draw_terrain() {
	//printf( "Get player pointer\n" );
	Player *current_player = the_player_manager->get_current_player();

	//printf( "Locking screen\n" );
	if( SDL_MUSTLOCK( the_screen ) ) {
		if( SDL_LockSurface( the_screen ) < 0 ) {
			fprintf( stderr, "SDL_LockSurface: %s\n", SDL_GetError() );
			return;
		}
	}

	//printf( "Iterating tiles\n" );
	for( std::vector<Tile>::iterator it = the_world->get_tiles()->begin();
			it < the_world->get_tiles()->end(); ++it )
	{
		//printf( "Check bounds\n" );
		if( it->get_col() * my_zoom > my_area.w ) continue;
		if( it->get_row() * my_zoom > my_area.h ) break;

		//printf( "Check discovered tile\n" );
		//printf( "(%d, %d)\n", it->get_row(), it->get_col() );
		Uint8 r, g, b;
		if( current_player->has_discovered_tile(
					it->get_row(), it->get_col() ) == false ) {
			//printf( "Undiscovered tile\n" );
			r = 0; g = 0; b = 0;
		}
		else {
			//printf( "Discovered tile\n" );
			switch( it->get_type() ) {
				case TILE_GRASS:
					r = 0x3e; g = 0x55; b = 0x1f;
					break;
				case TILE_WATER:
					r = 0x00; g = 0x0b; b = 0x4f;
					break;
				case TILE_TREES:
					r = 0x23; g = 0x29; b = 0x15;
					break;
				default:
					printf("Mini map gets unrecognized tile type(s), "
							"fix it!\n");
					r = 0xff; g = 0x00; b = 0xff;
			}

			if( current_player->can_see_tile(
						it->get_row(), it->get_col() ) == false ) {
				r /= 2; g /= 2; b /= 2;
			}
		}

		//printf( "Putting pixels\n" );
		for( int i = 0; i < my_zoom; ++i ) {
			for( int j = 0; j < my_zoom; ++j ) {
				put_pixel( my_area.x + it->get_col() * my_zoom + j,
						my_area.y + it->get_row() * my_zoom + i,
						SDL_MapRGB( the_screen->format, r, g, b ),
						the_screen );
			}
		}
	}

	//printf( "Unlocking screen\n" );
	if( SDL_MUSTLOCK( the_screen ) )
		SDL_UnlockSurface( the_screen );
}

void InterfaceMap::draw_units() {
	std::vector<Player> *players = the_player_manager->get_player_list();
	Player *current_player = the_player_manager->get_current_player();

	for( PlayerIterator p_it = players->begin();
			p_it < players->end(); ++p_it )
	{
		for( UnitIterator u_it = p_it->get_units()->begin();
				u_it < p_it->get_units()->end(); ++u_it )
		{
			if( u_it->get_col() * my_zoom > my_area.w
					|| u_it->get_row() * my_zoom > my_area.h )
			{
				continue;
			}

			if( current_player->can_see_tile( u_it->get_row(),
						u_it->get_col() ) == false )
			{
				continue;
			}

			Uint8 r, g, b;
			if( p_it->get_id() ==
					the_player_manager->get_current_player()->get_id() )
			{
				r = 0x00; g = 0xff; b = 0x00;
			}
			else if( the_player_manager->get_current_player()->
					is_ally( p_it->get_id() ) )
			{
				r = 0x00; g = 0x00; b = 0xff;
			}
			else {
				r = 0xff; g = 0x00; b = 0x00;
			}

			if( p_it->get_selected_unit() != NULL ) {
				if( u_it->get_id() == p_it->get_selected_unit()->get_id() ) {
					r = 0xff; g = 0xff; b = 0xff;
				}
			}

			for( int i = 0; i < my_zoom; ++i ) {
				for( int j = 0; j < my_zoom; ++j ) {
					put_pixel( my_area.x + u_it->get_col() * my_zoom + j,
							my_area.y + u_it->get_row() * my_zoom + i,
							SDL_MapRGB( the_screen->format, r, g, b ),
							the_screen );
				}
			}
		}
	}
}

void InterfaceMap::draw_camera_rect() {
	SDL_Rect cam_rect = {
		( the_camera->get_x() / TILE_SIZE ) * my_zoom,
		( the_camera->get_y() / TILE_SIZE ) * my_zoom,
		( the_camera->get_w() / TILE_SIZE ) * my_zoom,
		( the_camera->get_h() / TILE_SIZE ) * my_zoom
	};

	if( cam_rect.x > my_area.w
			|| cam_rect.y > my_area.h )
	{
		return;
	}

	Uint32 rect_color = SDL_MapRGB( the_screen->format, 0x88, 0x88, 0x88 );

	for( int i = cam_rect.x; i <= cam_rect.x + cam_rect.w; i += 2 ) {
		if( i > my_area.w ) break;
		put_pixel( my_area.x + i, my_area.y + cam_rect.y,
				rect_color, the_screen );
		if( cam_rect.y + cam_rect.h > my_area.h ) continue;
		put_pixel( my_area.x + i, my_area.y + cam_rect.y + cam_rect.h,
				rect_color, the_screen );
	}

	for( int i = cam_rect.y; i <= cam_rect.y + cam_rect.h; i += 2 ) {
		if( i > my_area.h ) break;
		put_pixel( my_area.x + cam_rect.x, my_area.y + i,
				rect_color, the_screen );
		if( cam_rect.x + cam_rect.w > my_area.w ) continue;
		put_pixel( my_area.x + cam_rect.x + cam_rect.w, my_area.y + i,
				rect_color, the_screen );
	}
}

bool InterfaceMap::covers( int x, int y ) {
	if( x < my_area.x || x > my_area.x + my_area.w
			|| y < my_area.y || y > my_area.y + my_area.h )
	{
		return false;
	}
	else
		return true;
}

void InterfaceMap::on_click( int x, int y ) {
	printf( "InterfaceMap->Click!\n" );
	the_camera->center_at_tile( ( y - my_area.y ) / my_zoom,
			( x - my_area.x ) / my_zoom );
}
