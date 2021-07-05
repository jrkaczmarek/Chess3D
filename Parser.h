#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

typedef pair < pair <int, int >, pair <int, int >> movetype;

class Parser {
private:
	fstream file;
	queue <movetype> moves;

public:
Parser(string filename) {
	file.open(filename);
	}

	void parse_moves() {
		while (true) {
			char row_1, row_2;
			char column_1, column_2;

			file >> column_1 >> row_1 >> column_2 >> row_2;

			if (file.eof()) {
				file.close();
				break;
			}

			moves.push(make_pair(make_pair(int(column_1) - 65, row_1 - '0' - 1), make_pair(int(column_2) - 65, row_2 - '0' - 1)));
		}
	}

	movetype next_move() {
		if (moves.empty()) {
			return make_pair(make_pair(-1, -1), make_pair(-1, -1));
		}

		movetype retval = moves.front();
		cout << "From: " << moves.front().first.first << moves.front().first.second << " To: " << moves.front().second.first << moves.front().second.second << "\n";
		moves.pop();
		return retval;
	}
};

#endif