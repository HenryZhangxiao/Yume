#ifndef STAR_POWER_UP_H
#define STAR_POWER_UP_H

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class StarPowerUp : public GameObject {

    public:
        StarPowerUp(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable);

        // Update function for moving the player object around
        void Update(double delta_time) override;

    }; // class ShieldPowerUp

} // namespace game

#endif // STAR_POWER_UP_H
