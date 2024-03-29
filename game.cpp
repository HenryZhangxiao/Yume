#include <stdexcept>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <SOIL/SOIL.h>
#include <iostream>
#include <cstdlib>
#include <math.h>
#include <utility>
#include <stack>
#include <typeinfo>

#include <path_config.h>

#include "shader.h"
#include "player_game_object.h"
#include "game.h"
#include "audio_manager.h"
#include "enemy_game_object.h"
#include "shield_game_object.h"
#include "game_object.h"
#include "buoy_game_object.h"
#include "shield_power_up.h"
#include "background_game_object.h"
#include "seeker_game_object.h"
#include "star_power_up.h"
#include "penguin_game_object.h"
#include "arrow_power_up.h"
#include "arrow_game_object.h"

#include "bin/path_config.h"
#include "glm/ext.hpp"

#define PrintException(exception_object)\
    std::cerr << exception_object.what() << std::endl

namespace game {

// Some configuration constants
// They are written here as global variables, but ideally they should be loaded from a configuration file

// Globals that define the OpenGL window and viewport
const char *window_title_g = "Yume";
const unsigned int window_width_g = 800;
const unsigned int window_height_g = 600;
const glm::vec3 viewport_background_color_g(0.0, 0.0, 1.0);


bool game_over = false;
bool bulletExists = false;
bool shielded = false;
bool invincible = false;
bool frozen = false;
bool arrowPowerUp = false;
bool arrowExists = false;
unsigned int audio_loops = 0;
double lastBulletFired = -1.0;
float timeUntilBulletHitsEnemy = 1.0f;
unsigned int numNonCollidableObjects = 13;
unsigned int numPowerUps = 2;
unsigned int numEnemies = 5;
unsigned int numBuoys = 2;
double lastInvincible = 0.0;
double lastFrozen = 0.0;
double lastArrow = 0.0;

//Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Directory with game resources such as textures
const std::string resources_directory_g = RESOURCES_DIRECTORY;


Game::Game(void)
{
    // Don't do work in the constructor, leave it for the Init() function
}


void Game::Init(void)
{

    // Initialize the window management library (GLFW)
    if (!glfwInit()) {
        throw(std::runtime_error(std::string("Could not initialize the GLFW library")));
    }

    // Set window to not resizable
    // Required or else the calculation to get cursor pos to screenspace will be incorrect
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

    // Create a window and its OpenGL context
    window_ = glfwCreateWindow(window_width_g, window_height_g, window_title_g, NULL, NULL);
    if (!window_) {
        glfwTerminate();
        throw(std::runtime_error(std::string("Could not create window")));
    }

    // Make the window's OpenGL context the current one
    glfwMakeContextCurrent(window_);

    // Initialize the GLEW library to access OpenGL extensions
    // Need to do it after initializing an OpenGL context
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        throw(std::runtime_error(std::string("Could not initialize the GLEW library: ") + std::string((const char *)glewGetErrorString(err))));
    }

    // Set event callbacks
    glfwSetFramebufferSizeCallback(window_, ResizeCallback);

    // Set up square geometry
    size_ = CreateSprite();

    // Initialize shader
    shader_.Init((resources_directory_g+std::string("/vertex_shader.glsl")).c_str(), (resources_directory_g+std::string("/fragment_shader.glsl")).c_str());
    shader_.CreateSprite();
    shader_.Enable();
    shader_.SetSpriteAttributes();

    // Set up z-buffer for rendering
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Enable Alpha blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}


Game::~Game()
{

    glfwDestroyWindow(window_);
    glfwTerminate();
}


