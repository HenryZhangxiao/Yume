#ifndef ARROW_GAME_OBJECT_H_
#define ARROW_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class ArrowGameObject : public GameObject {

    public:
        ArrowGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable);

        // Update function for moving the player object around
        void Update(double delta_time) override;

    }; // class ArrowGameObject

} // namespace game

#endif // ARROW_GAME_OBJECT_H_
