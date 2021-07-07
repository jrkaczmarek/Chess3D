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

	void capture(my_model* figure) {
		if (colors[figure] == black) {
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 8; i++) {
					if (this->white_captured[i][j] == nullptr) {
						this->white_captured[i][j] = figure;
						return;
					}
				}
			}
		}
		else if (colors[figure] == white) {
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 8; i++) {
					if (this->black_captured[i][j] == nullptr) {
						this->black_captured[i][j] = figure;
						return;
					}
				}
			}
		}
		return;
	}

	void initialize_pawns(Player P1, Player P2) {
		for (int i = 0; i < 8; i++) {
			this->chessboard[i][1] = P1.pawns[i];
			this->colors[P1.pawns[i]] = white;
			this->figures[P1.pawns[i]] = pawn;
			this->chessboard[i][6] = P2.pawns[i];
			this->colors[P2.pawns[i]] = black;
			this->figures[P2.pawns[i]] = pawn;
		}
	}

	void initialize_rooks(Player P1, Player P2) {
		this->chessboard[0][0] = P1.rooks[0];
		this->colors[P1.rooks[0]] = white;
		this->figures[P1.rooks[0]] = rook;
		this->chessboard[7][0] = P1.rooks[1];
		this->colors[P1.rooks[1]] = white;
		this->figures[P1.rooks[1]] = rook;

		this->chessboard[0][7] = P2.rooks[0];
		this->colors[P2.rooks[0]] = black;
		this->figures[P2.rooks[0]] = rook;
		this->chessboard[7][7] = P2.rooks[1];
		this->colors[P2.rooks[1]] = black;
		this->figures[P2.rooks[1]] = rook;
	}

	void initialize_knights(Player P1, Player P2) {
		this->chessboard[1][0] = P1.knights[0];
		this->colors[P1.knights[0]] = white;
		this->figures[P1.knights[0]] = knight;
		this->chessboard[6][0] = P1.knights[1];
		this->colors[P1.knights[1]] = white;
		this->figures[P1.knights[1]] = knight;

		this->chessboard[1][7] = P2.knights[0];
		this->colors[P2.knights[0]] = black;
		this->figures[P2.knights[0]] = knight;
		this->chessboard[6][7] = P2.knights[1];
		this->colors[P2.knights[1]] = black;
		this->figures[P2.knights[1]] = knight;
	}

	void initialize_bishops(Player P1, Player P2) {
		this->chessboard[2][0] = P1.bishops[0];
		this->colors[P1.bishops[0]] = white;
		this->figures[P1.bishops[0]] = bishop;
		this->chessboard[5][0] = P1.bishops[1];
		this->colors[P1.bishops[1]] = white;
		this->figures[P1.bishops[1]] = bishop;

		this->chessboard[2][7] = P2.bishops[0];
		this->colors[P2.bishops[0]] = black;
		this->figures[P2.bishops[0]] = bishop;
		this->chessboard[5][7] = P2.bishops[1];
		this->colors[P2.bishops[1]] = black;
		this->figures[P2.bishops[1]] = bishop;
	}

	void initialize_queens(Player P1, Player P2) {
		this->chessboard[3][0] = P1.queen;
		this->colors[P1.queen] = white;
		this->figures[P1.queen] = queen;

		this->chessboard[3][7] = P2.queen;
		this->colors[P2.queen] = black;
		this->figures[P2.queen] = queen;
	}

	void initialize_kings(Player P1, Player P2) {
		this->chessboard[4][0] = P1.king;
		this->colors[P1.king] = white;
		this->figures[P1.king] = king;

		this->chessboard[4][7] = P2.king;
		this->colors[P2.king] = black;
		this->figures[P2.king] = king;
	}

	void initialize_cylinders(Player P1, Player P2) {
		this->WHITE_CYLINDER = P1.cylinder;
		this->colors[P1.cylinder] = white;
		this->figures[P1.cylinder] = cylinder;

		this->BLACK_CYLINDER = P2.cylinder;
		this->colors[P2.cylinder] = black;
		this->figures[P2.cylinder] = cylinder;
	}

	void draw_shift(glm::mat4 M, GLuint texture,
		my_model* figure_to_move, chessboard_position from, chessboard_position to,
		float rotation, pair < float, float> direction, float path_legnth, float a, float b, float c)
	{
		float nx = chessboard_coordinates[from.first][from.second].first + (direction.first * step) / accuracy;
		float ny = chessboard_coordinates[from.first][from.second].second + (direction.second * step) / accuracy;
		float nz = a * pow((float)step / accuracy * path_legnth, 2) + b * (float)step / accuracy * path_legnth + c;

		glm::mat4 nM = glm::translate(M, glm::vec3(nx, ny, nz));
		nM = glm::rotate(nM, rotation * PI / 180, glm::vec3(0, 0, 1));
		figure_to_move->draw(nM, texture);

		if (this->step == this->accuracy) {

			if (chessboard[to.first][to.second] != nullptr) {
				capture(chessboard[to.first][to.second]);
			}

			this->chessboard[to.first][to.second] = figure_to_move;
		}

	}

	void draw_castling(glm::mat4 M, GLuint texture, float rotation) {
		draw_shift(M, texture, this->FIGURE_TO_MOVE, this->FROM, this->TO, rotation, this->DIRECTION, this->PATH_LENGTH, this->A, this->B, this->C);
		draw_shift(M, texture, this->FIGURE_TO_MOVE_2, this->FROM_2, this->TO_2, rotation, this->DIRECTION_2, this->PATH_LENGTH_2, 0, 0, 0);
	}

	void draw_fall(glm::mat4 M, GLuint texture, GLuint tex_cloth, float rotation) {
		float radius = 0.02;
		float alpha = (float)90 * this->step / this->accuracy;
		alpha = alpha * PI / 180;

		float nx = this->chessboard_coordinates[FROM.first][FROM.second].first;
		float ny = this->chessboard_coordinates[FROM.first][FROM.second].second;
		float nz = 0;

		glm::mat4 nM = glm::translate(M, glm::vec3(nx, ny, nz));
		nM = glm::rotate(nM, rotation * PI / 180, glm::vec3(0, 0, 1));

		nx = - radius + cos(alpha) * radius;
		nz = sin(alpha) * radius;

		nM = glm::translate(nM, glm::vec3(nx, 0, nz));
		nM = glm::rotate(nM, 35 * PI / 180, glm::vec3(0, 0, 1));
		nM = glm::rotate(nM, -alpha, glm::vec3(0, 1, 0));
		nM = glm::rotate(nM, (float)step / accuracy * 180 * PI / 180, glm::vec3(0, 0, 1));

		this->FIGURE_TO_MOVE->draw(nM, texture);
		
		if (this->colors[this->FIGURE_TO_MOVE] == white) {
			this->WHITE_CYLINDER->draw(nM, tex_cloth);
		}
		else if (this->colors[this->FIGURE_TO_MOVE] == black) {
			this->BLACK_CYLINDER->draw(nM, tex_cloth);
		}

		if (this->step == this->accuracy) {
			this->step = step - 1;
		}
	}

	pair <float, float>  calc_direction(chessboard_position X, chessboard_position Y) {
		float u = chessboard_coordinates[Y.first][Y.second].first - chessboard_coordinates[X.first][X.second].first;
		float v = chessboard_coordinates[Y.first][Y.second].second - chessboard_coordinates[X.first][X.second].second;
		return make_pair(u, v);
	}

	float calc_path_length(pair <float, float> d) {
		return sqrt(pow((d.first), 2) + pow((d.second), 2));
	}

