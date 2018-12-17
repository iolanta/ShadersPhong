#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glut.h>
#include <SOIL2.h>
#include <cmath>
#include <utility>
#include <iostream>
#include <vector>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "GLShader.h"
#include "GLobject.h"


int w = 0, h = 0;

GLShader * shaderwrap;
GLobject * objectwrap;

std::vector<int> VertShaders;
std::vector<int> FragShaders;
glm::mat4 Matrix_projection;

float rotateX = 0;
float rotateY = 0;
float scaleX = 1;
float scaleY = 1;

int VertShader, FragShader0, FragShader1, FragShader2, FragShader3, FragShader4, FragShader5;
GLuint tex1, tex2;
int mode = 0;


std::vector<std::string> pathsVert = {
"shader_lab12.vert"
};

std::vector<std::string> pathsFrag = {
"shader_lab12.frag",
"shader_lab12_horizontal.frag",
"shader_lab12_vertical.frag",
"shader_lab12_tex.frag",
"shader_lab12_texcolor.frag",
"shader_lab12_twotex.frag",
};



void Init(void)
{
	glClearColor(0, 0, 0, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}


void load_textures() {
	tex1 =  SOIL_load_OGL_texture("cat_diff.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y);
	tex2 = SOIL_load_OGL_texture("tex2.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y);

}

void Reshape(int x, int y)
{
	if (y == 0 || x == 0) return;

	w = x;
	h = y;
	glViewport(0, 0, w, h);

	Matrix_projection = glm::perspective(80.0f, (float)w / h, 0.01f, 200.0f);
	glm::vec3 eye = {1,0,0};
	glm::vec3 center = { 0,0,0 };
	glm::vec3 up = { 0,0,1};

	Matrix_projection  *= glm::lookAt(eye,center,up);

	

}



void Update(void) {
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shaderwrap->ShaderProgram);
	

	// vert shader part, never changes
	glm::mat4 transfor = glm::scale(Matrix_projection, glm::vec3{ scaleX,scaleY,1 });
	transfor = glm::rotate(transfor, rotateX, glm::vec3( 1,0,0 ));
	transfor = glm::rotate(transfor, rotateY, glm::vec3(0, 1, 0));
	shaderwrap->setUniformmat4("transf",false, transfor);	
	

	switch (mode)
	{
	case 0: // fill
		shaderwrap->setUniformfv3("color", glm::vec3{ 1,0,0 });
		break;
	case 1: // hor
	case 2: // vert
		shaderwrap->setUniform1i("width", 30);
		shaderwrap->setUniformfv3("color1", {1,0,0});
		shaderwrap->setUniformfv3("color2", { 0,1,0 });
		break;
	case 5: // 2 tex	
		shaderwrap->setUniform1i("ourTexture2", 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex2);
		shaderwrap->setUniform1f("mix_coef", 0.5);
	case 4: // texcolor		
	case 3: // tex1
		shaderwrap->setUniform1i("ourTexture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1);
		break;

	default:
		break;
	}

	

	objectwrap->drawObject();

	glUseProgram(0);
	


	glFlush();
	glutSwapBuffers();

}

void keyboard(unsigned char key, int x, int y)	
{
	switch (key)
	{
	case 'w':
		rotateX += 0.1;
		break;
	case 's':
		rotateX -= 0.1;
		break;
	case 'a':
		rotateY -= 0.1;
		break;
	case 'd':
		rotateY += 0.1;
		break;
	default:
		break;
	}

	glutPostRedisplay();
}

void next_mode() {
	mode = (mode + 1) % 6;
	switch (mode)
	{
	case 0: // fill color
		shaderwrap->linkProgram(VertShader, FragShader0);
		objectwrap->BindAttributesToShader(*shaderwrap);
		break;
	case 1: // hor line
		shaderwrap->linkProgram(VertShader, FragShader1);
		objectwrap->BindAttributesToShader(*shaderwrap);
		break;
	case 2: // vert line
		shaderwrap->linkProgram(VertShader, FragShader2);
		objectwrap->BindAttributesToShader(*shaderwrap);
		break;
	case 3: // 1tex
		shaderwrap->linkProgram(VertShader, FragShader3);
		objectwrap->BindAttributesToShader(*shaderwrap);
		break;
	case 4: // texcolor
		shaderwrap->linkProgram(VertShader, FragShader4);
		objectwrap->BindAttributesToShader(*shaderwrap);
		break;
	case 5: // 2tex
		shaderwrap->linkProgram(VertShader, FragShader5);
		objectwrap->BindAttributesToShader(*shaderwrap);
		break;

	default:
		break;
	}


}


void specialKeys(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		scaleX += 0.1;
		break;
	case GLUT_KEY_DOWN:
		scaleX -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		scaleY -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		scaleY += 0.1;
		break;
	case GLUT_KEY_F1:
		next_mode();
	default:
		break;
	}
	glutPostRedisplay();
}




void LoadShaders() {



	
		VertShader  = shaderwrap->load_shader(pathsVert[0], GL_VERTEX_SHADER);
		FragShader0 =  shaderwrap->load_shader(pathsFrag[0], GL_FRAGMENT_SHADER);
		FragShader1 = shaderwrap->load_shader(pathsFrag[1], GL_FRAGMENT_SHADER);
		FragShader2 = shaderwrap->load_shader(pathsFrag[2], GL_FRAGMENT_SHADER);
		FragShader3 = shaderwrap->load_shader(pathsFrag[3], GL_FRAGMENT_SHADER);
		FragShader4 = shaderwrap->load_shader(pathsFrag[4], GL_FRAGMENT_SHADER);
		FragShader5 = shaderwrap->load_shader(pathsFrag[5], GL_FRAGMENT_SHADER);
	

}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(800, 600);

	glutCreateWindow("OpenGL");
	glEnable(GL_DEPTH_TEST);

	
	glutDisplayFunc(Update);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << glewGetErrorString(err) << std::endl;

	}
	Init();

	shaderwrap = new GLShader();
	objectwrap = new GLobject("cat.obj");
	load_textures();
	LoadShaders();
	mode = -1;
	next_mode();
	
	shaderwrap->checkOpenGLerror();

	glutMainLoop();


	return 0;         
}