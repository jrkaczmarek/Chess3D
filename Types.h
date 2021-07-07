#ifndef TYPES_H
#define TYPES_H

#include <utility>
#include <vector>

using namespace std;

enum player_moves { shift = 2, castling = 4, fall = 1, still = 0 };
enum chess_figures { pawn = 1, rook = 2, knight = 3, bishop = 4, queen = 5, king = 6, cylinder = 0 };
enum chess_colors { white = 0, black = 1 };

typedef pair <int, int> chessboard_position;
typedef pair <float, float> chessboard_coordinate;
typedef pair <player_moves, vector <chessboard_position> > movetype;

#endif 

