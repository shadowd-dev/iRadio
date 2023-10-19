#include <WiFiManager.h>
#include <SPI.h>
#include "config.h"
#include "stations.h"
#include "player.h"
#include "radio.h"
#include "a2dp.h"
#include "control.h"

#define RADIO 0
#define BLUETOOTH 1

int sound_mode = RADIO;
int volume = 70;

void mode(int sound_mode_new)
{
    if (sound_mode == BLUETOOTH && sound_mode_new == RADIO) {
        bluetoothsink_end();
    } else if(sound_mode == RADIO && sound_mode_new == BLUETOOTH) {
        bluetoothsink_start();
    }
    player.softReset();
    delay(100);
    sound_mode = sound_mode_new;
}

void setup()
{
    SPI.begin();
    pinMode(BUTTON_ENCODER, INPUT_PULLUP);
    player.begin();
    player.setVolume(70);

    WiFiManager wm;
    bool res;
    res = wm.autoConnect("iRadio");
    if (!res) {
    } else { 
    }
    
    delay(100);
    bluetoothsink_setup();
    delay(100);
    mode(RADIO);
}

void loop()
{
    static int lastms_100ms = 0;
    static int station = 0;

    if (sound_mode == RADIO) {
        if (!radio_isRunning()) {
            radio_open_url(stations_host[station], stations_path[station]); 
        }
        radio_handle_stream();
    } else if (sound_mode == BLUETOOTH) {
        bluetoothsink_handle_stream();
        player.setVolume(volume);
    }

    front_read_encoder();

    if (millis() - lastms_100ms > 100) {
        lastms_100ms = millis();
        front_read_buttons();
    }

    if (encoder_button_long_pressed) {
        encoder_button_long_pressed = false;
        if (sound_mode < 1) {
            mode(sound_mode + 1);
        } else {
            mode(0);
        }
    }

    if (encoder_button_short_pressed) {
        encoder_button_short_pressed = false;
        if (sound_mode == RADIO){
            if (station < (stations_cnt - 1)) {
                station++;
            } else {
                station = 0;
            }
            radio_open_url(stations_host[station], stations_path[station]);
        }
    }

    if (encoder_turn_left) {
        encoder_turn_left = false;
        if (volume > 70) {
            volume--;
            player.setVolume(volume);
        }
    }

    if (encoder_turn_right){
        encoder_turn_right = false;

        if (volume <= 95) {
            volume++;
            player.setVolume(volume);
        }
    }

    if (f_bluetoothsink_metadata_received) {
        f_bluetoothsink_metadata_received = false;
    }
}
