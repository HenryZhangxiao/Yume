#ifndef BACKGROUND_GAME_OBJECT_H
#define BACKGROUND_GAME_OBJECT_H

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class BackgroundGameObject : public GameObject {

    public:
        BackgroundGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable);

        // Update function for moving the player object around
        void Update(double delta_time) override;

    }; // class BACKGROUND_GAME_OBJECT_H

} // namespace game

#endif // BACKGROUND_GAME_OBJECT_H
