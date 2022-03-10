#include <string>
#include <iostream>
#include "enemy_game_object.h"
#include <GLFW/glfw3.h>

namespace game {

	/*
		PlayerGameObject inherits from GameObject
		It overrides GameObject's update method, so that you can check for input to change the velocity of the player
	*/

	EnemyGameObject::EnemyGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, std::string state)
		: GameObject(position, texture, num_elements, collidable, state) {}

	// Update function for moving the player object around
	void EnemyGameObject::Update(double delta_time) {

		// Special player updates go here
		if (state_ == "patrolling") { //Patrolling
			double lastTime = glfwGetTime();
			SetVelocity(glm::vec3(glm::cos(lastTime), glm::sin(lastTime), 0.0f));
			//std::cout << "Patrolling" << std::endl;
			//std::cout << "enemy x vel: " << velocity_.x << std::endl;
			//position_ += velocity_ * ((float)delta_time);
		}
		else { //Moving
			//std::cout << "Moving" << std::endl;

		}

		// Call the parent's update method to move the object in standard way, if desired
		GameObject::Update(delta_time);
	}

} // namespace game
