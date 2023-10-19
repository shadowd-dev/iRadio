#ifndef PLAYER_H
#define PLAYER_H

#include <cbuf.h>
#include "VS1053.h"

extern cbuf circBuffer;
extern char readBuffer[1000] __attribute__((aligned(4)));
extern uint8_t mp3buff[32];
extern VS1053 player;

#endif