#include "Parser.h"

Parser::Parser(string filename) {
	this->file.open(filename);

	this->translate_moves['S'] = shift;
	this->translate_moves['R'] = castling;
	this->translate_moves['F'] = fall;
}

void Parser::parse_moves() {
	while (true) {
		char type, row, column;
		player_moves pl_mv;

		this->file >> type;

		if (this->file.eof()) {
			this->file.close();
			break;
		}

		pl_mv = translate_moves[type];

		vector <chessboard_position> positions;

		for (int i = 0; i < pl_mv; i++) {
			this->file >> column >> row;
			positions.push_back(make_pair(int(column) - 65, row - '0' - 1));
		}

		this->moves.push(make_pair(pl_mv, positions));
	}
}

movetype Parser::next_move() {
	if (this->moves.empty()) {
		return make_pair(still, this->no_positions);
	}

	movetype retval = this->moves.front();
	this->moves.pop();
	return retval;
}