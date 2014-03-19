/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091030
 */

#ifndef STATE_HPP
#define STATE_HPP

#include <SDL/SDL.h>

class State {
    public:
        virtual void initialize( SDL_Surface* ) = 0;
        virtual void handle_events() = 0;
        virtual void update() = 0;
        virtual void draw() = 0;
		virtual bool get_is_running() = 0;
};

#endif