void Game::Setup(void)
{

    // Load textures
    SetAllTextures();

    // Setup the player object (position, texture, vertex count)
    // Note that, in this specific implementation, the player object should always be the first object in the game object vector 
    game_objects_.push_back(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex_[0], size_, true));
    game_objects_[0]->SetMass(10.0f);

    // Blades as children of PlayerGameObject
    game_objects_[0]->AddChild(new PlayerGameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex_[4], size_, false));

    // Enemies
    game_objects_.push_back(new EnemyGameObject(glm::vec3(-3.0f, 4.0f, 0.0f), tex_[2], size_, true, 10.0f, "patrolling"));
    game_objects_.push_back(new EnemyGameObject(glm::vec3(3.0f, -2.0f, 0.0f), tex_[2], size_, true, 10.0f, "patrolling"));
    game_objects_.push_back(new EnemyGameObject(glm::vec3(0.8f, 1.5f, 0.0f), tex_[2], size_, true, 10.0f, "patrolling"));

    // Shield power ups
    game_objects_.push_back(new ShieldPowerUp(glm::vec3(3.0f, 1.0f, 0.0f), tex_[7], size_, false));
    game_objects_.push_back(new ShieldPowerUp(glm::vec3(-2.0f, -1.0f, 0.0f), tex_[7], size_, false));

    // Star power ups
    game_objects_.push_back(new StarPowerUp(glm::vec3(3.0f, 3.0f, 0.0f), tex_[10], size_, false));
    game_objects_.push_back(new StarPowerUp(glm::vec3(-2.0f, -3.0f, 0.0f), tex_[10], size_, false));

    // Buoys
    //game_objects_.push_back(new BuoyGameObject(glm::vec3(2.0f, 0.0f, 0.0f), tex_[8], size_, true, 10.0f));
    //game_objects_.push_back(new BuoyGameObject(glm::vec3(-2.0f, 1.0f, 0.0f), tex_[8], size_, true, 20.0f));

    // Arrow power up
    game_objects_.push_back(new ArrowPowerUp(glm::vec3(-4.0f, -3.0f, 0.0f), tex_[12], size_, false));

    // Seekers
    game_objects_.push_back(new SeekerGameObject(glm::vec3(3.0f, -2.0f, 0.0f), tex_[9], size_, true, 5.0f, "moving"));
    game_objects_.push_back(new SeekerGameObject(glm::vec3(-4.0f, 2.0f, 0.0f), tex_[9], size_, true, 5.0f, "moving"));

    // Penguins
    game_objects_.push_back(new PenguinGameObject(glm::vec3(0.0f, 5.0f, 0.0f), tex_[11], size_, false, 5.0f, "patrolling"));
    game_objects_.push_back(new PenguinGameObject(glm::vec3(0.0f, -5.0f, 0.0f), tex_[11], size_, false, 5.0f, "patrolling"));

    // Setup background
    // Origin
    GameObject *background = new BackgroundGameObject(glm::vec3(0.0f, 0.0f, 0.0f), tex_[3], size_, false);
    background->SetScale(10.0);
    game_objects_.push_back(background);

    //Top Left
    GameObject* background2 = new BackgroundGameObject(glm::vec3(-10.0f, 10.0f, 0.0f), tex_[3], size_, false);
    background2->SetScale(10.0);
    game_objects_.push_back(background2);

    //Top Middle
    GameObject* background3 = new BackgroundGameObject(glm::vec3(0.0f, 10.0f, 0.0f), tex_[3], size_, false);
    background3->SetScale(10.0);
    game_objects_.push_back(background3);

    //Top Right
    GameObject* background4 = new BackgroundGameObject(glm::vec3(10.0f, 10.0f, 0.0f), tex_[3], size_, false);
    background4->SetScale(10.0);
    game_objects_.push_back(background4);

    //Middle Left
    GameObject* background5 = new BackgroundGameObject(glm::vec3(-10.0f, 0.0f, 0.0f), tex_[3], size_, false);
    background5->SetScale(10.0);
    game_objects_.push_back(background5);

    //Middle Right
    GameObject* background6 = new BackgroundGameObject(glm::vec3(10.0f, 0.0f, 0.0f), tex_[3], size_, false);
    background6->SetScale(10.0);
    game_objects_.push_back(background6);

    //Bottom Left
    GameObject* background7 = new BackgroundGameObject(glm::vec3(-10.0f, -10.0f, 0.0f), tex_[3], size_, false);
    background7->SetScale(10.0);
    game_objects_.push_back(background7);

    //Bottom Middle
    GameObject* background8 = new BackgroundGameObject(glm::vec3(0.0f, -10.0f, 0.0f), tex_[3], size_, false);
    background8->SetScale(10.0);
    game_objects_.push_back(background8);

    //Bottom Right
    GameObject* background9 = new BackgroundGameObject(glm::vec3(10.0f, -10.0f, 0.0f), tex_[3], size_, false);
    background9->SetScale(10.0);
    game_objects_.push_back(background9);

}


