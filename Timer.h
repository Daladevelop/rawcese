/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091102
 */

#ifndef TIMER_H
#define TIMER_H

class Timer {
	public:
		void	initialize( int updates_per_second );

		void	set_mark();
		int		get_delta_ticks();
		void	await_next_update();
		bool	threshold_reached();

	private:
		int		my_ticks_at_mark;
		int		my_ticks_per_update;
};

#endif
