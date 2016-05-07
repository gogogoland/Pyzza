/* shader to render simple particle system points */
#version 410 core

uniform sampler2D	myTextureSampler;

in vec2		texture_coordinates;
in vec4		color;
out vec4	frag_colour;

vec4		default_color = vec4(1.0f, 0.0f, 1.0f, 1.0f);

void main ()
{
//	frag_colour = default_color;
	vec4 texel_e;
	texel_e.rgb = texture(myTextureSampler, texture_coordinates).rgb;

	frag_colour = color;

}
