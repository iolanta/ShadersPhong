#pragma once
#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glut.h>
#include <SOIL2.h>
#include <string>
#include "GLShader.h"


class GLobject
{
	
	GLushort * indeces;

public:
	GLuint VBO, VAO, IBO;
	GLsizei stride, CoordOffset, NormalOffset, TexCoordOffset, ColorOffset;
	
	int count_vertex;
	int count_indexes;
	GLobject(std::string path);


	bool BindAttributesToShader(GLShader& shaderobject);

	void drawObject();

	~GLobject();
};

