#include "Player.h"

Player::Player(chess_colors type, int pawn_mesh,
	int bishop_mesh, int knight_mesh, int rook_mesh,
	int queen_mesh, int king_mesh, int cylinder_mesh) {
	this->pawn_mesh = pawn_mesh;
	this->bishop_mesh = bishop_mesh;
	this->knight_mesh = knight_mesh;
	this->rook_mesh = rook_mesh;
	this->queen_mesh = queen_mesh;
	this->king_mesh = king_mesh;
	this->type = type;
	this->cylinder_mesh = cylinder_mesh;

	for (int i = 0; i < 8; i++) {
		this->pawns[i] = nullptr;
	}

	for (int i = 0; i < 2; i++) {
		this->bishops[i] = nullptr;
		this->knights[i] = nullptr;
		this->rooks[i] = nullptr;
	}

	this->queen = nullptr;
	this->king = nullptr;
	this->cylinder = nullptr;
}

void Player::initialize(string file) {
	for (int i = 0; i < 8; i++) {
		this->pawns[i] = new my_model(file, this->pawn_mesh);
	}

	for (int i = 0; i < 2; i++) {
		this->bishops[i] = new my_model(file, this->bishop_mesh);
		this->knights[i] = new my_model(file, this->knight_mesh);
		this->rooks[i] = new my_model(file, this->rook_mesh);
	}

	this->queen = new my_model(file, this->queen_mesh);
	this->king = new my_model(file, this->king_mesh);
	this->cylinder = new my_model(file, this->cylinder_mesh);
}

chess_colors Player::get_type() {
	return this->type;
}