/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091031
 */

#include "GameState.h"
#include "Interface.h"
#include "World.h"

#include <vector>
#include <sstream>

void GameState::initialize( SDL_Surface *screen ) {
	the_screen = screen;

	printf( "---Loading game state data---\n" );
	my_sprite_map = load_image( "sprites/sprite_map.png" );
	my_font_map   = load_image( "sprites/font_map.png" );

	printf( "---Generating world---\n" );
	my_world.generate( 30, 30 );

	printf( "---Initializing players---\n" );
	my_player_manager.initialize( &my_world, 3
	 );

	printf( "---Setting up interface---\n" );
	my_interface.initialize( the_screen, &my_camera, &my_world,
			&my_player_manager );

	printf( "---Creating camera---\n");
	SDL_Rect camera_rect = { 0, 0,
		SCREEN_WIDTH - my_interface.get_rect()->w, SCREEN_HEIGHT };
	my_camera.initialize( &my_world, &camera_rect );

	i_am_running = true;
}

void GameState::handle_events() {
	SDL_Event event;

	while( SDL_PollEvent( &event ) ) {
		switch( event.type ) {

			case SDL_KEYDOWN:
			case SDL_KEYUP:
				handle_keyboard_event( &event );
				break;

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				handle_mouse_event( &event );
				break;

			case SDL_QUIT:
				set_is_running( false );
				break;

				// Unhandled events
		}
	}
}

void GameState::update() {
	my_player_manager.update();
	my_camera.update();
}

void GameState::draw() {
	//printf("GameState: Drawing tiles\n");
	draw_tiles();
	//printf("GameState: Drawing statics\n");
	draw_static_entities();
	//printf("GameState: Drawing mobiles\n");
	draw_mobile_entities();
	//printf("GameState: Drawing interface\n");
	draw_interface();

	if( SDL_Flip( the_screen ) < 0 ) {
		fprintf( stderr, "SDL_Flip: %s\n", SDL_GetError() );
	}
}

void GameState::handle_keyboard_event( SDL_Event *event ) {
	Player *current_player = my_player_manager.get_current_player();
	Unit *unit = NULL;

	switch( event->type ) {
		case SDL_KEYDOWN:
			switch( event->key.keysym.sym ) {
				case SDLK_ESCAPE:
					set_is_running( false );
					break;

				case SDLK_TAB:
					if( SDL_GetModState() & KMOD_SHIFT ) {
						current_player->select_prev_unit();
					}
					else {
						current_player->select_next_unit();
					}
					unit = current_player->get_selected_unit();
					if( unit != NULL ) {
						printf("Centering camera\n");
						my_camera.center_at_tile(
								unit->get_row(), unit->get_col() ); 
					}
					break;

				case SDLK_KP7:
					current_player->move_selected_unit( NW );
					break;

				case SDLK_UP:
				case SDLK_KP8:
					printf( "Up pressed\n" );
					current_player->move_selected_unit( N );  
					break;

				case SDLK_KP9:
					current_player->move_selected_unit( NE );
					break;

				case SDLK_LEFT:
				case SDLK_KP4:
					printf( "Left pressed\n" );
					current_player->move_selected_unit( W );
					break;

				case SDLK_RIGHT:
				case SDLK_KP6:
					printf( "Right pressed\n" );
					current_player->move_selected_unit( E );
					break;

				case SDLK_KP1:
					current_player->move_selected_unit( SW );
					break;

				case SDLK_DOWN:
				case SDLK_KP2:
					printf( "Down pressed\n" );
					current_player->move_selected_unit( S );
					break;

				case SDLK_KP3:
					current_player->move_selected_unit( SE );
					break;

				case SDLK_a:
					my_camera.set_movement_x( -5 );
					break;

				case SDLK_c:
					if( SDL_GetModState() & KMOD_CTRL )
						set_is_running( false );
					break;

				case SDLK_d:
					my_camera.set_movement_x( 5 );
					break;

				case SDLK_e:
					printf( "End turn\n" );
					my_player_manager.end_turn();
					printf( "Your turn player %d\n",
							my_player_manager.get_current_player()->get_id() );
					break;

				case SDLK_n:
					my_player_manager.get_current_player()->
						create_unit(
								rand() % my_world.get_row_count(),
								rand() % my_world.get_col_count(),
								UNIT_MGUNNERORC );
					printf("Unit count: %d\n",
							my_player_manager.get_current_player()->
							get_unit_list()->size() );
					break;

				case SDLK_s:
					my_camera.set_movement_y( 5 );
					break;

				case SDLK_w:
					my_camera.set_movement_y( -5 );
					break;

				default:
					break;
			}
			break;

		case SDL_KEYUP:
			switch( event->key.keysym.sym ) {
				case SDLK_w:
				case SDLK_s:
					my_camera.stop_movement_y();
					break;

				case SDLK_a:
				case SDLK_d:
					my_camera.stop_movement_x();
					break;

				default:
					break;
			}
			break;

		default:
			fprintf( stderr, "Unhandled keyboard event.type,\n"
					" this shouldn't happen!\n" );
			fprintf( stderr, "File: %s, line: %d\n", __FILE__, __LINE__ );
	}
}

