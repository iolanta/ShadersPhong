#include <GL/glew.h>
#include <GL/wglew.h>
#include <GL/glut.h>
#include <SOIL2.h>
#include <cmath>
#include <utility>
#include <iostream>
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp> 

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

int VertShaderF, FragShaderF, VertShaderG, FragShaderG;
GLuint tex1, tex2;
int mode = 0;
bool use_texture = false;

glm::vec3 eye;
glm::vec4 light_position, light_ambient, light_diffuse, light_specular;
glm::vec3 light_attenuation;


float lightZ = 0;
float lightX = 2;
float lightAngle = 0;

std::vector<std::string> pathsVert = {
"shader_lab14_phong.vert",
"shader_lab14_gouraud.vert"
};

std::vector<std::string> pathsFrag = {
"shader_lab14_phong.frag",
"shader_lab14_gouraud.frag"
};




void LoadShaders() {

	VertShaderF = shaderwrap->load_shader(pathsVert[0], GL_VERTEX_SHADER);
	VertShaderG = shaderwrap->load_shader(pathsVert[1], GL_VERTEX_SHADER);
	FragShaderF = shaderwrap->load_shader(pathsFrag[0], GL_FRAGMENT_SHADER);
	FragShaderG = shaderwrap->load_shader(pathsFrag[1], GL_FRAGMENT_SHADER);

}

void Init(void)
{
	glClearColor(0, 0, 0, 1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


}


void load_textures() {
	tex1 =  SOIL_load_OGL_texture("cat_diff.tga", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_MULTIPLY_ALPHA | SOIL_FLAG_INVERT_Y);
	
}

void Reshape(int x, int y)
{
	if (y == 0 || x == 0) return;

	w = x;
	h = y;
	glViewport(0, 0, w, h);

	Matrix_projection = glm::perspective(80.0f, (float)w / h, 0.01f, 200.0f);
	eye = {1.5,0,0};
	glm::vec3 center = { 0,0,0 };
	glm::vec3 up = { 0,0,-1};

	Matrix_projection  *= glm::lookAt(eye,center,up);

	

}


void set_light() {

	light_position = {0,0,2,1};
	light_ambient = { 1,1,1,1 };
	light_diffuse = { 1,1,1,1 };
	light_specular = { 1,1,1,1 };
	light_attenuation = {0,0,0};
}


void Update(void) {
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(shaderwrap->ShaderProgram);
	

	glm::vec4  lp = { lightX,0,0,1 };
	glm::mat4  m = glm::translate(glm::vec3(0, 0, lightZ));
	m = glm::rotate(m,lightAngle, glm::vec3(0, 0, 1));
	lp = m * lp;

	// vert shader part, never changes
	glm::mat4 transform_model = glm::rotate(rotateY, glm::vec3(0, 0, 1));
	transform_model = glm::rotate(transform_model, rotateX, glm::vec3( 1,0,0 ));
	
	shaderwrap->setUniformmat4("transform_model",false, transform_model);	
	shaderwrap->setUniformmat4("transform_viewProjection", false, Matrix_projection);
	shaderwrap->setUniformfv3("transform_viewPosition", eye);

	glm::mat3 transform_normal = glm::inverseTranspose(glm::mat3(transform_model));
	shaderwrap->setUniformmat3("transform_normal", false, transform_normal);




	shaderwrap->setUniformfv4("light_position", lp);
	shaderwrap->setUniformfv4("light_ambient", light_ambient);
	shaderwrap->setUniformfv4("light_diffuse", light_diffuse);
	shaderwrap->setUniformfv4("light_specular", light_specular);

	shaderwrap->setUniformfv3("light_attenuation", light_attenuation);

	shaderwrap->setUniformfv4("material_ambient", objectwrap->material_ambient);
	shaderwrap->setUniformfv4("material_diffuse", objectwrap->material_diffuse);
	shaderwrap->setUniformfv4("material_specular", objectwrap->material_specular);
	shaderwrap->setUniformfv4("material_emission", objectwrap->material_emission);
	shaderwrap->setUniform1f("material_shininess", objectwrap->material_shininess);

	shaderwrap->setUniform1b("use_texture", use_texture);


	shaderwrap->setUniform1s("material_texture", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);


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
	mode = (mode + 1) % 2;
	switch (mode)
	{
	case 0: // Fong
		shaderwrap->linkProgram(VertShaderF, FragShaderF);
		objectwrap->BindAttributesToShader(*shaderwrap);
		break;
	case 1: // Gourough
		shaderwrap->linkProgram(VertShaderG, FragShaderG);
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
		lightZ += 0.1;
		break;
	case GLUT_KEY_DOWN:
		lightZ -= 0.1;
		break;
	case GLUT_KEY_LEFT:
		lightAngle -= 0.1;
		break;
	case GLUT_KEY_RIGHT:
		lightAngle += 0.1;
		break;
	case GLUT_KEY_F1:
		next_mode();
		break;
	case GLUT_KEY_F2:
		use_texture = !use_texture;
			break;
	default:
		break;
	}
	glutPostRedisplay();
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
	
	load_textures();
	LoadShaders();
	mode = -1;
	objectwrap = new GLobject("cat.obj");
	next_mode();
	shaderwrap->checkOpenGLerror();
	set_light();
	glutMainLoop();


	return 0;         
}