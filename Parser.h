#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <map>

enum player_moves { shift = 2, castling = 4, fall = 1, still = 0 };
enum chess_figures { pawn, rook, knight, bishop, queen, king, cylinder};
enum chess_colors { white, black };

typedef pair <int, int> chessboard_position;
typedef pair <float, float> chessboard_coordinate;
typedef pair <player_moves, vector <chessboard_position> > movetype;

using namespace std;

class Parser {
private:
	fstream file;
	queue <movetype> moves;
	vector < chessboard_position > no_positions;
	map <char, player_moves> translate_moves;

public:
	Parser(string filename) {
		file.open(filename);

		translate_moves['S'] = shift;
		translate_moves['R'] = castling;
		translate_moves['F'] = fall;
	}

	void parse_moves() {
		while (true) {
			char type, row, column;
			player_moves pl_mv;

			file >> type;

			if (file.eof()) {
				file.close();
				break;
			}

			pl_mv = translate_moves[type];

			vector <chessboard_position> positions;

			for (int i = 0; i < pl_mv; i++) {
				file >> column >> row;
				positions.push_back(make_pair(int(column) - 65, row - '0' - 1));
			}

			moves.push(make_pair(pl_mv, positions));
		}
	}

	movetype next_move() {
		if (moves.empty()) {
			return make_pair(still, no_positions);
		}

		movetype retval = moves.front();
		moves.pop();
		return retval;
	}
};

#endif