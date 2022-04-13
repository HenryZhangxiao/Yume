#include <string>
#include <iostream>
#include "seeker_game_object.h"
#include <GLFW/glfw3.h>

namespace game {

	/*
		SeekerGameObject inherits from GameObject
	*/

	SeekerGameObject::SeekerGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, float mass, std::string state)
		: GameObject(position, texture, num_elements, collidable, mass, state) {}

	// Update function for moving the seeker object around
	void SeekerGameObject::Update(double delta_time) {

		// Special seeker updates go here


		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

} // namespace game
