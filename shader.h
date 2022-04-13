#ifndef SHADER_H_
#define SHADER_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

#define NUM_PARTICLES 4000

namespace game {

    class Shader {

        public:
            Shader(void);
            ~Shader();

            void Init(const char *vertPath, const char *fragPath);

            void Enable();
            void Disable();

            // Create geometry for sprite
            void CreateSprite(void);

            // Create geometry for particles
            void CreateParticles(void);

            // Set shader attributes for sprite
            void SetSpriteAttributes(void);

            // Set shader attributes for particles
            void SetParticleAttributes(void);

            // Sets a uniform integer variable in your shader program to a value
            void SetUniform1i(const GLchar *name, int value);

            // Sets a uniform float variable in your shader program to a value
            void SetUniform1f(const GLchar *name, float value);

            // Sets a uniform vector2 variable in your shader program to a vector
            void SetUniform2f(const GLchar *name, const glm::vec2 &vector);

            // Sets a uniform vector3 variable in your shader program to a vector
            void SetUniform3f(const GLchar *name, const glm::vec3 &vector);

            // Sets a uniform vector4 variable in your shader program to a vector
            void SetUniform4f(const GLchar *name, const glm::vec4 &vector);

            // Sets a uniform matrix4x4 variable in your shader program to a matrix4x4
            void SetUniformMat4(const GLchar *name, const glm::mat4 &matrix);

            // Getters
            inline GLuint GetShaderID() { return shader_program_; }
            inline int GetSpriteSize(void) { return size_sprite_; }
            inline int GetParticleSize(void) { return size_particles_; }

        private:
            GLuint shader_program_;

            // Geometry of sprite
            GLuint vbo_sprite_;
            GLuint ebo_sprite_;
            int size_sprite_;

            // Geometry of particles
            GLuint vbo_particles_;
            GLuint ebo_particles_;
            int size_particles_;

    }; // class Shader

} // namespace game

#endif // SHADER_H_
