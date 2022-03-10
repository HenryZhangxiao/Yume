#ifndef SHIELD_GAME_OBJECT_H_
#define SHIELD_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class ShieldGameObject : public GameObject {

    public:
        ShieldGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable);

        // Update function for moving the player object around
        void Update(double delta_time) override;

    }; // class ShieldGameObject

} // namespace game

#endif // SHIELD_GAME_OBJECT_H_
