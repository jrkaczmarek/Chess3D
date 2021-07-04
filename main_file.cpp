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

// TESTOWY COMMIT 123 LALALAL TRALLALALALLA

#define GLM_FORCE_RADIANS

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "constants.h"
#include "allmodels.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>

using namespace std;

float speed_x = 0;//[radiany/s]
float speed_y = 0;//[radiany/s]

GLuint tex;
GLuint tex1;

class my_model
{
public:
	vector<glm::vec4> verts;
	vector<glm::vec4> norms;
	vector<glm::vec2> texCoords;
	vector<unsigned int> indices;

	my_model(string plik, int nr_mesh)
	{
		//wgranie modelu
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(plik, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
		cout << importer.GetErrorString() << endl;

		aiMesh* mesh = scene->mMeshes[nr_mesh];

		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			aiVector3D vertex = mesh->mVertices[i];
			verts.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1));

			aiVector3D normal = mesh->mNormals[i];
			norms.push_back(glm::vec4(normal.x, normal.y, normal.z, 0));

			aiVector3D texCoord = mesh->mTextureCoords[0][i];
			texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
		}

		for (int i = 0; i < mesh->mNumFaces; i++) {
			aiFace& face = mesh->mFaces[i];
			for (int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		for (int i = 0; i < 19; i++) {
			cout << i << " " << material->GetTextureCount((aiTextureType)i) << endl;
		}

		for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++) {
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE, i, &str);
			cout << str.C_Str() << endl;
		}
	}

	void draw(glm::mat4 M, GLuint &tex)
	{
		glUniformMatrix4fv(spLambertTextured->u("M"), 1, false, glm::value_ptr(M));
		glEnableVertexAttribArray(spLambertTextured->a("vertex"));
		glVertexAttribPointer(spLambertTextured->a("vertex"), 4, GL_FLOAT, false, 0, verts.data());
		glEnableVertexAttribArray(spLambertTextured->a("normal"));
		glVertexAttribPointer(spLambertTextured->a("normal"), 4, GL_FLOAT, false, 0, norms.data());
		glEnableVertexAttribArray(spLambertTextured->a("texCoord"));
		glVertexAttribPointer(spLambertTextured->a("texCoord"), 2, GL_FLOAT, false, 0, texCoords.data());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex);
		glUniform1i(spLambertTextured->u("tex"), 0);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
		glDisableVertexAttribArray(spLambertTextured->a("vertex"));
		glDisableVertexAttribArray(spLambertTextured->a("normal"));
		glDisableVertexAttribArray(spLambertTextured->a("texCoord"));
	}
};

my_model* plansza;
my_model* pionek;

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
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0.152, 0.298, 0.905, 1); //Ustaw kolor czyszczenia bufora kolorów
	glEnable(GL_DEPTH_TEST); //Włącz test głębokości na pikselach
	glfwSetKeyCallback(window, key_callback);

	tex = readTexture("board_DefaultMaterial_BaseColor.png");
	tex1 = readTexture("Backside_pieces_Backside_pieces_BaseColor.png");
	plansza = new my_model("szachy.fbx", 1);
	pionek = new my_model("szachy.fbx", 0);
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    freeShaders();
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

	//Usunięcie tekstury z pamięci karty graficznej – w freeOpenGLProgram
	glDeleteTextures(1, &tex);
}

void drawScene(GLFWwindow* window,float angle_x,float angle_y) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glm::mat4 M = glm::mat4(1.0f);
	M = glm::rotate(M, angle_y, glm::vec3(0.0f, 1.0f, 0.0f));
	M = glm::rotate(M, angle_x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 V = glm::lookAt(glm::vec3(0.0f, -5.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 P = glm::perspective(glm::radians(100.0f), 1.0f, 1.0f, 1000.0f);

	spLambertTextured->use();
	glUniformMatrix4fv(spLambertTextured->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(spLambertTextured->u("V"), 1, false, glm::value_ptr(V));

	plansza->draw(M, tex);
	M = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.3f));
	pionek->draw(M, tex1);

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

	window = glfwCreateWindow(900, 900, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

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

	//Główna pętla
	float angle_x = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	float angle_y = 0; //zadeklaruj zmienną przechowującą aktualny kąt obrotu
	glfwSetTime(0); //Wyzeruj licznik czasu
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
		angle_x += speed_x * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		angle_y += speed_y * glfwGetTime(); //Oblicz kąt o jaki obiekt obrócił się podczas poprzedniej klatki
		glfwSetTime(0); //Wyzeruj licznik czasu
		drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
