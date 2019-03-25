/*
 * mario_kart.c
 *
 *  Created on: Mar 18, 2019
 *      Author: Nikola
 */

//
#include <stdint.h>
#include "project_settings.h"
#include "random_int.h"
#include "stddef.h"
#include "strings.h"
#include "game.h"
#include "timing.h"
#include "task.h"
#include "terminal.h"
#include "random_int.h"

#define WINDOW_WIDTH 25
#define WINDOW_HEIGHT 10
#define TRACK_WIDTH 10

struct mario_kart
{
    uint8_t x;  // coordinate of the car
    uint8_t c;  // character of the car itself
    uint8_t speed;  // speed of car
    uint8_t timeWithoutCollision;   // time to determine speed
    uint8_t time;   // time of the race
    uint8_t id; ///< ID of game
};

#define MAXSPEED 5
#define MINSPACE 1
#define MAXSPACE 7

static struct mario_kart game;

static void Play(void);
static void Help(void);

static void initialTrackWidth(void);
static void newTrackWidth(void);
static void drawTrack(void);

uint8_t leftLimits[WINDOW_HEIGHT-1] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t rightLimits[WINDOW_HEIGHT -1] = {0, 0, 0, 0, 0, 0, 0, 0, 0};


void Mario_Kart_Init()
{
    // Register the module with the game system and give it the name "MUH3"
    game.id = Game_Register("Mario", "Race", Play, Help);
}

void Help(void)
{
    Game_Printf(
            "Press 'a' to turn the car left\n and 'd' to turn the car right.");
}

void Play(void)
{
    // clear the screen
    Game_ClearScreen();
    Game_HideCursor();

    // draw a box around our map
    Game_DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    initialTrackWidth();

    //drawTrack();
    Task_Schedule(drawTrack, 0, 1000, 750);
}

void initialTrackWidth() {
    leftLimits[0] = random_int(MINSPACE, MAXSPACE);
}

void newTrackWidth() {
    uint8_t y = 8;
    for (y = WINDOW_HEIGHT - 2; y > 0; y--) {
        leftLimits[y] = leftLimits[y-1];
    }
    uint8_t min = leftLimits[1] - 2;
    uint8_t max = leftLimits[1] + 2;
    if (min <= 2) {min = 2;}
    if (max >= MAXSPACE) {max = MAXSPACE;}
    leftLimits[0] = random_int(min, max);

}

void drawTrack() {
    uint8_t x = 1;
    uint8_t y = 1;

    for (y = 1; y < WINDOW_HEIGHT; y++){
        for (x = 1; x < WINDOW_WIDTH; x++){
            if (x <= leftLimits[y-1]){
                Game_CharXY('-', x, y);
            }
            else if (leftLimits[y-1] <= 0){
                    Game_CharXY(' ', x, y);
            }
            else if (x <= leftLimits[y-1] + TRACK_WIDTH){
                Game_CharXY(' ', x, y);

            } else {
                Game_CharXY('-', x, y);
            }
        }

    }
    newTrackWidth();
}
