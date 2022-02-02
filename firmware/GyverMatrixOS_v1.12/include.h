#ifndef includeh 
#define includeh

#define ADD_COLOR 0x010101


int8_t fig = 0, ang = 0, pos = WIDTH / 2, height = HEIGHT - 1;
int8_t prev_ang, prev_pos, prev_height;

uint32_t colors[6] {0x0000EE, 0xEE0000, 0x00EE00, 0x00EEEE, 0xEE00EE, 0xEEEE00};
uint32_t color = 0x000088;

byte color_index;
byte linesToClear;
boolean down_flag = true;
byte lineCleanCounter;

#endif