void Game::MainLoop(void) {

    // Loop while the user did not close the window
    double lastTime = glfwGetTime();
    
    while (!glfwWindowShouldClose(window_)){
        // Clear background
        glClearColor(viewport_background_color_g.r,
                     viewport_background_color_g.g,
                     viewport_background_color_g.b, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set view to zoom out, centered by default at 0,0
        float cameraZoom = 0.25f;

        cameraPos = game_objects_[0]->GetPosition();

        glm::mat4 view_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(cameraZoom, cameraZoom, cameraZoom)) * glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        shader_.SetUniformMat4("view_matrix", view_matrix);

        // Calculate delta time
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Update the game
        Update(deltaTime);

        // Push buffer drawn in the background onto the display
        glfwSwapBuffers(window_);

        // Update other events like input handling
        glfwPollEvents();
    }
}


void Game::ResizeCallback(GLFWwindow* window, int width, int height)
{

    // Set OpenGL viewport based on framebuffer width and height
    glViewport(0, 0, width, height);
}


// Create the geometry for a sprite (a squared composed of two triangles)
// Return the number of array elements that form the square
int Game::CreateSprite(void)
{
    // The face of the square is defined by four vertices and two triangles

    // Number of attributes for vertices and faces
    // const int vertex_att = 7;  // 7 attributes per vertex: 2D (or 3D) position (2), RGB color (3), 2D texture coordinates (2)
    // const int face_att = 3; // Vertex indices (3)

    GLfloat vertex[]  = {
        // Four vertices of a square
        // Position      Color                Texture coordinates
        -0.5f,  0.5f,    1.0f, 0.0f, 0.0f,    0.0f, 0.0f, // Top-left
         0.5f,  0.5f,    0.0f, 1.0f, 0.0f,    1.0f, 0.0f, // Top-right
         0.5f, -0.5f,    0.0f, 0.0f, 1.0f,    1.0f, 1.0f, // Bottom-right
        -0.5f, -0.5f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f  // Bottom-left
    };

    // Two triangles referencing the vertices
    GLuint face[] = {
        0, 1, 2, // t1
        2, 3, 0  //t2
    };

    // OpenGL buffers
    GLuint vbo, ebo;

    // Create buffer for vertices
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

    // Create buffer for faces (index buffer)
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(face), face, GL_STATIC_DRAW);

    // Return number of elements in array buffer (6 in this case)
    return sizeof(face) / sizeof(GLuint);
}


