/*
 *	Rawcese aims to become a simple but yet intriguing turn based strategy game with dragons and tanks
 *	Licensed under GNU GPL v2
 *	Authors: Emil Österlund, Adam Nyberg, Christofer Beijar Oden
 *	Created on:	091022
 *	http://launchpad.net/rawcese
 */

#include "Timer.h"
#include "State.h"
#include "GameState.h"
#include "globals.h"

#include <ctime>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

int main( int argc, char **args )
{
	SDL_Surface *the_screen = NULL;
	Timer        timer;
	Timer		fps_timer;
	GameState	game_state;
	State		*current_state;
	int			frames = 0;

	printf( "---Initializing---\n" );
	srand( (unsigned)time(NULL) );

	printf( "Initializing SDL\n" );
	if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
		fprintf( stderr, "Failed to initialize SDL\n");
		return 1;
	}

	printf( "Initializing screen (%dx%dx%d)\n",
			SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP );
	the_screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT,
			SCREEN_BPP, SDL_SWSURFACE );
	if ( the_screen == NULL ) {
		printf( "SDL_SetVideoMode: %s\n", SDL_GetError( ) );
		return 1;
	}

	SDL_WM_SetCaption( "rawcese IMA EAT YOUR TANK. NOM NOM NOM. NO MORE TANK."
			" / DRAGONMASTER", NULL );

	printf( "Initializing timers\n" );
	timer.initialize( UPDATES_PER_SECOND );
	fps_timer.initialize( 1 );

	printf( "Initializing state \"Game\"\n" );
	game_state.initialize( the_screen );

	current_state = &game_state;

	printf( "---Starting main loop---\n" );
	fps_timer.set_mark();
	while( current_state->get_is_running() ) {
		timer.set_mark();
		//printf("Handling events\n");
		current_state->handle_events();
		//printf("Updating logic\n");
		current_state->update();
		//printf("Drawing\n");
		current_state->draw();
		++frames;
		if( fps_timer.threshold_reached() ) {
			printf( "FPS: %d\r", frames );
			frames = 0;
			fps_timer.set_mark();
			fflush( stdout );
		}
		timer.await_next_update();
	}

	SDL_FreeSurface( the_screen );
	SDL_Quit( );

	return 0;
}
