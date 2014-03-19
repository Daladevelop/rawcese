#include "Timer.h"
#include <SDL/SDL.h>

void Timer::initialize( int updates_per_second ) {
	my_ticks_per_update = int( 1000.0 / updates_per_second + 0.5 );
	printf( "Timer initialized:\n"
			"Updates per second:              %d =>\n"
			"Ticks (milliseconds) per update: %d\n",
			updates_per_second,
			my_ticks_per_update );
}

void Timer::set_mark() {
	my_ticks_at_mark = SDL_GetTicks();
}

inline
int Timer::get_delta_ticks() {
	return ( SDL_GetTicks() - my_ticks_at_mark );
}

void Timer::await_next_update() {
	int dt = get_delta_ticks();
	if( dt > my_ticks_per_update ) {
		return;
	}
	SDL_Delay( my_ticks_per_update - dt );
}

bool Timer::threshold_reached() {
	if( get_delta_ticks() > my_ticks_per_update )
		return true;
	else
		return false;
}
