<h1 align="center">
    YUME
  <br>
  <br>
  <p align="center">
  	<img width="460" height="300" src="https://user-images.githubusercontent.com/44578113/167240350-daef9c0e-a3bb-45e9-b7f1-f954fb680a54.png">
	</p>
</h1>

## Introduction

A 2D real-time top-view aircraft shooter written in C++.


#### Table of Contents
- [Softwares and Libraries used](#softwares)
- [Rules and Features](#rules)
- [How to Run](#run)


<br></br>
## Softwares and Libraries used <a name="softwares"></a>

- Windows
- Visual Studio
- OpenGL
- OpenAL
- SOIL
- ALUT
- GLEW
- GLFW
- GLM
- CMake
- Git


<br></br>
## Rules and Features <a name="rules"></a>
- Game Mechanics
  - Just killing the enemies will win the game <br><br>
- Enemies
  - 3 different enemies
    - Ghosts patrol and move towards the player if too close
	 - Clowns are seekers who will constantly follow the player
	 - Penguins will freeze the player for three seconds rendering them completely immobile <br><br>
- Weapons
  - 2 different weapons
    - Bullets are the regular weapon which can be triggered with spacebar with a small delay between each bullet
    - The second weapon is a power up which is an arrow that does not get destroyed on impact, ie. can kill multiple enemies. Fire the arrow with v once you get the power up <br><br>
- Collectible items
  - 3 collectible power ups
    - Shield that blocks one would-be death from a seeker or a ghost
    - Star that grants ghost-mode for the player, ie. invincibility for a short 5 seconds
	 - A arrow power up that gives you the arrow weapon <br><br>
- Game World
  - The game world is continuously scrolling with the camera positioned on the player


<br></br>
## How to Run <a name="run"></a>
- Download and build the project using CMake
- Assign the Library path to the Library folder provided
- Set Yume as the Startup Project
- Run
