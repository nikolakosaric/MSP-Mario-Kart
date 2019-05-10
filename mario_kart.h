/*
 * mario_kart.h
 *
 *  Created on: Mar 20, 2019
 *      Author: nikol
 */

/** @defgroup mario MSP Mario Kart
 * @file mario_kart.h
 * @addtogroup mario MSP Mario Kart
 *
 * Created on: Match 20, 2019
 * @author Nikola Kosaric
 *
 * @brief A terminal based game where the player uses to keyboard to maneuver a vehicle and avoid obstacles.
 *
 * MSP Mario Kart is a terminal based game, meant to mimic the Nintendo game, Mario Kart.
 * The player uses his/her keyboard to maneuver a vehicle left and right. The track is drawn, and the user must stay on the track.
 * The longer the play stays on track, the quicker the car accelerates.
 * If the user goes off track, the car reaches its minimum speed, and then must accelerate back to its top speed.
 * Going off track is defined by the car intersecting with any of the '-' lines running horizontal.
 * Once the player is off track, the terminal will illuminate red.
 * The player must travel a certain distance before he/she completes the race.
 * Once the race is complete, the player's score is given to him/her based off of his/her time.
 * The lower the time, the better the player did.
 *
 * The track is plotted by storing the position of the left end of the track in an array, and then drawing a specified track width,
 * and filling the remainder of the track.
 *
 * In order to play the game, type the following into the terminal:
 * - $game mario play
 *
 * In order to get help regarding how to play the game, type the following into the terminal:
 * - $game mario help
 *
 * Controls
 * - 'A', 'a', and '<' all move the car to the left
 * - 'D', 'd', and '>' all move the car to the right
 *
 * Controls can also be seen in the help screen.
 *
 * The MSP Mario Kart game relies on the following modules from the embedded software library:
 * - <a href="https://muhlbaier.github.io/embedded-software/group__random__int.html">Random Integer Module</a>
 * - <a href="https://muhlbaier.github.io/embedded-software/group__game.html">Game Module</a>
 * - <a href="https://muhlbaier.github.io/embedded-software/group__terminal.html">Terminal Module</a>
 * - <a href="https://muhlbaier.github.io/embedded-software/group__timing.html">Timing Module</a>
 * - <a href="https://muhlbaier.github.io/embedded-software/group__task.html">Task Module</a>
 * - <a href="https://muhlbaier.github.io/embedded-software/group__subsystem.html">Subsystem Module</a>
 * - <a href="https://muhlbaier.github.io/embedded-software/group__uart.html">UART Module</a>
 *
 * Documentation for all of these module can be seen by clicking on the title of the module.
 * (You will be taken to the resulting embedded softwares Doxygen library)
 *
 * @{
 */

#ifndef MARIO_KART_H_
#define MARIO_KART_H_

#include <stdint.h>
#include <stdint.h>
#include "project_settings.h"
#include "random_int.h"
#include "stddef.h"
#include "strings.h"
#include "game.h"
#include "timing.h"
#include "task.h"
#include "terminal.h"
#include "subsystem.h"
#include "uart.h"

#define WINDOW_WIDTH 25 /*!< Define for the width of the track. Allows for quick modification of window size.
Because of utilization in functions, provides modularity and ability to change size of track without having to
rewrite significant ammounts of code. */
#define WINDOW_HEIGHT 10 /*!< Define for the height of the track. Allows for quick modification of window size.
Because of utilization in functions, provides modularity and ability to change size of track without having to
rewrite significant ammounts of code. */
#define TRACK_WIDTH 10 /*!< Define for the width of the track at each point. Allows for quick modification of the
trach. Because of utilization in functions, provides modularity and ability to change size of track without having to
rewrite significant ammounts of code. This value can be used to increase or decrease the difficulty of the trap. */
#define MAXSPEED 5 /*!< Define for the maximum speed that the car can move. */
#define MINSPACE 1 /*!< Define for the minimum speed that the car can move. */
#define MAXSPACE 7 /*!< Define for the maximum distance from the left wall of the track to the left wall of the screen.
Used to draw the track.*/
#define MAXCYCLES 350 /*!< Define for the maximum number of cycles the user must complete before the race is completed.
This serves as the 'distance' the user must race. To lengthen the race, this number needs to be increased*/

/**
 * @brief Structure to hold all the variables utilized throughout the game
 *
 * @param x - Horizontal coordinate of the car on the track
 * @param c - Character of the car itself that shows in the terminal
 * @param s - Speed of the car used to adjust speed throughout the track
 * @param ts - Time driving without a collision in order to help determine proper speed
 * @param st - Start time of the race used to calculate the time the player took to complete the race
 * @param ft - Finish time of the race used to calculate the time the player took to complete the race
 * @param ss - Speed of the car from 1 to 5 used to adjust speed throughout the track
 * @param t - Time it took for the player to complete the race (finish time - start time)
 * @param i - Index of cycles completed (distance driven)
 * @param id - ID of the game
 */
