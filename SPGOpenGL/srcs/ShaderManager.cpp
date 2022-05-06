#include "ShaderManager.h"
#include <iostream>
#include <fstream>

ShaderManager::ShaderManager()
{
	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
	const GLubyte* version = glGetString(GL_VERSION); // version as a string
	printf("Renderer: %s\n", renderer);
	printf("OpenGL version supported %s\n", version);

	shader_programme = loadShader("shaders/vertex.vert", "shaders/fragment.frag");
	shader_skybox = loadShader("shaders/vertexSkybox.vert", "shaders/fragmentSkybox.frag");
	//glDeleteShader(shader_programme);
}

GLuint ShaderManager::loadShader(const char* vert, const char* frag)
{
	GLuint shader;
	//read shader files
	std::string vstext = textFileRead(vert);
	std::string fstext = textFileRead(frag);
	const char* vertex_shader = vstext.c_str();
	const char* fragment_shader = fstext.c_str();

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);

	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::"<<vert<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	// check for shader compile errors
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fs, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::"<<frag<<"::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	shader = glCreateProgram();

	glAttachShader(shader, fs);
	glAttachShader(shader, vs);
	glLinkProgram(shader);

	printShaderInfoLog(fs);
	printShaderInfoLog(vs);
	printProgramInfoLog(shader);
	return shader;
}

ShaderManager::~ShaderManager()
{
	std::cout << "Delete Shader" << '\n';
	glDeleteShader(shader_programme);
	glDeleteShader(shader_skybox);
}

void ShaderManager::printShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char*)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

void ShaderManager::printProgramInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten = 0;
	char* infoLog;

	glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		infoLog = (char*)malloc(infologLength);
		glGetProgramInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n", infoLog);
		free(infoLog);
	}
}

std::string ShaderManager::textFileRead(const char* fn)
{
	std::ifstream ifile(fn);
	std::string filetext;
	while (ifile.good()) {
		std::string line;
		std::getline(ifile, line);
		filetext.append(line + "\n");
	}
	return filetext;
}

GLuint ShaderManager::getShaderProgramme()
{
	return shader_programme;
}

GLuint ShaderManager::getShaderSkybox()
{
	return shader_skybox;
}