void Game::SetTexture(GLuint w, const char *fname)
{
    // Bind texture buffer
    glBindTexture(GL_TEXTURE_2D, w);

    // Load texture from a file to the buffer
    int width, height;
    unsigned char* image = SOIL_load_image(fname, &width, &height, 0, SOIL_LOAD_RGBA);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    // Texture Wrapping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Texture Filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void Game::SetAllTextures(void)
{
    // Load all textures that we will need
    glGenTextures(NUM_TEXTURES, tex_);
    SetTexture(tex_[0], (resources_directory_g + std::string("/textures/chopper.png")).c_str());
    SetTexture(tex_[1], (resources_directory_g + std::string("/textures/alien.png")).c_str());
    SetTexture(tex_[2], (resources_directory_g + std::string("/textures/alien.png")).c_str());
    SetTexture(tex_[3], (resources_directory_g + std::string("/textures/space.png")).c_str());
    SetTexture(tex_[4], (resources_directory_g + std::string("/textures/blade.png")).c_str());
    SetTexture(tex_[5], (resources_directory_g + std::string("/textures/bullet.png")).c_str());
    SetTexture(tex_[6], (resources_directory_g + std::string("/textures/orb.png")).c_str());
    SetTexture(tex_[7], (resources_directory_g + std::string("/textures/shield.png")).c_str());
    SetTexture(tex_[8], (resources_directory_g + std::string("/textures/donut.png")).c_str());
    SetTexture(tex_[9], (resources_directory_g + std::string("/textures/clown.png")).c_str());
    SetTexture(tex_[10], (resources_directory_g + std::string("/textures/star.png")).c_str());
    SetTexture(tex_[11], (resources_directory_g + std::string("/textures/penguin.png")).c_str());
    SetTexture(tex_[12], (resources_directory_g + std::string("/textures/bow.png")).c_str());
    SetTexture(tex_[13], (resources_directory_g + std::string("/textures/arrow.png")).c_str());
    glBindTexture(GL_TEXTURE_2D, tex_[0]);
}


void Game::Controls(void) {

    // Get player game object
    GameObject *player = game_objects_[0];
    GameObject *blades = player->GetChildren()[0];
    glm::vec3 curpos = player->GetPosition();
    glm::vec3 curvel = player->GetVelocity();

    //float deg_angle = glm::angle(glm::normalize(curpos), glm::vec3(1.0f, 0.0f, 0.0f));
    float deg_angle = player->GetAngle() + 90;
    float angle = glm::radians(deg_angle);

    // Check for player input and make changes accordingly
    if (glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS) {
        player->SetVelocity(curvel + glm::vec3(0.05*glm::cos(angle), 0.05*glm::sin(angle), 0.0f));
    }
    if (glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS) {
        player->SetVelocity(curvel - glm::vec3(0.05*glm::cos(angle), 0.05*glm::sin(angle), 0.0f));
    }
    if (glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS) {
        player->SetAngle(player->GetAngle() - 0.02f);
        blades->SetAngle(blades->GetAngle() - 0.02f);
    }
    if (glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS) {
        player->SetAngle(player->GetAngle() + 0.02f);
        blades->SetAngle(blades->GetAngle() + 0.02f);
    }
    if (glfwGetKey(window_, GLFW_KEY_Q) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window_, true);
    }
    if (glfwGetKey(window_, GLFW_KEY_V) == GLFW_PRESS) {
        if (arrowPowerUp) {
            std::cout << "Should be making an arrow now" << std::endl;
            /*GameObject* arrow = new ArrowGameObject(glm::vec3(player->GetPosition()), tex_[13], size_, false);
            float angle = player->GetAngle() + 90.0;
            //std::cout << "supposed velocity: " << glm::to_string(2.5f * player->GetVelocity()) << std::endl;
            glm::vec3 arrowVelocity = glm::vec3(8 * glm::cos(glm::radians(angle)), 8 * glm::sin(glm::radians(angle)), 0.0);
            arrow->SetVelocity(arrowVelocity, true);
            arrow->SetAngle(player->GetAngle());
            arrowPowerUp = false;
            arrowExists = true;
            game_objects_.push_back(arrow);*/
            GameObject* arrow = new GameObject(glm::vec3(player->GetPosition()), tex_[13], size_, false);
            arrow->SetPosition(player->GetPosition());
            float angle = player->GetAngle() + 90.0;
            //std::cout << "supposed velocity: " << glm::to_string(2.5f * player->GetVelocity()) << std::endl;
            glm::vec3 arrowVelocity = glm::vec3(8 * glm::cos(glm::radians(angle)), 8 * glm::sin(glm::radians(angle)), 0.0);
            arrow->SetVelocity(arrowVelocity, true);
            arrow->SetAngle(player->GetAngle());
            player->AddArrow(arrow);
            arrowPowerUp = false;
            arrowExists = true;
            lastArrow = glfwGetTime();
            std::cout << "successfully made arrow" << std::endl;
        }
    }
    
    if (glfwGetKey(window_, GLFW_KEY_SPACE) == GLFW_PRESS) {
        double currentTime = glfwGetTime();
        double bulletDifference = currentTime - lastBulletFired;

        if (bulletDifference >= 1.0 && !bulletExists) {
            // Bullet
            std::cout << "Should be making a bullet now" << std::endl;
            GameObject* bullet = new GameObject(glm::vec3(player->GetPosition()), tex_[5], size_, false);
            bullet->SetPosition(player->GetPosition());
            float angle = player->GetAngle() + 90.0;
            //std::cout << "supposed velocity: " << glm::to_string(2.5f * player->GetVelocity()) << std::endl;
            glm::vec3 bulletVelocity = glm::vec3(8 * glm::cos(glm::radians(angle)), 8 * glm::sin(glm::radians(angle)), 0.0);
            bullet->SetVelocity(bulletVelocity, true);
            bullet->SetAngle(player->GetAngle());
            player->AddBullet(bullet);
            lastBulletFired = currentTime;
            bulletExists = true;
        }
    }
}


