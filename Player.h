#ifndef PLAYER_H
#define PLAYER_H

#include "my_model.h"
#include "Types.h"

class Player {
private:
	int pawn_mesh;
	int bishop_mesh;
	int knight_mesh;
	int rook_mesh;
	int queen_mesh;
	int king_mesh;
	int cylinder_mesh;
	chess_colors type;
public:
	my_model* pawns[8];
	my_model* bishops[2];
	my_model* knights[2];
	my_model* rooks[2];
	my_model* queen;
	my_model* king;
	my_model* cylinder;

	Player(chess_colors type, int pawn_mesh,
		int bishop_mesh, int knight_mesh, int rook_mesh,
		int queen_mesh, int king_mesh, int cylinder_mesh);

	void initialize(string file);
	chess_colors get_type();
};

#endif
