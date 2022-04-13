#ifndef SHIELD_POWER_UP_H
#define SHIELD_POWER_UP_H

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class ShieldPowerUp : public GameObject {

    public:
        ShieldPowerUp(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable);

        // Update function for moving the player object around
        void Update(double delta_time) override;

    }; // class ShieldPowerUp

} // namespace game

#endif // SHIELD_POWER_UP_H
