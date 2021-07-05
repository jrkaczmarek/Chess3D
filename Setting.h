#ifndef SETTING_H
#define SETTING_H

#include "Player.h"
#include <utility>
#include <map>

using namespace std;

typedef pair < pair <int, int >, pair <int, int >> movetype;

class Setting {
private:
	my_model* chessboard[8][8];
	pair < float, float > chessboard_coordinates[8][8];
	map <my_model*, string> colors;
	map <my_model*, string> figures;

	my_model* white_captured[8][2];
	my_model* black_captured[8][2];

	pair <float, float> white_captured_coordinates[8][2];
	pair <float, float> black_captured_coordinates[8][2];

	bool is_moving;
	int accuracy;
	int step;
	my_model* figure_to_move;
	pair <int, int> FROM;
	pair <int, int> TO;
	pair <float, float> dir_vec;

	void capture(my_model* figure) {
		if (colors[figure] == "black") {
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 8; i++) {
					if (white_captured[i][j] == nullptr) {
						white_captured[i][j] = figure;
						cout << "Sending " << colors[figure] << " " << figures[figure] << " to " << white_captured_coordinates[i][j].first << " " << white_captured_coordinates[i][j].second << "\n";
						return;
					}
				}
			}
		}
		else if (colors[figure] == "white") {
			for (int j = 0; j < 2; j++) {
				for (int i = 0; i < 8; i++) {
					if (black_captured[i][j] == nullptr) {
						cout << "Sending " << colors[figure] << " " << figures[figure] << " to " << black_captured_coordinates[i][j].first << " " << black_captured_coordinates[i][j].second << "\n";
						black_captured[i][j] = figure;
						return;
					}
				}
			}
		}
		return;
	}

public:
	Setting() {
		this->is_moving = false;
		this->accuracy = 0;
		this->figure_to_move = nullptr;

		pair <float, float> crd = make_pair(-0.21f, -0.21f);
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				chessboard[i][j] = nullptr;
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
		if (P1.get_type() != "white") {
			Player tmp = P1;
			P1 = P2;
			P2 = tmp;
		}

		for (int i = 0; i < 8; i++) {
			this->chessboard[i][1] = P1.pawns[i];
			colors[P1.pawns[i]] = "white";
			figures[P1.pawns[i]] = "pawn";
			this->chessboard[i][6] = P2.pawns[i];
			colors[P2.pawns[i]] = "black";
			figures[P2.pawns[i]] = "pawn";
		}

		this->chessboard[0][0] = P1.rooks[0];
		colors[P1.rooks[0]] = "white";
		figures[P1.rooks[0]] = "rook";
		this->chessboard[7][0] = P1.rooks[1];
		colors[P1.rooks[1]] = "white";
		figures[P1.rooks[1]] = "rook";
		this->chessboard[1][0] = P1.knights[0];
		colors[P1.knights[0]] = "white";
		figures[P1.knights[0]] = "knight";
		this->chessboard[6][0] = P1.knights[1];
		colors[P1.knights[1]] = "white";
		figures[P1.knights[1]] = "knight";
		this->chessboard[2][0] = P1.bishops[0];
		colors[P1.bishops[0]] = "white";
		figures[P1.bishops[0]] = "bishop";
		this->chessboard[5][0] = P1.bishops[1];
		colors[P1.bishops[1]] = "white";
		figures[P1.bishops[1]] = "bishop";
		this->chessboard[4][0] = P1.queen;
		colors[P1.queen] = "white";
		figures[P1.queen] = "queen";
		this->chessboard[3][0] = P1.king;
		colors[P1.king] = "white";
		figures[P1.king] = "king";

		this->chessboard[0][7] = P2.rooks[0];
		colors[P2.rooks[0]] = "black";
		figures[P2.rooks[0]] = "rook";
		this->chessboard[7][7] = P2.rooks[1];
		colors[P2.rooks[1]] = "black";
		figures[P2.rooks[1]] = "rook";
		this->chessboard[1][7] = P2.knights[0];
		colors[P2.knights[0]] = "black";
		figures[P2.knights[0]] = "knight";
		this->chessboard[6][7] = P2.knights[1];
		colors[P2.knights[1]] = "black";
		figures[P2.knights[1]] = "knight";
		this->chessboard[2][7] = P2.bishops[0];
		colors[P2.bishops[0]] = "black";
		figures[P2.bishops[0]] = "bishop";
		this->chessboard[5][7] = P2.bishops[1];
		colors[P2.bishops[1]] = "black";
		figures[P2.bishops[1]] = "bishop";
		this->chessboard[4][7] = P2.queen;
		colors[P2.queen] = "black";
		figures[P2.queen] = "queen";
		this->chessboard[3][7] = P2.king;
		colors[P2.king] = "black";
		figures[P2.king] = "king";
	}

	void draw_board(glm::mat4 M, GLuint tex_light, GLuint tex_dark) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (this->chessboard[i][j] != nullptr) {
					if (this->colors[chessboard[i][j]] == "white") {
						glm::mat4 nM = glm::translate(M, glm::vec3(this->chessboard_coordinates[i][j].first, chessboard_coordinates[i][j].second, 0));
						nM = glm::rotate(nM, -90 * PI / 180, glm::vec3(0, 0, 1));
						chessboard[i][j]->draw(nM, tex_light);
					}
					else if (this->colors[chessboard[i][j]] == "black") {
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

	void draw_move(glm::mat4 M, GLuint tex_light, GLuint tex_dark) {
		if (this->is_moving == false) {
			return;
		}

		float nx = chessboard_coordinates[FROM.first][FROM.second].first + (dir_vec.first * step) / accuracy;
		float ny = chessboard_coordinates[FROM.first][FROM.second].second + (dir_vec.second * step) / accuracy;

		glm::mat4 nM = glm::translate(M, glm::vec3(nx, ny, 0));

		if (colors[figure_to_move] == "white") {
			nM = glm::rotate(nM, -90 * PI / 180, glm::vec3(0, 0, 1));
			figure_to_move->draw(nM, tex_light);
		}
		else if (colors[figure_to_move] == "black") {
			nM = glm::rotate(nM, 90 * PI / 180, glm::vec3(0, 0, 1));
			figure_to_move->draw(nM, tex_dark);
		}

		if (step == accuracy) {
			this->is_moving = false;
			
			if (chessboard[TO.first][TO.second] != nullptr) {
				capture(chessboard[TO.first][TO.second]);
			}

			chessboard[TO.first][TO.second] = figure_to_move;
		}

		step += 1;
	}

	void next_move(movetype move) {
		this->is_moving = true;
		this->FROM = move.first;
		this->TO = move.second;
		this->figure_to_move = chessboard[FROM.first][FROM.second];
		this->chessboard[FROM.first][FROM.second] = nullptr;

		this->dir_vec = make_pair(chessboard_coordinates[TO.first][TO.second].first - chessboard_coordinates[FROM.first][FROM.second].first, chessboard_coordinates[TO.first][TO.second].second - chessboard_coordinates[FROM.first][FROM.second].second);
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



	void next_move2(movetype move) {
		my_model* tmp;
		pair <int, int> from = move.first;
		pair <int, int> to = move.second;

		if (this->chessboard[to.first][to.second] != nullptr) {
			my_model* figure = this->chessboard[to.first][to.second];
			this->capture(figure);
		}

		tmp = this->chessboard[from.first][from.second];
		this->chessboard[from.first][from.second] = nullptr;
		this->chessboard[to.first][to.second] = tmp;
	}
};

#endif