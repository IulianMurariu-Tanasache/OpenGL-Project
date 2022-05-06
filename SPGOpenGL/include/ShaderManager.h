#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

//Singleton probabil candva
class ShaderManager {
	private:
		GLuint shader_programme;
		GLuint shader_skybox;
	public:
		ShaderManager();
		~ShaderManager();
		GLuint loadShader(const char* vert, const char* frag);
		void printShaderInfoLog(GLuint obj);
		void printProgramInfoLog(GLuint obj);
		std::string textFileRead(const char* fn);
		GLuint getShaderProgramme();
		GLuint getShaderSkybox();
};