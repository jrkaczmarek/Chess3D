#version 330

uniform sampler2D tex;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in vec2 i_tc;

in vec4 l_1;
in vec4 n_1;
in vec4 v_1;

in vec4 l_2;
in vec4 n_2;
in vec4 v_2;

vec3 calc_point_light(vec4 l, vec4 n, vec4 v) {
	vec4 color=texture(tex,i_tc);

	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	vec4 mr = reflect(-ml, mn);
	float nl = clamp(dot(mn, ml), 0, 1); 
    float rv = pow(clamp(dot(mr, mv), 0, 1), 90);

	return color.rgb*nl + vec3(rv,rv,rv);
}

void main(void) {
	vec3 light1 = calc_point_light(l_1, n_1, v_1);
	vec3 light2 = calc_point_light(l_2, n_2, v_2);
	pixelColor= vec4(light1+light2, 1);
}
