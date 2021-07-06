#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>

using namespace std;

typedef pair <int, int> position;
typedef pair <char, vector <position> > movetype;
// typedef pair < pair <int, int >, pair <int, int >> movetype;

class Parser {
private:
	fstream file;
	queue <movetype> moves;
	vector <position> empty;
public:
	Parser(string filename) {
		file.open(filename);
	}

	void parse_moves() {
		while (true) {
			char type, row, column;
			// char row_1, row_2;
			// char column_1, column_2;

			// file >> type >> column_1 >> row_1 >> column_2 >> row_2;

			file >> type;

			if (file.eof()) {
				file.close();
				break;
			}

			// moves.push(make_pair(make_pair(int(column_1) - 65, row_1 - '0' - 1), make_pair(int(column_2) - 65, row_2 - '0' - 1)));

			if (type == 'M') {
				vector <position> tmp;

				file >> column >> row;
				tmp.push_back(make_pair(int(column) - 65, row - '0' - 1));

				file >> column >> row;
				tmp.push_back(make_pair(int(column) - 65, row - '0' - 1));

				cout << "From: " << tmp[0].first << tmp[0].second << " to: " << tmp[1].first << tmp[1].second << "\n";

				moves.push(make_pair('M', tmp));
			}

			else if (type == 'R') {
				vector <position> tmp;

				file >> column >> row;
				tmp.push_back(make_pair(int(column) - 65, row - '0' - 1));

				file >> column >> row;
				tmp.push_back(make_pair(int(column) - 65, row - '0' - 1));

				file >> column >> row;
				tmp.push_back(make_pair(int(column) - 65, row - '0' - 1));

				file >> column >> row;
				tmp.push_back(make_pair(int(column) - 65, row - '0' - 1));

				moves.push(make_pair('R', tmp));
			}
		}
	}

	movetype next_move() {
		if (moves.empty()) {
			return make_pair('0', empty);
		}

		movetype retval = moves.front();
		cout << "Type: " << retval.first << " From: " << retval.second[0].first << retval.second[0].second << " to: " << retval.second[1].first << retval.second[1].second << "\n";
		moves.pop();
		return retval;
	}
};

#endif