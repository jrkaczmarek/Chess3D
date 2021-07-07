#ifndef PARSER_H
#define PARSER_H

#include <fstream>
#include <queue>
#include <map>
#include "Types.h"

using namespace std;


class Parser {
private:
	fstream file;
	queue <movetype> moves;
	vector < chessboard_position > no_positions;
	map <char, player_moves> translate_moves;

public:
	Parser(string filename);
	void parse_moves();
	movetype next_move();
};

#endif