void GameState::handle_mouse_event( SDL_Event *event ) {
	Player *current_player = my_player_manager.get_current_player();

	switch( event->type ) {
		case SDL_MOUSEBUTTONDOWN:
			switch( event->button.button ) {
				case SDL_BUTTON_LEFT:
					if( my_interface.covers( event->button.x,
								event->button.y ) )
					{
						my_interface.on_click( event->button.x,
								event->button.y );
					}
					else {
						current_player->select_unit_at_xy(
								my_camera.get_rect()->x + event->button.x,
								my_camera.get_rect()->y + event->button.y );
					}
					break;
				case SDL_BUTTON_RIGHT:
					my_camera.center_at_xy(
							my_camera.get_rect()->x + event->button.x,
							my_camera.get_rect()->y + event->button.y );
					break;
			}
			break;

		case SDL_MOUSEBUTTONUP:
			break;

		case SDL_MOUSEMOTION:
			break;

		default:
			fprintf( stderr, "Unhandled mouse event.type,\n"
					" this shouldn't happen!\n" );
			fprintf( stderr, "File: %s, line: %d\n", __FILE__, __LINE__ );
	}
}

void GameState::draw_tiles() {
	int		row_count		= my_world.get_row_count();
	int		col_count		= my_world.get_col_count();
	Player	*current_player	= my_player_manager.get_current_player();

	for( int row = 0; row < row_count; ++row ) {
		for( int col = 0; col < col_count; ++col ) {
			if( my_camera.tile_is_in_scope( row, col ) == false )
				continue;

			if ( current_player->has_discovered_tile( row, col ) == false ) {
				SDL_Rect temp_rect = { col * TILE_SIZE - my_camera.get_x(),
					row * TILE_SIZE - my_camera.get_y(),
					TILE_SIZE, TILE_SIZE };
				//printf( "Draw tiles: Drawing undiscovered tile\n");
				SDL_FillRect( the_screen, &temp_rect,
						SDL_MapRGB( the_screen->format, 0x00, 0x00, 0x00 ) );
				continue;
			}

			//printf( "Draw tiles: Drawing terrain tile\n");
			switch( my_world.get_tile_type( row, col ) ) {
				case TILE_GRASS:
					apply_surface( col * TILE_SIZE - my_camera.get_x(),
							row * TILE_SIZE - my_camera.get_y(),
							my_sprite_map, the_screen, &RECT_GRASS );
					break;

				case TILE_WATER:
					draw_water_sprites( row, col );
					break;

				case TILE_TREES:
					draw_tree_sprites( row, col );
					break;

				default:
					break;
			}

			/*
			if( !current_player->can_see_tile( row, col ) ) {
				SDL_Rect temp_rect = { col * TILE_SIZE - my_camera.get_x(),
					row * TILE_SIZE - my_camera.get_y(),
					TILE_SIZE, TILE_SIZE };
				SDL_FillRect( the_screen, &temp_rect,
						SDL_MapRGBA( the_screen->format,
							0x00, 0x00, 0x00, 0xff ) );
			}
			*/

			if( !current_player->can_see_tile( row, col ) ) {
                apply_surface(
                        col * TILE_SIZE - my_camera.get_x(),
                        row * TILE_SIZE - my_camera.get_y(),
                        my_sprite_map, the_screen, &RECT_SHROUD );
			}
		}
	}
}

