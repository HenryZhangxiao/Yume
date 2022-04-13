#include <string>
#include <iostream>
#include "background_game_object.h"
#include <GLFW/glfw3.h>

namespace game {

	/*
		ShieldGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	BackgroundGameObject::BackgroundGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable)
		: GameObject(position, texture, num_elements, collidable) {}

	// Update function for moving the player object around
	void BackgroundGameObject::Update(double delta_time) {
		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);

	}

} // namespace game
