#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 lp_1;
uniform vec4 lp_2;

//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location=1) in vec4 normal; //wektor normalny w wierzcholku
layout (location=2) in vec2 texCoord; //wspó³rzêdne teksturowania

//Zmienne interpolowane
out vec2 i_tc;

out vec4 l_1;
out vec4 n_1;
out vec4 v_1;

out vec4 l_2;
out vec4 n_2;
out vec4 v_2;

void main(void) {
    l_1 = normalize(V*lp_1 - V*M*vertex);
    n_1 = normalize(V*M*normal);
    v_1 = normalize(vec4(0,0,0,1) - V*M*vertex);

    l_2 = normalize(V*lp_2 - V*M*vertex);
    n_2 = normalize(V*M*normal);
    v_2 = normalize(vec4(0,0,0,1) - V*M*vertex);

    gl_Position=P*V*M*vertex;
    i_tc=texCoord;
}