void GameState::draw_static_entities() {
	/*
	   .
	   .
	   .
	 * empty *
	 .
	 .
	 .
	 */
}

void GameState::draw_mobile_entities() {
	std::vector<Player> *player_list = my_player_manager.get_player_list();
	Player *current_player			 = my_player_manager.get_current_player();

	for( PlayerIterator p_it = player_list->begin();
			p_it < player_list->end(); ++p_it )
	{
		std::vector<Unit> *unit_list = p_it->get_unit_list();
		for( UnitIterator u_it = unit_list->begin();
				u_it < unit_list->end(); ++u_it )
		{
			int row = u_it->get_row();
			int col = u_it->get_col();

			if( !my_camera.tile_is_in_scope( row, col ) ) {
				continue;
			}

			if( !current_player->can_see_tile( row, col ) ) {
				continue;
			}


			draw_unit_sprite( col * TILE_SIZE - my_camera.get_x(),
					row * TILE_SIZE - my_camera.get_y(),
					u_it->get_type(), u_it->get_owner_id() );
		}
	}

	for( PlayerIterator p_it = player_list->begin();
			p_it < player_list->end(); ++p_it )
	{
		std::vector<Unit> *unit_list = p_it->get_unit_list();
		for( UnitIterator u_it = unit_list->begin();
				u_it < unit_list->end(); ++u_it )
		{
			int row = u_it->get_row();
			int col = u_it->get_col();

			if( !my_camera.tile_is_in_scope( row, col ) ) {
				continue;
			}

			if( !current_player->can_see_tile( row, col ) ) {
				continue;
			}

			SDL_Rect temp_bar = { col * TILE_SIZE - my_camera.get_x() + 4,
				row * TILE_SIZE - my_camera.get_y() - 4,
				24, 4 };
			draw_health_bar( &temp_bar,
					u_it->get_health(), u_it->get_maxhealth() );
		}
	}
}

void GameState::draw_unit_sprite( int x, int y,
		UnitType unit_type, int player_id )
{
	SDL_Rect rect;

	switch ( unit_type ) {
		case UNIT_DRAGON:
			rectcpy( &rect, &RECT_DRAGON );
			rect.y += rect.h * player_id;
			apply_surface( x, y, my_sprite_map, the_screen, &rect );
			break;

		case UNIT_INFANTRY:
			rectcpy( &rect, &RECT_INFANTRY );
			rect.y += rect.h * player_id;
			apply_surface( x, y, my_sprite_map, the_screen, &rect );
			break;

		case UNIT_ARMOR:
			rectcpy( &rect, &RECT_ARMOR );
			rect.y += rect.h * player_id;
			apply_surface( x, y, my_sprite_map, the_screen, &rect );
			break;

		case UNIT_HOWITZER:
			rectcpy( &rect, &RECT_HOWITZER );
			rect.y += rect.h * player_id;
			apply_surface( x, y, my_sprite_map, the_screen, &rect );
			break;

		case UNIT_MGUNNERORC:
			rectcpy( &rect, &RECT_MGUNNERORC );
			rect.y += rect.h * player_id;
			apply_surface( x, y, my_sprite_map, the_screen, &rect );
			break;

		case UNIT_BEHOLDER:
			rectcpy( &rect, &RECT_BEHOLDER );
			apply_surface( x, y, my_sprite_map, the_screen, &rect );
			break;

		default:
			fprintf( stderr, "I was told to draw an unrecognized unit type!\n"
					" This shouldn't happen, fix it!\n");
			fprintf( stderr, "File: %s, line: %d\n", __FILE__, __LINE__ );
	}
}

