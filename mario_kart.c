/*
 * mario_kart.c
 *
 *  Created on: Mar 18, 2019
 *      Author: Nikola
 */

//

#include "mario_kart.h"

static struct mario_kart game;

uint8_t leftLimits[WINDOW_HEIGHT - 1] = { 7, 0, 0, 0, 0, 0, 0, 0, 0 };

void Mario_Kart_Init()
{
    // Register the module with the game system and give it the name "MUH3"
    game.id = Game_Register("Mario", "Race", Play, Help);
    Game_ClearScreen();
}

void Help(void)
{
    Game_Printf(
            "Press 'a' to turn the car left\n and 'd' to turn the car right.");
    Game_ClearScreen();
}

void Play(void)
{
    // clear the screen
    Game_ClearScreen();
    Game_HideCursor();

    // draw a box around our map
    Game_SetColor(BackgroundBlack);
    Game_DrawRect(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    game.x = 12;
    game.c = '^';
    game.ss = 5;
    game.s = 175;
    game.i = 0;
    game.ts = TimeNow();
    Game_CharXY(game.c, game.x, WINDOW_HEIGHT - 1);
    Game_RegisterPlayer1Receiver(Receiver);

    Task_Schedule(drawTrack, 0, 1000, game.s);
}

void newTrackWidth()
{
    uint8_t y = 8;
    for (y = WINDOW_HEIGHT - 2; y > 0; y--)
    {
        leftLimits[y] = leftLimits[y - 1];
    }
    uint8_t min = leftLimits[1] - 2;
    uint8_t max = leftLimits[1] + 2;
    if (min <= 2)
    {
        min = 2;
    }
    if (max >= MAXSPACE)
    {
        max = MAXSPACE;
    }
    leftLimits[0] = random_int(min, max);

}

void drawTrack()
{
    uint8_t x = 1;
    uint8_t y = 1;

    for (y = 1; y < WINDOW_HEIGHT; y++)
    {
        for (x = 1; x < WINDOW_WIDTH; x++)
        {
            if (x <= leftLimits[y - 1])
            {
                Game_CharXY('-', x, y);
            }
            else if (leftLimits[y - 1] <= 0)
            {
                Game_CharXY(' ', x, y);
            }
            else if (x <= leftLimits[y - 1] + TRACK_WIDTH)
            {
                Game_CharXY(' ', x, y);

            }
            else
            {
                Game_CharXY('-', x, y);
            }
        }
    }

    game.i++;

    if (game.i == MAXCYCLES)
    {
        FinishRace();
    }
    else
    {
        Game_CharXY(game.c, game.x, WINDOW_HEIGHT - 1);
        Game_CharXY(game.ss + '0', WINDOW_WIDTH + 1, 1);
        DetectCollision();
        newTrackWidth();
        SetSpeed();
    }
}

void Receiver(uint8_t c)
{
    if (game.i != MAXCYCLES)
    {
        switch (c)
        {
        case 'a':
        case ',':
        case 'A':
        case '<':
            MoveLeft();
            break;
        case 'd':
        case 'D':
        case '.':
        case '>':
            MoveRight();
            break;
        default:
            break;
        }
    }
}

void MoveRight()
{
    // make sure we can move right
    if (game.x < WINDOW_WIDTH - 1)
    {
        // clear location
        Game_CharXY(' ', game.x, WINDOW_HEIGHT - 1);
        game.x++;
        // update
        Game_CharXY(game.c, game.x, WINDOW_HEIGHT - 1);
    }
}

void MoveLeft()
{
    // make sure we can move right
    if (game.x > 1)
    {
        // clear location
        Game_CharXY(' ', game.x, WINDOW_HEIGHT - 1);
        game.x--;
        // update
        Game_CharXY(game.c, game.x, WINDOW_HEIGHT - 1);
    }
}

void DetectCollision()
{
    if ((game.x <= leftLimits[WINDOW_HEIGHT - 2])
            || (game.x >= (leftLimits[WINDOW_HEIGHT - 2] + TRACK_WIDTH + 1)))
    {
        if (leftLimits[WINDOW_HEIGHT - 2] != 0)
        {
            game.ts = 0;
            Game_SetColor(BackgroundRed);
        }
    }
    else
    {
        game.ts++;
        Game_SetColor(BackgroundBlack);
    }
}

void SetSpeed()
{
    switch (game.ts)
    {
    case 0:
        game.ss = 1;
        game.s = 1000;
        break;
    case 10:
        game.ss = 2;
        game.s = 750;
        break;
    case 30:
        game.ss = 3;
        game.s = 500;
        break;
    case 50:
        game.ss = 4;
        game.s = 250;
        break;
    case 75:
        game.ss = 5;
        game.s = 175;
        break;
    }
    Task_ChangePeriod(drawTrack, game.s, 1);
}

void FinishRace()
{
    Game_SetColor(BackgroundBlack);
    Task_Remove(drawTrack, 0);
    game.ft = TimeNow();
    game.t = (game.ft - game.st) / 1000;
    Game_ClearScreen();
    Game_Printf("YOU WIN!\r\n");
    Game_Printf("Your time was %d seconds!", game.t);
}
