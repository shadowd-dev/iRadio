#ifndef CONTROL_H
#define CONTROL_H

extern bool encoder_button_pressed;
extern bool encoder_button_short_pressed;
extern bool encoder_button_long_pressed;
extern bool encoder_turn_left;
extern bool encoder_turn_right;

void front_read_buttons();
void front_read_encoder();

#endif