#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <string>

//Singleton probabil candva
class ShaderManager {
	private:
		GLuint shader_programme;
	public:
		ShaderManager();
		~ShaderManager();
		void printShaderInfoLog(GLuint obj);
		void printProgramInfoLog(GLuint obj);
		std::string textFileRead(char* fn);
		GLuint getShaderProgramme();
};