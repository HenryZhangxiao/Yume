#include <iostream>
#include <string>
#include <glm/gtc/type_ptr.hpp>

#include "file_utils.h"
#include "shader.h"

namespace game {

Shader::Shader(void)
{
    // Don't do work in the constructor, leave it for the Init() function
    shader_program_ = 0;
    vbo_sprite_ = 0;
    ebo_sprite_ = 0;
    size_sprite_ = 0;
    vbo_particles_ = 0;
    ebo_particles_ = 0;
    size_particles_ = 0;
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

void Shader::CreateSprite(void) {

    // The face of the square is defined by four vertices and two triangles

    // Number of attributes for vertices and faces
    // const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
    // const int face_att = 3; // Vertex indices (3)

    GLfloat vertex[] = {
        // Four vertices of a square
        // Position      Color                Texture coordinates
        -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Top-left
         0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Top-right
         0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f  // Bottom-left
    };

    // Two triangles referencing the vertices
    GLuint face[] = {
        0, 1, 2, // t1
        2, 3, 0  // t2
    };

    // Create buffer for vertices
    glGenBuffers(1, &vbo_sprite_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sprite_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // Create buffer for faces (index buffer)
    glGenBuffers(1, &ebo_sprite_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_sprite_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

    // Set number of elements in array buffer (6 in this case)
    size_sprite_ = sizeof(face) / sizeof(GLuint);
}

void Shader::CreateParticles(void)
{

    // Each particle is a square with four vertices and two triangles

    // Number of attributes for vertices and faces
    const int vertex_attr = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), direction (2), 2D texture coordinates (2), time (1)
                                //    const int face_att = 3; // Vertex indices (3)

    GLfloat vertex[] = {
        // Four vertices of a square
        // Position      Color                Texture coordinates
        -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Top-left
         0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Top-right
         0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f  // Bottom-left
    };

    // Two triangles referencing the vertices
    GLuint face[] = {
        0, 1, 2, // t1
        2, 3, 0  // t2
    };

    // Initialize all the particle vertices
    GLfloat particles[NUM_PARTICLES * vertex_attr];
    float theta, r, tmod;
    float pi = glm::pi<float>();
    float two_pi = 2.0f * pi;

    for (int i = 0; i < NUM_PARTICLES; i++) {
        if (i % 4 == 0) {
            //theta = (two_pi*(rand() % 1000) / 1000.0f);
            theta = (2.0 * (rand() % 10000) / 10000.0f - 1.0f) * 0.13f + pi;
            r = 0.0f + 0.8 * (rand() % 10000) / 10000.0f;
            tmod = (rand() % 10000) / 10000.0f;
        }

        // Position    
        particles[i * vertex_attr + 0] = vertex[(i % 4) * 7 + 0];
        particles[i * vertex_attr + 1] = vertex[(i % 4) * 7 + 1];

        // Velocity
        particles[i * vertex_attr + 2] = sin(theta) * r;
        particles[i * vertex_attr + 3] = cos(theta) * r;

        // Phase
        particles[i * vertex_attr + 4] = tmod;

        // Texture coordinate
        particles[i * vertex_attr + 5] = vertex[(i % 4) * 7 + 5];
        particles[i * vertex_attr + 6] = vertex[(i % 4) * 7 + 6];
    }

    // Initialize all the particle faces
    GLuint manyfaces[NUM_PARTICLES * 6];

    for (int i = 0; i < NUM_PARTICLES; i++) {
        for (int j = 0; j < 6; j++) {
            manyfaces[i * 6 + j] = face[j] + i * 4;
        }
    }

    // Create buffer for vertices
    glGenBuffers(1, &vbo_particles_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_particles_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particles), particles, GL_STATIC_DRAW);

    // Create buffer for faces (index buffer)
    glGenBuffers(1, &ebo_particles_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_particles_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(manyfaces), manyfaces, GL_STATIC_DRAW);

    // Set number of elements in array buffer
    size_particles_ = sizeof(manyfaces) / sizeof(GLuint);
}

void Shader::SetSpriteAttributes(void)
{

    // No blending
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);

    // Bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo_sprite_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_sprite_);

    // Set attributes for shaders
    // Should be consistent with how we created the buffers for the square
    GLint vertex_att = glGetAttribLocation(shader_program_, "vertex");
    glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vertex_att);

    GLint color_att = glGetAttribLocation(shader_program_, "color");
    glVertexAttribPointer(color_att, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(color_att);

    GLint tex_att = glGetAttribLocation(shader_program_, "uv");
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(tex_att);
}

void Shader::SetParticleAttributes(void) {

    // Set blending
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);

    // Bind buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo_particles_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_particles_);

    // Set attributes for shaders
    // Should be consistent with how we created the buffers for the particle elements
    GLint vertex_att = glGetAttribLocation(shader_program_, "vertex");
    glVertexAttribPointer(vertex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(vertex_att);

    GLint dir_att = glGetAttribLocation(shader_program_, "dir");
    glVertexAttribPointer(dir_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(dir_att);

    GLint time_att = glGetAttribLocation(shader_program_, "t");
    glVertexAttribPointer(time_att, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(4 * sizeof(GLfloat)));
    glEnableVertexAttribArray(time_att);

    GLint tex_att = glGetAttribLocation(shader_program_, "uv");
    glVertexAttribPointer(tex_att, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
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
