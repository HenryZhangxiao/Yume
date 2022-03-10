#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class EnemyGameObject : public GameObject {

    public:
        EnemyGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, std::string state);

        // Update function for moving the player object around
        void Update(double delta_time) override;

    }; // class EnemyGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_
