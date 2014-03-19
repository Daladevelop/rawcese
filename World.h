/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Adam Nyberg, Christofer Oden, Emil Österlund
 *	Created on:	09102?
 */

#ifndef WORLD_H
#define WORLD_H

#include "Tile.h"
#include "globals.h"

#include <vector>

class World {
	public:
		void generate( int row_count, int col_count,
				TileType default_tile = TILE_WATER );
		void load( const char *path );

		bool is_out_of_bounds( int row, int col );
		std::vector<Tile> *get_tiles() { return &my_tiles; }
		TileType get_tile_type( int row, int col );
		int  get_row_count() { return my_row_count; }
		int  get_col_count() { return my_col_count; }

		void set_tile_type( int row, int col, TileType type );

	private:
		int			my_row_count, my_col_count;
		TileType	my_default_tile;
		std::vector<Tile> my_tiles; 
};

#endif
