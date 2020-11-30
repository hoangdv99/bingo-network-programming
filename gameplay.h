#ifndef __GAMEPLAY__
#define __GAMEPLAY__

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define GAME_PLAYING 1
#define GAME_END 0

typedef enum{
    ANAUTH,
    INLOBBY,
    INROOM,
    INGAME
}GAMEPLAY_STATUS;

#endif