struct mario_kart
{
    uint8_t x;  // coordinate of the car
    uint8_t c;  // character of the car itself
    uint16_t s;  // speed of car
    uint32_t ts;   // time without a collision to determine speed
    uint32_t st;   // start time of the race
    uint32_t ft;   // finishing time of the race
    uint8_t ss;
    uint32_t t; // time of race
    uint16_t i; //
    uint8_t id; ///< ID of game
};

/**
 * @brief Initialized the MSP Mario Kart game
 *
 * This function is called from Main.c to initialize the game. Once it is called, it brings up the game setup in termine.
 * The user must type in either
 * - '$game mario play'
 *
 * to begin playing the game, or
 * - '$game mario help'
 *
 * to get information on what the controls are.
 *
 * This function registers the game with the game module and clears the terminal screen so that the game can be displayed without issue.
 */
void Mario_Kart_Init(void);

/**
 * @brief Starts the game
 *
 * This function is called once the user types in '$game mario play' and begins the game. The car and track are plotted on the terminal.
 * The car initally moves at the slowest speed, and begins to accelerate over time.
 *
 * This function is where the initial variables are set, such as the color of the terminal, border around the track, start speed,
 * index location, start time, and player character. This function also initializes the player receiver function so that the
 * player can control the car. Finally, the task of drawing the track is scheduled.
 */
void Play(void);

/**
 * @brief Provides the player with helpful information
 *
 * This function is called once the user types in '$game mario help' and the help section is displayed.
 * It tells the player the controls for the game.
 */
void Help(void);

/**
 * @brief Generates the newest section of the track.
 *
 * This function is used to generate a new section of the track each time the player advances an index.
 * It uses the random module to determine a random distance from the wall of the terminal to the left end of the track.
 * This value is later used in the drawTrack function to complete plotting the track.
 *
 * The value is stored in an array, which holds the leftmost position of each track section.
 * As the player moves through the index, the array is updated, moving where each position is stored in the array, removing the
 * leftmost track position that was passed, and adding the new leftmost track position.
 */
void newTrackWidth(void);

/**
 * @brief Draws the newest section of the track
 *
 * This function draws the track by plotting the leftmost track positions in the array. Then a gap is placed in between the left
 * wall and the right wall. This gap is defined above as TRACK_WIDTH. The remainder of the track to the right is filled in order
 * to plot the entirety of the track, As the array is updated in the newTrackWidth function, the draw track function is called
 * to update the terminal.
 */
void drawTrack(void);

/**
 * @brief Moves the player's location based on the key pressed
 *
 * Every time the player presses a key, the Receiver function is called. This ensures that there is minimum latency
 * with the player's inputs. If the player presses one of the pre-defined keys, it moves his/her position either left
 * or right. This is done by calling either the MoveLeft or MoveRight function based on what key is pressed.
 */
void Receiver(uint8_t c);

/**
 * @brief Moves the position of the player to the left
 *
 * Given that the player is not at the left most position, the player is moved by decrementing his/her x position.
 * If the player is at the left most position, it will not update his/her location to ensure that he/she is not
 * plotted outside of the window.
 */
void MoveLeft(void);

/**
 * @brief Moves the position of the playet to the right
 *
 * Given that the player is not at the right most position, the player is moved by incrementing his/her x position.
 * If the player is at the right most position, it will not update his/her location to ensure that he/she is not
 * plotted outside of the window.
 */
void MoveRight(void);

/**
 * @brief Detects if the player has collided with the track walls
 *
 * This function is called every time the receiver function is called to ensure that the speed is updated in the event of a collision.
 * The function compares the players x position with the position of the track walls. If the two are equal, the time since collision
 * is set to zero in order to reset the speed. If a collision is detected, the color of the screen is set to red, otherwise the color
 * returns to black.
 */
void DetectCollision(void);

/**
 * @brief Sets the speed at which the car moves
 *
 * This function determines the players speed based on the amount of time since the last collision. If a collision is detected
 * the speed is set back to one. The longer the player goes without colliding with a wall, the more the speed increases. Speed is
 * increased by updating increasing the frequency at which the newTrackWidth function is called.
 */
void SetSpeed(void);

/**
 * @brief Prints a congratulations message with score once the player finishes the race
 *
 * This function is called once the player has completed the entire distance of the track. Once the player completes the race,
 * the time to complete the race is calculated and displayed on the screen.
 */
void FinishRace(void);

/** @}*/

#endif /* MARIO_KART_H_ */