void GameState::draw_health_bar( SDL_Rect *box, int health, int max_health )
{
	if( box->y < 0 )
		return;

    double ratio = double(health) / double(max_health);

	SDL_FillRect( the_screen, box,
			SDL_MapRGB( the_screen->format, 0xc0, 0xb0, 0xb0 ) );

	box->x += 1;
	box->y += 1;
	box->w -= 2;
	box->h -= 2;

	SDL_FillRect( the_screen, box,
			SDL_MapRGB( the_screen->format, 0x20, 0x00, 0x00 ) );

	box->w *= ratio;

	SDL_FillRect( the_screen, box,
			SDL_MapRGB( the_screen->format, 0x80, 0x00, 0x00 ) );
}

void GameState::draw_interface() {
    Unit *unit = my_player_manager.get_current_player()->get_selected_unit();

    if( unit != NULL ) {
        int row = unit->get_row();
        int col = unit->get_col();

		if( my_camera.tile_is_in_scope( row, col ) == true ) {
			apply_surface( col * TILE_SIZE - my_camera.get_x(),
					row * TILE_SIZE - my_camera.get_y(),
					my_sprite_map, the_screen, &RECT_MARKER );
		}
    }

	my_interface.draw();
	if( unit != NULL )
	{
		std::stringstream ss;

		ss << "Health: " << unit->get_health() << " of "
			<< unit->get_maxhealth();
		blit_string( SCREEN_WIDTH - 90, 200, ss.str().c_str(),
				the_screen, my_font_map );

		ss.str("");
		ss << "Moves: " << unit->get_moves();
		blit_string( SCREEN_WIDTH - 90, 210, ss.str().c_str(),
				the_screen, my_font_map );
	}
}

