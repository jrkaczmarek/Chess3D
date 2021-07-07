/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "my_model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>
#include <Parser.h>
#include <Player.h>
#include <Setting.h>

using namespace std;

float speed_x = 0; //[radiany/s]
float speed_y = 0; //[radiany/s]

GLuint tex_light;
GLuint tex_dark;
GLuint tex_spec;
GLuint tex_cloth;

Player white_player = Player(white, 54, 0, 5, 6, 28, 3, 2);
Player black_player = Player(black, 54, 0, 5, 6, 28, 3, 2);
Setting setting = Setting();

my_model* plansza0;
my_model* plansza1;

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mod
) {
	if (action == GLFW_PRESS) {
		if (key == GLFW_KEY_LEFT) {
			speed_y = -PI;
		}
		if (key == GLFW_KEY_RIGHT) {
			speed_y = PI;
		}
		if (key == GLFW_KEY_UP) {
			speed_x = -PI;
		}
		if (key == GLFW_KEY_DOWN) {
			speed_x = PI;
		}
	}
	if (action == GLFW_RELEASE) {
		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT) {
			speed_y = 0;
		}
		if (key == GLFW_KEY_UP || key == GLFW_KEY_DOWN) {
			speed_x = 0;
		}
	}
}


//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
    initShaders();
	
	glClearColor(0, 0., 0., 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, key_callback);

	tex_light = readTexture("textures/Chess_Wood_Base.png");
	tex_dark = readTexture("textures/Chess_Wood_Dark_Base.png");
	tex_spec = readTexture("textures/Chess_Wood_Specular.png");
	tex_cloth = readTexture("textures/Soft_Cloth_Base.png");
	plansza0 = new my_model("models/Wooden_Chess.fbx", 65);
	plansza1 = new my_model("models/Wooden_Chess.fbx", 64);
	
	white_player.initialize("models/Wooden_Chess.fbx");
	black_player.initialize("models/Wooden_Chess.fbx");
	setting.initialize(black_player, white_player, 150);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    

	//Usunięcie tekstury z pamięci karty graficznej – w freeOpenGLProgram
	glDeleteTextures(1, &tex_light);
	glDeleteTextures(1, &tex_dark);
	glDeleteTextures(1, &tex_spec);
	glDeleteTextures(1, &tex_cloth);
}

glm::vec3 calcObserver(float angle_x, float angle_y)
{
	float camera_radius = 0.5f;
	glm::vec4 observer = glm::vec4(0, 0, 0, 1);
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angle_y, glm::vec3(0, 0, 1));
	M = glm::rotate(M, angle_x, glm::vec3(0, 1, 0));
	M = glm::translate(M, glm::vec3(camera_radius, 0.0, 0.3f));
	observer = M * observer;
	return glm::vec3(observer);
}

void drawScene(GLFWwindow* window,float angle_x,float angle_y) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	glm::mat4 V = glm::lookAt(calcObserver(angle_x,angle_y), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 P = glm::perspective(glm::radians(85.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 M = glm::mat4(1.0f);

	// rysowanie modeli
	spPhong->use();
	glUniformMatrix4fv(spPhong->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spPhong->u("V"), 1, false, glm::value_ptr(V));
	glUniform4f(spPhong->u("lp_1"), 0.2, -0.2, 0.2, 1);
	glUniform4f(spPhong->u("lp_2"), 0.2, 0.2, 0.2, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex_spec);
	glUniform1i(spPhong->u("tex_spec"), 1);

	plansza0->draw(glm::rotate(M, 90 * PI / 180, glm::vec3(0, 0, 1)), tex_light);
	plansza1->draw(glm::rotate(M, 90 * PI / 180, glm::vec3(0, 0, 1)), tex_dark);
	setting.draw_board(M, tex_light, tex_dark);
	setting.draw_captured(M, tex_light, tex_dark);
	setting.draw_move(M, tex_light, tex_dark, tex_cloth);

	glfwSwapBuffers(window);
}

int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(900, 900, "Chess3D", NULL, NULL);  //Utwórz okno 900x900 o tytule "Chess3D" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące
	Parser parser("games/Anand vs Carlsen.txt");
	parser.parse_moves();

	//Główna pętla
	double angle_x = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	double angle_y = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		if (setting.is_board_static()) {
			movetype next_move = parser.next_move();
			if (next_move.first != still) {
				setting.next_move(next_move);
			}
		}

		if(angle_x + speed_x * glfwGetTime() >= -0.8 && angle_x + speed_x * glfwGetTime() <= 0.35)
			angle_x += speed_x * glfwGetTime();
		
		angle_y += speed_y * glfwGetTime();

		glfwSetTime(0);
		drawScene(window,angle_x,angle_y);
		glfwPollEvents();
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
