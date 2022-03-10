#ifndef SHADER_H_
#define SHADER_H_

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

namespace game {

    class Shader {

        public:
            Shader(void);
            ~Shader();

            void Init(const char *vertPath, const char *fragPath);

            void Enable();
            void Disable();

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

        private:
            GLuint shader_program_;

    }; // class Shader

} // namespace game

#endif // SHADER_H_
