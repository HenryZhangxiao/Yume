#include <string>
#include <iostream>
#include "shield_game_object.h"
#include <GLFW/glfw3.h>

namespace game {

	/*
		ShieldGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	ShieldGameObject::ShieldGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable)
		: GameObject(position, texture, num_elements, collidable) {}

	// Update function for moving the player object around
	void ShieldGameObject::Update(double delta_time) {

		double lastTime = glfwGetTime();
		SetVelocity(glm::vec3(glm::cos(lastTime), glm::sin(lastTime), 0.0f));

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

} // namespace game
