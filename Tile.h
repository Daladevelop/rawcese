/*
 *	Licensed under GNU GPL v2
 *	Author(s):	Christofer Oden
 *	Created on:	091031
 */

#ifndef TILE_H
#define TILE_H

enum TileType {
	TILE_GRASS,
	TILE_WATER,
	TILE_TREES
};

class Tile {
	public:
		Tile( int row, int col, TileType tile_type ) {
			my_row	= row;
			my_col	= col;
			my_type = tile_type;
		}

		void	set_type( TileType tile_type ) { my_type = tile_type; }

		int get_row()		{ return my_row; }
		int get_col()		{ return my_col; }
		TileType get_type()	{ return my_type; }

	private:
		int my_row, my_col;
		TileType my_type;
};

#endif
