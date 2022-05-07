#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader
{
public:
    // the program ID
    GLuint shader_programme;

    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void loadShader(const char* vert, const char* frag);
    void printShaderInfoLog(GLuint obj);
    void printProgramInfoLog(GLuint obj);
    std::string textFileRead(const char* fn);
    void use();
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, glm::mat4& mat) const;
    void setVec3(const std::string& name, glm::vec3& vec) const;
};
