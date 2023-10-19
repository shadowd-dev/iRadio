#include <RotaryEncoder.h>
#include "config.h"
#include "control.h"

bool encoder_button_laststate = true;
bool encoder_button_pressed = false;
bool encoder_button_short_pressed = false;
bool encoder_button_long_pressed = false;
bool encoder_turn_left = false;
bool encoder_turn_right = false;
unsigned long encoder_button_pressedTime  = 0;
unsigned long encoder_button_releasedTime = 0;

RotaryEncoder encoder(ROTARY_A, ROTARY_B, RotaryEncoder::LatchMode::FOUR3);

void front_read_buttons()
{

  if (encoder_button_laststate == HIGH && digitalRead(BUTTON_ENCODER) == LOW) {
    encoder_button_pressedTime = millis();
  } else if (encoder_button_laststate == LOW && digitalRead(BUTTON_ENCODER) == HIGH) {
    encoder_button_releasedTime = millis();
    long pressDuration = encoder_button_releasedTime - encoder_button_pressedTime;
    if (pressDuration < (int) 1000) {
        encoder_button_short_pressed = true;
    }
    if (pressDuration > (int) 1000) {
      encoder_button_long_pressed = true;
    }
  }
  encoder_button_laststate = digitalRead(BUTTON_ENCODER);
}

void front_read_encoder()
{
    static int pos = 0;
    encoder.tick();

    int newPos = encoder.getPosition();
    if (pos != newPos) {
        if ((int)(encoder.getDirection()) == 1) {
            encoder_turn_right = true;
        } else {
            encoder_turn_left = true;
        }
        pos = newPos;
    }
}