void Game::PlayExplosionAudio(void) {
    audio_manager::AudioManager am;
   
    // Reference index of each file to be played
    int explosion_index;

    try {
        // Initialize audio manager
        am.Init(NULL);

        // Set position of listener
        am.SetListenerPosition(0.0, 0.0, 0.0);

        // Load first sound to be played
        std::string filename = std::string(resources_directory_g).append(std::string("/").append(std::string("explosion.wav")));
        explosion_index = am.AddSound(filename.c_str());
        // Set sound properties
        am.SetSoundPosition(explosion_index, -10.0, 0.0, 0.0);
        am.SetLoop(explosion_index, false);
        // Play the sound
        am.PlaySound(explosion_index);

        // Keep program running while something is being played
        while (am.AnySoundIsPlaying()) {
            ;
        }
        //audio_loops++;
        // Shut down the audio manager
        am.ShutDown();
        //if (audio_loops >= 2) {
        //    exit(0);
        //}
        //exit(0);
    }
    catch (std::exception& e) {
        PrintException(e);
    }
}

glm::vec3 Game::GetVectorBetweenTwoPoints(glm::vec3 start, glm::vec3 destination) {

    return(glm::normalize(glm::vec3(destination.x - start.x, destination.y - start.y, 0)));
}

std::pair<float, float> Game::RayCircleCollisionMath(glm::vec3 bulletPosition, glm::vec3 bulletVelocity, glm::vec3 circlePosition) {
    float w = bulletPosition.x; //P.x
    float x = bulletVelocity.x; //d.x
    float y = bulletPosition.y; //P.y
    float z = bulletVelocity.y; //d.y
    float r = 0.5f;
    float a = circlePosition.x;
    float b = circlePosition.y;

    float leftSide = -x*w+a*x-y*z+b*z;
    float discriminant = sqrt(x*x*r*r+2*b*x*x*y-b*b*x*x-x*x*y*y+2*a*b*x*z+2*x*y*z*w-2*a*x*y*z-2*b*x*z*w+2*a*z*z*w+z*z*r*r-a*a*z*z-z*z*w*w);
    float divisor = x * x + z * z;

    float t1 = (leftSide + discriminant) / divisor;
    float t2 = (leftSide - discriminant) / divisor;

    std::pair<float, float> pair;
    pair.first = t1;
    pair.second = t2;
    return pair;
}

void Game::createShields(glm::vec3 curpos) {
    GameObject* player = game_objects_[0];
    player->AddShield(new ShieldGameObject(glm::vec3(curpos.x, curpos.y + 1.0f, 0.0f), tex_[6], size_, false));
    player->AddShield(new ShieldGameObject(glm::vec3(curpos.x + 1.0f, curpos.y - 0.5f, 0.0f), tex_[6], size_, false));
    player->AddShield(new ShieldGameObject(glm::vec3(curpos.x - 1.0f, curpos.y - 0.5f, 0.0f), tex_[6], size_, false));
    player->AddShield(new ShieldGameObject(glm::vec3(curpos.x - 1.0f, curpos.y - 0.5f, 0.0f), tex_[6], size_, false));
    player->AddShield(new ShieldGameObject(glm::vec3(curpos.x - 1.0f, curpos.y - 0.5f, 0.0f), tex_[6], size_, false));
    player->AddShield(new ShieldGameObject(glm::vec3(curpos.x - 1.0f, curpos.y - 0.5f, 0.0f), tex_[6], size_, false));
}

