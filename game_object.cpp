#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"
#include <iostream>

#include "game_object.h"

namespace game {

GameObject::GameObject(const glm::vec3 &position, GLuint texture, GLint num_elements, bool collidable) 
{
    // Initialize all attributes
    position_ = position;
    scale_ = 1.0;
    velocity_ = glm::vec3(0.0f, 0.0f, 0.0f); // Starts out stationary
    num_elements_ = num_elements;
    texture_ = texture;
    collidable_ = collidable;
    angle_ = 0.0f;

}

GameObject::GameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, std::string state)
{
    // Initialize all attributes
    position_ = position;
    scale_ = 1.0;
    velocity_ = glm::vec3(0.0f, 0.0f, 0.0f); // Starts out stationary
    num_elements_ = num_elements;
    texture_ = texture;
    collidable_ = collidable;
    state_ = state;
    angle_ = 0.0f;
}


void GameObject::Update(double delta_time) {

    // Update object position with Euler integration
    position_ += velocity_ * ((float) delta_time);
}


void GameObject::Render(Shader &shader) {

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);

    // TODO: Add other types of transformations
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0), angle_, glm::vec3(0, 0, 1));

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;
    //std::cout << "trans matrix: " << glm::to_string(transformation_matrix) << std::endl;
    transformation_matrix_ = transformation_matrix;
    /*
    for (GameObject* child: children_) {
        child->Render(shader, transformation_matrix);
    }
    */
    // Set the transformation matrix in the shader
    shader.SetUniformMat4("transformation_matrix", transformation_matrix);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, num_elements_, GL_UNSIGNED_INT, 0);
}

void GameObject::Render(Shader& shader, glm::mat4 ParentTransformation){
    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);
    /*
    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
    */
    // TODO: Add other types of transformations
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0), angle_, glm::vec3(0, 0, 1));

    // Setup the transformation matrix for the shader
    glm::mat4 transformation_matrix = ParentTransformation * rotation_matrix;
    //std::cout << "trans matrix: " << glm::to_string(transformation_matrix) << std::endl;
    //transformation_matrix_ = transformation_matrix;

    // Set the transformation matrix in the shader
    shader.SetUniformMat4("transformation_matrix", transformation_matrix);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, num_elements_, GL_UNSIGNED_INT, 0);
}

} // namespace game
