#ifndef SETTING_H
#define SETTING_H

#include "Player.h"
#include <utility>
#include <map>

using namespace std;

class Setting {
private:
	my_model* chessboard[8][8];
	chessboard_coordinate chessboard_coordinates[8][8];
	map <my_model*, chess_colors> colors;
	map <my_model*, chess_figures> figures;

	my_model* WHITE_CYLINDER;
	my_model* BLACK_CYLINDER;

	my_model* white_captured[8][2];
	my_model* black_captured[8][2];

	chessboard_coordinate white_captured_coordinates[8][2];
	chessboard_coordinate black_captured_coordinates[8][2];

	bool is_moving;
	int accuracy;
	int step;
	movetype current_move;

	my_model* FIGURE_TO_MOVE, * FIGURE_TO_MOVE_2;
	chessboard_position FROM, FROM_2;
	chessboard_position TO, TO_2;
	pair <float, float> DIRECTION, DIRECTION_2;
	float PATH_LENGTH, PATH_LENGTH_2;
	float H, A, B, C;

	void capture(my_model* figure);
	void initialize_pawns(Player P1, Player P2);
	void initialize_rooks(Player P1, Player P2);
	void initialize_knights(Player P1, Player P2);
	void initialize_bishops(Player P1, Player P2);
	void initialize_queens(Player P1, Player P2);
	void initialize_kings(Player P1, Player P2);
	void initialize_cylinders(Player P1, Player P2);

	void draw_shift(glm::mat4 M, GLuint texture,
		my_model* figure_to_move, chessboard_position from, chessboard_position to,
		float rotation, pair < float, float> direction, float path_legnth, float a, float b, float c);
	void draw_castling(glm::mat4 M, GLuint texture, float rotation);
	void draw_fall(glm::mat4 M, GLuint texture, GLuint tex_cloth, float rotation);

	pair <float, float>  calc_direction(chessboard_position X, chessboard_position Y);
	float calc_path_length(pair <float, float> d);

public:
	Setting();

	void initialize(Player P1, Player P2, int accuracy);
	void draw_board(glm::mat4 M, GLuint tex_light, GLuint tex_dark);
	void draw_captured(glm::mat4 M, GLuint tex_light, GLuint tex_dark);
	void draw_move(glm::mat4 M, GLuint tex_light, GLuint tex_dark, GLuint tex_cloth);
	void next_move(movetype move);
	bool is_board_static();
};

#endif