void Game::renderBlades(void) {
    GameObject* player = game_objects_[0];
    GameObject* blades = player->GetChildren()[0];
    blades->SetAngle(blades->GetAngle() + 0.3f);
    blades->Render(shader_, player->GetTransformationMatrix());
}

void Game::bulletUpdate(void) {
    GameObject* bullet = game_objects_[0]->GetBullet()[0];
    double currentTime = glfwGetTime();
    double bulletDifference = currentTime - lastBulletFired;
    int enemyToDelete = 0;
    float timeUntilBulletHitsEnemy = 1.0f;
    // These bounds are for checking the bullet


    // Perform Ray-Circle Collision detection
    // Loop through each enemy to check for collision
    for (int i = 0; i < game_objects_.size(); i++) {
        GameObject* circle = game_objects_[i];
        if (typeid(*circle) == typeid(SeekerGameObject) || typeid(*circle) == typeid(EnemyGameObject)) {
            std::pair<float, float> pair = RayCircleCollisionMath(bullet->GetPosition(), bullet->GetVelocity(), circle->GetPosition());
            if (pair.first >= 0 || pair.second >= 0) {
                if (std::min(pair.first, pair.second) < timeUntilBulletHitsEnemy) {
                    timeUntilBulletHitsEnemy = std::min(pair.first, pair.second);
                    enemyToDelete = i;
                }
            }
        }
        
    }

    if (currentTime >= lastBulletFired + timeUntilBulletHitsEnemy && enemyToDelete != 0) { // If enough time has passed (enemy hit is assumed)
        game_objects_.erase(game_objects_.begin() + enemyToDelete);
        game_objects_[0]->DeleteBullet();
        lastBulletFired = -1.5;
        bulletExists = false;
        numEnemies--;
    }

    // Check if it's been 1.5 seconds since last bullet was fired
    // If so, then we can delete the bullet from the child vector
    if (bulletDifference >= 1.0) {
        game_objects_[0]->DeleteBullet();
        lastBulletFired = -1.5;
        bulletExists = false;
    }
}

void Game::arrowUpdate(void) {
    std::cout << "updating arrow" << std::endl;
    GameObject* arrow = game_objects_[0]->GetArrow()[0];
    int enemyToDelete = 0;

    if(glfwGetTime() - lastArrow >= 3.0){
        game_objects_[0]->DeleteArrow();
        arrowExists = false;
        return;
    }

    // Loop through each enemy to check for collision
    for (int i = 0; i < game_objects_.size(); i++) {
        GameObject* obj = game_objects_[i];
        float distance = glm::length(arrow->GetPosition() - obj->GetPosition());
        if (distance <= 1.0) {
            if (typeid(*obj) == typeid(SeekerGameObject) || typeid(*obj) == typeid(EnemyGameObject)) {
                enemyToDelete = i;
                game_objects_.erase(game_objects_.begin() + enemyToDelete);
                break;
            }
        }   
    }
    
    std::cout << "done updating arrow" << std::endl;
}

void Game::renderBullet(double delta_time) {
    GameObject* bullet = game_objects_[0]->GetBullet()[0];
    bullet->Update(delta_time);
    bullet->Render(shader_);
}

void Game::renderArrow(double delta_time) {
    std::cout << "rendering arrow" << std::endl;
    GameObject* arrow = game_objects_[0]->GetArrow()[0];
    arrow->Update(delta_time);
    arrow->Render(shader_);
    std::cout << "done rendering arrow" << std::endl;
}

void Game::renderShields(double delta_time) {
    GameObject* player = game_objects_[0];
    for (int k = 0; k < player->GetShields().size(); k++) {
        GameObject* shield = player->GetShields()[k];
        float lastTime = glfwGetTime();
        shield->SetVelocity(glm::vec3(glm::cos(lastTime + k), glm::sin(lastTime + k), 0.0f));
        //std::cout << "shield velocity: " << glm::to_string(shield->GetVelocity()) << std::endl;
        shield->Update(delta_time);

        glm::mat4 around = glm::translate(glm::mat4(1.0f), shield->GetVelocity());
        //glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), shield->GetPosition());
        glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(shield->GetScale(), shield->GetScale(), 1.0));

        shield->Render(shader_, player->GetMovementMatrix() * around * scaling_matrix);
    }
}

