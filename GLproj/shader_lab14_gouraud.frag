#version 410

out vec4 fcolor;
uniform bool use_texture;
uniform sampler2D material_texture;

// параметры, полученные из вершинного шейдера
in vec2 vert_texcoord;       
in vec4 vert_color;

void main(){
	if(use_texture)
		fcolor = vert_color * texture(material_texture, vert_texcoord);
	else
		fcolor = vert_color;
	
}