#include <string>
#include <iostream>
#include "buoy_game_object.h"
#include <GLFW/glfw3.h>

namespace game {

	/*
		BuoyGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	BuoyGameObject::BuoyGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, float mass)
		: GameObject(position, texture, num_elements, collidable, mass) {}

	// Update function for moving the player object around
	void BuoyGameObject::Update(double delta_time) {

		// Special player updates go here
		

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

} // namespace game
