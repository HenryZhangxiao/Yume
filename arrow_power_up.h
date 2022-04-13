#ifndef ARROW_POWER_UP_H
#define ARROW_POWER_UP_H

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class ArrowPowerUp : public GameObject {

    public:
        ArrowPowerUp(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable);

        // Update function for moving the player object around
        void Update(double delta_time) override;

    }; // class ShockPowerUp

} // namespace game

#endif // ARROW_POWER_UP_H