void GameState::draw_water_sprites( int row, int col ) {
	SDL_Rect rect;
	// Draw NW tile
	// Check northern and western tile
	if( my_world.get_tile_type( row - 1, col ) == TILE_WATER &&
			my_world.get_tile_type( row, col - 1 ) == TILE_WATER )
	{
		if( my_world.get_tile_type( row - 1, col - 1 ) != TILE_WATER )
			rect = RECT_OUTER_CLIFF_NW;
		else
			rect = RECT_WATER;
	}
	else if( my_world.get_tile_type( row - 1, col ) != TILE_WATER &&
			my_world.get_tile_type( row, col - 1 ) == TILE_WATER )
	{
		rect = RECT_CLIFF_N;
	}
	else if( my_world.get_tile_type( row - 1, col ) == TILE_WATER &&
			my_world.get_tile_type( row, col - 1 ) != TILE_WATER )
	{
		rect = RECT_CLIFF_W;
	}
	else if( my_world.get_tile_type( row - 1, col ) != TILE_WATER &&
			my_world.get_tile_type( row, col - 1 ) != TILE_WATER )
	{
		rect = RECT_CLIFF_NW;
	}

	//    printf( "DEBUG: Drawing NW tile\n" );
	apply_surface(
			col * TILE_SIZE - my_camera.get_x(),
			row * TILE_SIZE - my_camera.get_y(),
			my_sprite_map, the_screen, &rect );

	// Draw NE tile
	// Check northern and eastern tile
	if (    my_world.get_tile_type( row - 1, col ) == TILE_WATER &&
			my_world.get_tile_type( row, col + 1 ) == TILE_WATER )

		if ( my_world.get_tile_type( row - 1, col + 1 ) != TILE_WATER )
			rect = RECT_OUTER_CLIFF_NE;
		else
			rect = RECT_WATER;

	else if( my_world.get_tile_type( row - 1, col ) != TILE_WATER &&
			my_world.get_tile_type( row, col + 1 ) == TILE_WATER )

		rect = RECT_CLIFF_N;

	else if( my_world.get_tile_type( row - 1, col ) == TILE_WATER &&
			my_world.get_tile_type( row, col + 1 ) != TILE_WATER )

		rect = RECT_CLIFF_E;

	else if( my_world.get_tile_type( row - 1, col ) != TILE_WATER &&
			my_world.get_tile_type( row, col + 1 ) != TILE_WATER )

		rect = RECT_CLIFF_NE;

	//    printf( "DEBUG: Drawing NE tile\n" );
	apply_surface(
			col * TILE_SIZE + 16 - my_camera.get_x(),
			row * TILE_SIZE - my_camera.get_y(),
			my_sprite_map, the_screen, &rect );

	// Draw SW tile
	// Check southern and western tile
	if (    my_world.get_tile_type( row + 1, col ) == TILE_WATER &&
			my_world.get_tile_type( row, col - 1 ) == TILE_WATER )

		if ( my_world.get_tile_type( row + 1, col - 1 ) != TILE_WATER )
			rect = RECT_OUTER_CLIFF_SW;
		else
			rect = RECT_WATER;

	else if( my_world.get_tile_type( row + 1, col ) != TILE_WATER &&
			my_world.get_tile_type( row, col - 1 ) == TILE_WATER )

		rect = RECT_CLIFF_S;

	else if( my_world.get_tile_type( row + 1, col ) == TILE_WATER &&
			my_world.get_tile_type( row, col - 1 ) != TILE_WATER )

		rect = RECT_CLIFF_W;

	else if( my_world.get_tile_type( row + 1, col ) != TILE_WATER &&
			my_world.get_tile_type( row, col - 1 ) != TILE_WATER )

		rect = RECT_CLIFF_SW;

	//    printf( "DEBUG: Drawing SW tile\n" );
	apply_surface(
			col * TILE_SIZE - my_camera.get_x(),
			row * TILE_SIZE + 16 - my_camera.get_y(),
			my_sprite_map, the_screen, &rect );

	// Draw SE tile
	// Check southern and eastern tile
	if (    my_world.get_tile_type( row + 1, col ) == TILE_WATER &&
			my_world.get_tile_type( row, col + 1 ) == TILE_WATER )

		if ( my_world.get_tile_type( row + 1, col + 1 ) != TILE_WATER )
			rect = RECT_OUTER_CLIFF_SE;
		else
			rect = RECT_WATER;

	else if( my_world.get_tile_type( row + 1, col ) != TILE_WATER &&
			my_world.get_tile_type( row, col + 1 ) == TILE_WATER )

		rect = RECT_CLIFF_S;

	else if( my_world.get_tile_type( row + 1, col ) == TILE_WATER &&
			my_world.get_tile_type( row, col + 1 ) != TILE_WATER )

		rect = RECT_CLIFF_E;

	else if( my_world.get_tile_type( row + 1, col ) != TILE_WATER &&
			my_world.get_tile_type( row, col + 1 ) != TILE_WATER )

		rect = RECT_CLIFF_SE;

	//    printf( "DEBUG: Drawing SE tile\n" );
	apply_surface(
			col * TILE_SIZE + 16 - my_camera.get_x(),
			row * TILE_SIZE + 16 - my_camera.get_y(),
			my_sprite_map, the_screen, &rect );
}

