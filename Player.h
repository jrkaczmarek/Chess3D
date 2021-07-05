#ifndef PLAYER_H
#define PLAYER_H

#include "my_model.h"

class Player {
private:
	int pawn_mesh;
	int bishop_mesh;
	int knight_mesh;
	int rook_mesh;
	int queen_mesh;
	int king_mesh;
	string type;
public:
	my_model* pawns[8];
	my_model* bishops[2];
	my_model* knights[2];
	my_model* rooks[2];
	my_model* queen;
	my_model* king;

	Player(string type, int pawn_mesh, int bishop_mesh, int knight_mesh, int rook_mesh, int queen_mesh, int king_mesh) {
		this->pawn_mesh = pawn_mesh;
		this->bishop_mesh = bishop_mesh;
		this->knight_mesh = knight_mesh;
		this->rook_mesh = rook_mesh;
		this->queen_mesh = queen_mesh;
		this->king_mesh = king_mesh;
		this->type = type;
	}

	void initialize(string file) {
		for (int i = 0; i < 8; i++) {
			this->pawns[i] = new my_model(file, pawn_mesh);
		}

		for (int i = 0; i < 2; i++) {
			this->bishops[i] = new my_model(file, bishop_mesh);
			this->knights[i] = new my_model(file, knight_mesh);
			this->rooks[i] = new my_model(file, rook_mesh);
		}

		this->queen = new my_model(file, queen_mesh);
		this->king = new my_model(file, king_mesh);
	}

	string get_type() {
		return this->type;
	}
};

#endif
