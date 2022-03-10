#include "player_game_object.h"
#include <iostream>
#include "glm/ext.hpp"

namespace game {

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, GLuint texture, GLint num_elements, bool collidable)
	: GameObject(position, texture, num_elements, collidable) {
	velocity_ = glm::vec3(0.0f, 0.001f, 0.0f);
}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {
	// Special player updates go here

	//std::cout << "position: " << glm::to_string(GetPosition()) << std::endl;
	//float angle = glm::angle(glm::normalize(position_), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
	//std::cout << "angle: " << angle << std::endl;
	//std::cout << "velocity: " << glm::to_string(velocity_) << std::endl;
	//std::cout << "position: " << glm::to_string(position_) << std::endl;
	float length = glm::length(velocity_);

	glm::vec3 temp = glm::vec3 (length * glm::cos(glm::radians(angle_ + 90.0f)), length * glm::sin(glm::radians(angle_ + 90.0f)), 0.0f);
	velocity_ = temp;

	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time);
}

} // namespace game
