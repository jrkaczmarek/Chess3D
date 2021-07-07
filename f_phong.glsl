#version 330

uniform sampler2D tex;
uniform sampler2D tex_spec;
uniform vec4 lp_1;
uniform vec4 lp_2;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
in vec2 i_tc;

in vec4 l_1;
in vec4 n_1;
in vec4 v_1;

in vec4 l_2;
in vec4 n_2;
in vec4 v_2;

in vec4 frag_position;

vec3 calc_point_light(vec4 l, vec4 n, vec4 v, vec4 lp) {
	vec4 color=texture(tex,i_tc);
	vec4 color_spec=texture(tex_spec,i_tc);

	vec4 ml = normalize(l);
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	vec4 mr = reflect(-ml, mn);
	float nl = clamp(dot(mn, ml), 0, 1); 
    float rv = pow(clamp(dot(mr, mv), 0, 1), 90);

	float dist = length(frag_position - lp);
	float constant = 1.0f;
	float linear = 0.1f;
	float quadratic = 0.028f;
	float attenuation = 1.0 / (constant + linear * dist + quadratic * (dist*dist));

	vec3 amb = 0.06f * color.rgb * attenuation;
	vec3 diff = 0.7f * nl * color.rgb * attenuation;
	vec3 spec = 1.0f * rv * color_spec.rgb * attenuation;

	return amb + diff + spec;
}

void main(void) {
	vec3 light1 = calc_point_light(l_1, n_1, v_1, lp_1);
	vec3 light2 = calc_point_light(l_2, n_2, v_2, lp_2);
	pixelColor= vec4(light1+light2, 1);
}
