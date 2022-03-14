#ifndef BUOY__GAME_OBJECT_H_
#define BUOY__GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class BuoyGameObject : public GameObject {

    public:
        BuoyGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, float mass);

        // Update function for moving the player object around
        void Update(double delta_time) override;

    }; // class BuoyGameObject

} // namespace game

#endif // BUOY__GAME_OBJECT_H_