void Game::buoyCollision(GameObject* object, GameObject* buoy) {
    glm::vec3 n = glm::normalize(object->GetPosition() - buoy->GetPosition());
    glm::vec3 v1 = object->GetVelocity();
    glm::vec3 v2 = buoy->GetVelocity();

    float m1 = object->GetMass();
    float m2 = buoy->GetMass();

    glm::vec3 v1prime = v1 - ((2 * m2) / (m1 + m2)) * (glm::dot(n, v1 - v2)) * n;
    glm::vec3 v2prime = v2 - ((2 * m1) / (m1 + m2)) * (glm::dot(n, v2 - v1)) * n;

    object->SetVelocity(v1prime);
    buoy->SetVelocity(v2prime);
}

void Game::Update(double delta_time) {

    // Handle user input
    if (!game_over && !frozen) {
        Controls();
    }
    if(game_over || numEnemies == 0){
        exit(0);
    }

    // Update and render all game objects
    for (int i = 0; i < game_objects_.size(); i++) {
        // Get the current game object
        GameObject* current_game_object = game_objects_[i];

        // Update the current game object
        current_game_object->Update(delta_time);

        // Check for collision with other game objects
        for (int j = i + 1; j < game_objects_.size(); j++) {
            GameObject* other_game_object = game_objects_[j];

            // Collision detection between player and enemies
            float distance = glm::length(current_game_object->GetPosition() - other_game_object->GetPosition());

            // Player/Enemy interaction (Moving/Patrolling)
            if(typeid(*current_game_object) == typeid(PlayerGameObject) && typeid(*other_game_object) == typeid(EnemyGameObject)){
                if (distance < 1.5f) {
                    other_game_object->SetState("moving");
                    glm::vec3 resultantVector = GetVectorBetweenTwoPoints(other_game_object->GetPosition(), current_game_object->GetPosition());

                    float angle = glm::angle(resultantVector, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));

                    // Enemy state = moving
                    other_game_object->SetVelocity(glm::vec3(resultantVector.x, resultantVector.y, 0.0f));
                    other_game_object->SetAngle(angle + 90);
                }
                else {
                    other_game_object->SetState("patrolling");
                }
            }
            else if (typeid(*current_game_object) == typeid(PlayerGameObject) && typeid(*other_game_object) == typeid(SeekerGameObject)) {
                glm::vec3 resultantVector = GetVectorBetweenTwoPoints(other_game_object->GetPosition(), current_game_object->GetPosition());

                float angle = glm::angle(resultantVector, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));

                other_game_object->SetVelocity(glm::vec3(resultantVector.x, resultantVector.y, 0.0f));
                other_game_object->SetAngle(angle + 90);
            }
            else if (typeid(*current_game_object) == typeid(PlayerGameObject) && typeid(*other_game_object) == typeid(PenguinGameObject)) {
                if (distance < 1.5f) {
                    other_game_object->SetState("moving");
                    glm::vec3 resultantVector = GetVectorBetweenTwoPoints(other_game_object->GetPosition(), current_game_object->GetPosition());

                    float angle = glm::angle(resultantVector, glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));

                    // Enemy state = moving
                    other_game_object->SetVelocity(glm::vec3(resultantVector.x, resultantVector.y, 0.0f));
                    other_game_object->SetAngle(angle + 90);
                }
                else {
                    other_game_object->SetState("patrolling");
                }
            }


            if (distance < 1.0f && current_game_object->GetCollidable() && other_game_object->GetCollidable()) {
                if (typeid(*other_game_object) != typeid(BuoyGameObject)) { // Not a buoy so can apply destruction logic
                    if (!shielded && i == 0) { // Not shielded
                        
                        std::cout << "currentgameobject collidable is " << current_game_object->GetCollidable() << std::endl;
                        std::cout << "Explode";
                        game_over = true;
                        SetTexture(tex_[0], (resources_directory_g + std::string("/textures/explosion.png")).c_str()); //Player
                        SetTexture(tex_[1], (resources_directory_g + std::string("/textures/explosion.png")).c_str()); //Enemy 1
                        SetTexture(tex_[2], (resources_directory_g + std::string("/textures/explosion.png")).c_str()); //Enemy 2

                        current_game_object->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
                        other_game_object->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));

                        // Add the sound
                        PlayExplosionAudio();
                    }
                    else { // Shielded
                        if (i == 0) {
                            //std::cout << "collided with enemy but shielded" << std::endl;
                            game_objects_.erase(game_objects_.begin() + j);
                            current_game_object->RemoveShields();
                            shielded = false;
                            numEnemies--;
                            continue;
                        }
                    }
                }
                else { // It's a buoy so have appropriate collision response
                    //std::cout << "It's a buoy" << std::endl;
                    GameObject* buoy = other_game_object;
                    buoyCollision(current_game_object, buoy);
                }
            }

            // Checking for collision of power up
            if (typeid(*current_game_object) == typeid(PlayerGameObject) && typeid(*other_game_object) == typeid(ShieldPowerUp)) {
                if (distance < 1.0f) {
                    //std::cout << "collided with shield" << std::endl;
                    glm::vec3 curpos = current_game_object->GetPosition();
                    
                    game_objects_.erase(game_objects_.begin() + j); // Erases the power up

                    if (!shielded) {
                        createShields(curpos);

                        for (int k = 0; k < current_game_object->GetShields().size(); k++) {
                            GameObject* shield = current_game_object->GetShields()[k];
                            shield->SetScale(0.25f);
                        }
                        shielded = true;
                    }
                }
            }
            else if (typeid(*current_game_object) == typeid(PlayerGameObject) && typeid(*other_game_object) == typeid(StarPowerUp)) {
                if (distance < 1.0f) {
                    //std::cout << "collided with star" << std::endl;
                    glm::vec3 curpos = current_game_object->GetPosition();

                    game_objects_.erase(game_objects_.begin() + j); // Erases the power up

                    current_game_object->SetCollidable(false);
                    lastInvincible = glfwGetTime();
                    invincible = true;

                }
            }
            else if (typeid(*current_game_object) == typeid(PlayerGameObject) && typeid(*other_game_object) == typeid(PenguinGameObject)) {
                if (distance < 1.0f) {
                    game_objects_.erase(game_objects_.begin() + j); // Erases the penguin
                    lastFrozen = glfwGetTime();
                    frozen = true;
                    current_game_object->SetVelocity(glm::vec3(0.0f, 0.0f, 0.0f));
                }
            }
            else if (typeid(*current_game_object) == typeid(PlayerGameObject) && typeid(*other_game_object) == typeid(ArrowPowerUp)) {
                if (distance < 1.0f) {
                    game_objects_.erase(game_objects_.begin() + j); // Erases the power up
                    arrowPowerUp = true;
                }
            }

        }

        // Render game object
        current_game_object->Render(shader_);


        // 'Parent' Main sprite object
        if (i == 0) {
            // Render the blades
            renderBlades();

            // Render the shields (if exists)
            if (!current_game_object->GetShields().empty()) {
                renderShields(delta_time);
            }
            if (bulletExists) {
                renderBullet(delta_time);
            }
            if (arrowExists) {
                renderArrow(delta_time);
            }

            double currentTime = glfwGetTime();
            double invincibleDifference = currentTime - lastInvincible;
            double frozenDifference = currentTime - lastFrozen;

            if (invincible) {
                if (invincibleDifference >= 5.0) {
                    current_game_object->SetCollidable(true);
                    invincible = false;
                }
            }
            if (frozen) {
                if (frozenDifference >= 3.0) {
                    frozen = false;
                }
            }
        }

        // If bullet exists, update and check for collisions
        if (bulletExists) {
            bulletUpdate();
        }

        

    }
    if (arrowExists) {
        arrowUpdate();
    }
}
       
} // namespace game
