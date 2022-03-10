#include <iostream>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "file_utils.h"
#include "shader.h"

namespace game {

Shader::Shader(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Shader::Init(const char *vertPath, const char *fragPath)
{
   
    // Load shader program source code
    // Vertex program
    std::string vp = LoadTextFile(vertPath);
    const char *source_vp = vp.c_str();
    // Fragment program
    std::string fp = LoadTextFile(fragPath);
    const char *source_fp = fp.c_str();

    // Create a shader from vertex program source code
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &source_vp, NULL);
    glCompileShader(vs);

    // Check if shader compiled successfully
    GLint status;
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(vs, 512, NULL, buffer);
        throw(std::ios_base::failure(std::string("Error compiling vertex shader: ") + std::string(buffer)));
    }

    // Create a shader from the fragment program source code
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &source_fp, NULL);
    glCompileShader(fs);

    // Check if shader compiled successfully
    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(fs, 512, NULL, buffer);
        throw(std::ios_base::failure(std::string("Error compiling fragment shader: ") + std::string(buffer)));
    }

    // Create a shader program linking both vertex and fragment shaders
    // together
    shader_program_ = glCreateProgram();
    glAttachShader(shader_program_, vs);
    glAttachShader(shader_program_, fs);
    glLinkProgram(shader_program_);

    // Check if shaders were linked successfully
    glGetProgramiv(shader_program_, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader_program_, 512, NULL, buffer);
        throw(std::ios_base::failure(std::string("Error linking shaders: ") + std::string(buffer)));
    }

    // Delete memory used by shaders, since they were already compiled
    // and linked
    glDeleteShader(vs);
    glDeleteShader(fs);


    // Set attributes for shaders
    // Should be consistent with how we created the buffers for the square
    GLint vertex_att = glGetAttribLocation(shader_program_, "vertex");
    glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vertex_att);

    GLint color_att = glGetAttribLocation(shader_program_, "color");
    glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(color_att);

    GLint tex_att = glGetAttribLocation(shader_program_, "uv");
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void *)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(tex_att);
}


void Shader::SetUniform1i(const GLchar *name, int value)
{

    glUniform1i(glGetUniformLocation(shader_program_, name), value);
}


void Shader::SetUniform1f(const GLchar *name, float value)
{

    glUniform1f(glGetUniformLocation(shader_program_, name), value);
}


void Shader::SetUniform2f(const GLchar *name, const glm::vec2 &vector)
{

    glUniform2f(glGetUniformLocation(shader_program_, name), vector.x, vector.y);
}


void Shader::SetUniform3f(const GLchar *name, const glm::vec3 &vector)
{

    glUniform3f(glGetUniformLocation(shader_program_, name), vector.x, vector.y, vector.z);
}


void Shader::SetUniform4f(const GLchar *name, const glm::vec4 &vector)
{

    glUniform4f(glGetUniformLocation(shader_program_, name), vector.x, vector.y, vector.z, vector.w);
}


void Shader::SetUniformMat4(const GLchar *name, const glm::mat4 &matrix)
{

    glUniformMatrix4fv(glGetUniformLocation(shader_program_, name), 1, GL_FALSE, glm::value_ptr(matrix));
}


Shader::~Shader() 
{

    glDeleteProgram(shader_program_);
}


void Shader::Enable() 
{

    glUseProgram(shader_program_);
}


void Shader::Disable()
{

    glUseProgram(0);
}

} // namespace game