void GameState::draw_tree_sprites( int row, int col ) {
	SDL_Rect rect;
	// Draw NW tile
	// Check northern and western tile
	if( my_world.get_tile_type( row - 1, col ) == TILE_TREES
			&& my_world.get_tile_type( row, col - 1 ) == TILE_TREES )
	{
		rect = RECT_TREES;
	}
	else if( my_world.get_tile_type( row - 1, col ) != TILE_TREES
			&& my_world.get_tile_type( row, col - 1 ) == TILE_TREES )
	{
		rect = RECT_TREES_N;
	}
	else if( my_world.get_tile_type( row - 1, col ) == TILE_TREES
			&& my_world.get_tile_type( row, col - 1 ) != TILE_TREES )
	{
		rect = RECT_TREES_W;
	}
	else if( my_world.get_tile_type( row - 1, col ) != TILE_TREES
			&& my_world.get_tile_type( row, col - 1 ) != TILE_TREES )
	{
		rect = RECT_TREES_NW;
	}

	apply_surface(col * TILE_SIZE - my_camera.get_x(),
			row * TILE_SIZE - my_camera.get_y(),
			my_sprite_map, the_screen, &rect );

	// Draw NE tile
	// Check northern and eastern tile
	if (    my_world.get_tile_type( row - 1, col ) == TILE_TREES &&
			my_world.get_tile_type( row, col + 1 ) == TILE_TREES )

		rect = RECT_TREES;

	else if( my_world.get_tile_type( row - 1, col ) != TILE_TREES &&
			my_world.get_tile_type( row, col + 1 ) == TILE_TREES )

		rect = RECT_TREES_N;

	else if( my_world.get_tile_type( row - 1, col ) == TILE_TREES &&
			my_world.get_tile_type( row, col + 1 ) != TILE_TREES )

		rect = RECT_TREES_E;

	else if( my_world.get_tile_type( row - 1, col ) != TILE_TREES &&
			my_world.get_tile_type( row, col + 1 ) != TILE_TREES )

		rect = RECT_TREES_NE;

	apply_surface(
			col * TILE_SIZE + 16 - my_camera.get_x(),
			row * TILE_SIZE - my_camera.get_y(),
			my_sprite_map, the_screen, &rect );

	// Draw SW tile
	// Check southern and western tile
	if (    my_world.get_tile_type( row + 1, col ) == TILE_TREES &&
			my_world.get_tile_type( row, col - 1 ) == TILE_TREES )

		rect = RECT_TREES;

	else if( my_world.get_tile_type( row + 1, col ) != TILE_TREES &&
			my_world.get_tile_type( row, col - 1 ) == TILE_TREES )

		rect = RECT_TREES_S;

	else if( my_world.get_tile_type( row + 1, col ) == TILE_TREES &&
			my_world.get_tile_type( row, col - 1 ) != TILE_TREES )

		rect = RECT_TREES_W;

	else if( my_world.get_tile_type( row + 1, col ) != TILE_TREES &&
			my_world.get_tile_type( row, col - 1 ) != TILE_TREES )

		rect = RECT_TREES_SW;

	apply_surface(
			col * TILE_SIZE - my_camera.get_x(),
			row * TILE_SIZE + 16 - my_camera.get_y(),
			my_sprite_map, the_screen, &rect );

	// Draw SE tile
	// Check southern and eastern tile
	if (    my_world.get_tile_type( row + 1, col ) == TILE_TREES &&
			my_world.get_tile_type( row, col + 1 ) == TILE_TREES )

		rect = RECT_TREES;

	else if( my_world.get_tile_type( row + 1, col ) != TILE_TREES &&
			my_world.get_tile_type( row, col + 1 ) == TILE_TREES )

		rect = RECT_TREES_S;

	else if( my_world.get_tile_type( row + 1, col ) == TILE_TREES &&
			my_world.get_tile_type( row, col + 1 ) != TILE_TREES )

		rect = RECT_TREES_E;

	else if( my_world.get_tile_type( row + 1, col ) != TILE_TREES &&
			my_world.get_tile_type( row, col + 1 ) != TILE_TREES )

		rect = RECT_TREES_SE;

	apply_surface(
			col * TILE_SIZE + 16 - my_camera.get_x(),
			row * TILE_SIZE + 16 - my_camera.get_y(),
			my_sprite_map, the_screen, &rect );

}
