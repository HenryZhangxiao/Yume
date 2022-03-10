/*
 *
 * A skeleton for a 2D game
 *
 * Copyright (c) 2020-2021 Oliver van Kaick <Oliver.vanKaick@carleton.ca>, David Mould <mould@scs.carleton.ca>
 *
 */

#include <iostream>
#include <exception>
#include "game.h"

// Macro for printing exceptions
#define PrintException(exception_object)\
    std::cerr << exception_object.what() << std::endl

// Main function that builds and runs the game
int main(void){
    game::Game the_game;

    try {
        // Initialize graphics libraries and main window
        the_game.Init();
        // Setup the game (scene, game objects, etc.)
        the_game.Setup();
        // Run the game
        the_game.MainLoop();
    }
    catch (std::exception &e){
        // Catch and print any errors
        PrintException(e);
    }

    return 0;
}
