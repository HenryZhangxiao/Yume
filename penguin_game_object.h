#ifndef PENGUIN_GAME_OBJECT_H_
#define PENGUIN_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class PenguinGameObject : public GameObject {

    public:
        PenguinGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, float mass, std::string state);

        // Update function for moving the player object around
        void Update(double delta_time) override;

    }; // class PenguinGameObject

} // namespace game

#endif // PENGUIN_GAME_OBJECT_H_
