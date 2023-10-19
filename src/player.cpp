#include <cbuf.h>
#include "VS1053.h"
#include "config.h"
#include "player.h"

cbuf circBuffer(1024 * 24);
char readBuffer[1000] __attribute__((aligned(4)));

uint8_t mp3buff[32];

VS1053 player(VS1053_CS, VS1053_DCS, VS1053_DREQ);