public:
	Setting() {
		this->is_moving = false;
		this->accuracy = 0;
		this->FIGURE_TO_MOVE = nullptr;
		this->H = 0.08;

		chessboard_coordinate crd = make_pair(-0.21f, -0.21f);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				this->chessboard[i][j] = nullptr;
				this->chessboard_coordinates[i][j] = crd;
				crd.second = crd.second + 0.06f;
			}
			crd.first = crd.first + 0.06f;
			crd.second = -0.21f;
		}

		crd = make_pair(-0.21f, -0.33f);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 2; j++) {
				this->white_captured[i][j] = nullptr;
				this->white_captured_coordinates[i][j] = crd;
				crd.second = crd.second - 0.06f;
			}
			crd.first = crd.first + 0.06f;
			crd.second = -0.33f;
		}

		crd = make_pair(0.21f, 0.33f);

		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 2; j++) {
				this->black_captured[i][j] = nullptr;
				this->black_captured_coordinates[i][j] = crd;
				crd.second = crd.second + 0.06f;
			}
			crd.first = crd.first - 0.06f;
			crd.second = 0.33f;
		}
	}

	void initialize(Player P1, Player P2, int accuracy) {
		this->accuracy = accuracy;

		if (P1.get_type() != white) {
			Player tmp = P1;
			P1 = P2;
			P2 = tmp;
		}

		initialize_pawns(P1, P2);
		initialize_rooks(P1, P2);
		initialize_knights(P1, P2);
		initialize_bishops(P1, P2);
		initialize_queens(P1, P2);
		initialize_kings(P1, P2);
		initialize_cylinders(P1, P2);
	}

	void draw_board(glm::mat4 M, GLuint tex_light, GLuint tex_dark) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (this->chessboard[i][j] != nullptr) {
					if (this->colors[chessboard[i][j]] == white) {
						glm::mat4 nM = glm::translate(M, glm::vec3(this->chessboard_coordinates[i][j].first, chessboard_coordinates[i][j].second, 0));
						nM = glm::rotate(nM, -90 * PI / 180, glm::vec3(0, 0, 1));
						chessboard[i][j]->draw(nM, tex_light);
					}
					else if (this->colors[chessboard[i][j]] == black) {
						glm::mat4 nM = glm::translate(M, glm::vec3(this->chessboard_coordinates[i][j].first, chessboard_coordinates[i][j].second, 0));
						nM = glm::rotate(nM, 90 * PI / 180, glm::vec3(0, 0, 1));
						chessboard[i][j]->draw(nM, tex_dark);
					}
				}
			}
		}
	}

	void draw_captured(glm::mat4 M, GLuint tex_light, GLuint tex_dark) {
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				if (this->white_captured[i][j] != nullptr) {
					glm::mat4 nM = glm::translate(M, glm::vec3(white_captured_coordinates[i][j].first, white_captured_coordinates[i][j].second, 0));
					nM = glm::rotate(nM, -90 * PI / 180, glm::vec3(0, 0, 1));
					white_captured[i][j]->draw(nM, tex_dark);
				}
				if (this->black_captured[i][j] != nullptr) {
					glm::mat4 nM = glm::translate(M, glm::vec3(black_captured_coordinates[i][j].first, black_captured_coordinates[i][j].second, 0));
					nM = glm::rotate(nM, 90 * PI / 180, glm::vec3(0, 0, 1));
					black_captured[i][j]->draw(nM, tex_light);
				}
				if (white_captured[i][j] == nullptr && black_captured[i][j] == nullptr) {
					return;
				}
			}
		}
	}

	void draw_move(glm::mat4 M, GLuint tex_light, GLuint tex_dark, GLuint tex_cloth) {
		
		if (this->is_moving == false) {
			return;
		}

		if (this->current_move.first == shift) {
			cout << "Drawing shift" << "\n";
			if (this->colors[this->FIGURE_TO_MOVE] == white) {
				draw_shift(M, tex_light, this->FIGURE_TO_MOVE, this->FROM, this->TO, -90, this->DIRECTION, this->PATH_LENGTH, this->A, this->B, this->C);
			}
			else if(this->colors[this->FIGURE_TO_MOVE] == black) {
				draw_shift(M, tex_dark, this->FIGURE_TO_MOVE, this->FROM, this->TO, 90, this->DIRECTION, this->PATH_LENGTH, this->A, this->B, this->C);
			}
		}

		else if (this->current_move.first == castling) {
			cout << "Drawing castling" << "\n";
			if (this->colors[this->FIGURE_TO_MOVE] == white) {
				draw_castling(M, tex_light, -90);
			}
			else if (this->colors[this->FIGURE_TO_MOVE] == black) {
				draw_castling(M, tex_dark, 90);
			}
		}

		else if (this->current_move.first == fall) {
			if (this->colors[this->FIGURE_TO_MOVE] == white) {
				draw_fall(M, tex_light, tex_cloth, -90);
			}
			else if (this->colors[this->FIGURE_TO_MOVE] == black) {
				draw_fall(M, tex_dark, tex_cloth, 90);
			}
		}

		if (this->step == this->accuracy) {
			this->is_moving = false;

			this->FIGURE_TO_MOVE = nullptr;
			this->FIGURE_TO_MOVE_2 = nullptr;
		}

		step += 1;
	}

	void next_move(movetype move) {
		this->is_moving = true;
		this->current_move = move;

		player_moves pl_mv = move.first;

		if (pl_mv == shift || pl_mv == castling) {
			this->FROM = move.second[0];
			this->TO = move.second[1];
			this->FIGURE_TO_MOVE = chessboard[FROM.first][FROM.second];
			this->chessboard[this->FROM.first][this->FROM.second] = nullptr;
			this->DIRECTION = calc_direction(this->FROM, this->TO);
			this->PATH_LENGTH = calc_path_length(this->DIRECTION);

			float x1 = 0;
			float x2 = this->PATH_LENGTH;

			this->A = this->H / ((float)x1 * x2 - (float)pow((x1 + x2), 2) / 4);
			this->B = -this->A * (x1 + x2);
			this->C = this->A * x1 * x2;
		}

		if (pl_mv == castling) {
			this->FROM_2 = move.second[2];
			this->TO_2 = move.second[3];
			this->FIGURE_TO_MOVE_2 = chessboard[FROM_2.first][FROM_2.second];
			this->chessboard[FROM_2.first][FROM_2.second] = nullptr;
			this->DIRECTION_2 = calc_direction(this->FROM_2, this->TO_2);
			this->PATH_LENGTH_2 = calc_path_length(this->DIRECTION_2);
		}

		if (pl_mv == fall) {
			this->FROM = move.second[0];
			this->FIGURE_TO_MOVE = chessboard[FROM.first][FROM.second];
			this->chessboard[this->FROM.first][this->FROM.second] = nullptr;
		}

		this->step = 1;
	}

	bool is_board_static() {
		if (this->is_moving == false) {
			return true;
		}
		else {
			return false;
		}
	}
};


#endif
