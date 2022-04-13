#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <vector>

#include "shader.h"


namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of objects in the game world
        The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor
            GameObject(const glm::vec3& position, GLuint texture);
            GameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable);
            GameObject(const glm::vec3 &position, GLuint texture, GLint num_elements, bool collidable, float mass);
            GameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, float mass, std::string state);
            // Update the GameObject's state. Can be overriden for children
            virtual void Update(double delta_time);

            // Renders the GameObject using a shader
            void Render(Shader &shader);
            void Render(Shader& shader, glm::mat4 ParentTransformation);

            // Getters
            inline glm::vec3& GetPosition(void) { return position_; }
            inline float GetScale(void) { return scale_; }
            inline float GetMass(void) { return mass_; }
            inline glm::vec3& GetVelocity(void) { return velocity_; }
            inline bool GetCollidable(void) { return collidable_; }
            inline std::string GetState(void) { return state_; }
            inline float GetAngle(void) { return angle_; }
            inline glm::mat4& GetTransformationMatrix(void) { return transformation_matrix_; }
            inline glm::mat4& GetRotationMatrix(void) { return rotation_matrix_; }
            inline glm::mat4& GetTranslationMatrix(void) { return translation_matrix_; }
            inline glm::mat4& GetMovementMatrix(void) { return movement_matrix_; }
            inline std::vector<GameObject*> GetChildren(void) { return children_; }
            inline std::vector<GameObject*> GetBullet(void) { return bullet_; }
            inline std::vector<GameObject*> GetArrow(void) { return arrow_; }
            inline std::vector<GameObject*> GetShields(void) { return shields_; }

            // Setters
            inline void SetPosition(const glm::vec3& position) { position_ = position; }
            inline void SetScale(float scale) { scale_ = scale; }
            inline void SetState(std::string state) { state_ = state; }

            inline void SetVelocity(const glm::vec3& velocity) { 
                if (velocity.x > 2 || velocity.y > 2 || velocity.x < -2 || velocity.y < -2) {
                }
                else {
                    velocity_ = velocity; 
                }
            }

            inline void SetVelocity(const glm::vec3& velocity, bool override) { velocity_ = velocity; }
            inline void SetCollidable(bool collidable) { collidable_ = collidable; }
            inline void SetAngle(float angle) { angle_ = angle; }
            inline void SetMass(float mass) { mass_ = mass; }
            inline void SetTransformationMatrix(const glm::mat4& matrix) { transformation_matrix_ = matrix; }
            inline void SetRotationMatrix(const glm::mat4& matrix) { rotation_matrix_ = matrix; }
            inline void SetMovementMatrix(const glm::mat4& matrix) { movement_matrix_ = matrix; }
            inline void SetChildren(std::vector<GameObject*> children) { children_ = children; }
            inline void AddChild(GameObject* child) { children_.push_back(child); }
            inline void AddBullet(GameObject* bullet) { bullet_.push_back(bullet); }
            inline void AddArrow(GameObject* arrow) { arrow_.push_back(arrow); }
            inline void AddShield(GameObject* shield) { shields_.push_back(shield); }

            inline void DeleteBullet() { bullet_.clear(); }
            inline void DeleteArrow() { arrow_.clear(); }
            inline void RemoveShields() { shields_.clear(); }


        protected:
            // Object's Transform Variables
            // TODO: Add more transformation variables
            glm::vec3 position_;
            float scale_;
            float angle_;
            float mass_;
            glm::vec3 velocity_;
            glm::mat4 transformation_matrix_;
            glm::mat4 rotation_matrix_;
            glm::mat4 translation_matrix_;
            glm::mat4 movement_matrix_;
            std::vector<GameObject*> children_;
            std::vector<GameObject*> bullet_;
            std::vector<GameObject*> arrow_;
            std::vector<GameObject*> shields_;

            // Object's details
            GLint num_elements_;

            // Object's texture reference
            GLuint texture_;

            //Collidable bool
            bool collidable_;

            std::string state_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
