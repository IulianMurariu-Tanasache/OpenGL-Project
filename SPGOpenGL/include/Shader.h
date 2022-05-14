#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader
{
private:
    GLuint shader_programme;
    void loadShader(const char* vert, const char* frag);
    void printShaderInfoLog(GLuint obj);
    void printProgramInfoLog(GLuint obj);
    std::string textFileRead(const char* fn);

public:
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();
    void use();
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setMat4(const std::string& name, glm::mat4& mat) const;
    void setVec3(const std::string& name, glm::vec3& vec) const;
};
