/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	09102?
 */

#include "Camera.h"

void Camera::initialize( World *world, SDL_Rect *rect ) {
	the_world = world;

	my_rect.x = rect->x;
	my_rect.y = rect->y;
	my_rect.w = rect->w;
	my_rect.h = rect->h;

	my_movement_x = 0;
	my_movement_y = 0;
}

bool Camera::tile_is_in_scope( int row, int col ) {
	if( (col + 1) * TILE_SIZE < my_rect.x
			|| (row + 1) * TILE_SIZE < my_rect.y
			|| col * TILE_SIZE > my_rect.x + my_rect.w
			|| row * TILE_SIZE > my_rect.y + my_rect.h )
	{
		return false;
	}
	else
		return true;
}

bool Camera::xy_is_in_scope( int x, int y ) {
	if( x < my_rect.x || x > my_rect.x + my_rect.w
		|| y < my_rect.y || y > my_rect.y + my_rect.h )
	{
		return false;
	}
	else
		return true;
}

void Camera::update() {
	my_rect.x += my_movement_x;
	my_rect.y += my_movement_y;

	if ( my_rect.x < 0 )
		my_rect.x = 0;
	else if ( my_rect.x + my_rect.w
			> the_world->get_col_count() * TILE_SIZE )
		my_rect.x = the_world->get_col_count() * TILE_SIZE - my_rect.w;

	if ( my_rect.y < 0 )
		my_rect.y = 0;
	else if ( my_rect.y + my_rect.h
			> the_world->get_row_count() * TILE_SIZE )
		my_rect.y = the_world->get_row_count() * TILE_SIZE - my_rect.h;
}

void Camera::set_movement_x( int x ) {
	my_movement_x = x;	
}

void Camera::set_movement_y( int y ) {
	my_movement_y = y;	
}

void Camera::stop_movement_x() {
	my_movement_x = 0;	
}

void Camera::stop_movement_y() {
	my_movement_y = 0;	
}

void Camera::center_at_xy( int x, int y ) {
	printf( "Centering camera at (%d, %d)\n", x, y );
	my_rect.x = x - my_rect.w / 2;
	my_rect.y = y - my_rect.h / 2;
}

void Camera::center_at_tile( int row, int col ) {
	printf( "Centering camera at tile (%d, %d)\n", row, col );
	my_rect.y = TILE_SIZE * row - my_rect.h / 2;
	my_rect.x = TILE_SIZE * col - my_rect.w / 2;
}
