#include <string>
#include <iostream>
#include "penguin_game_object.h"
#include <GLFW/glfw3.h>

namespace game {

	/*
		EnemyGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	PenguinGameObject::PenguinGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, float mass, std::string state)
		: GameObject(position, texture, num_elements, collidable, mass, state) {}

	// Update function for moving the player object around
	void PenguinGameObject::Update(double delta_time) {

		// Special player updates go here
		if (state_ == "patrolling") { //Patrolling
			double lastTime = glfwGetTime();
			SetVelocity(glm::vec3(glm::cos(lastTime), glm::sin(lastTime), 0.0f));
		}
		else { //Moving
			//std::cout << "Moving" << std::endl;

		}

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

} // namespace game
