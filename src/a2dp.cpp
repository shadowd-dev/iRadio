#include <BluetoothA2DPSink.h>
#include "player.h"
#include "a2dp.h"

BluetoothA2DPSink a2dp_sink;

unsigned char bt_wav_header[44] = {
    0x52, 0x49, 0x46, 0x46, // RIFF
    0xFF, 0xFF, 0xFF, 0xFF, // size
    0x57, 0x41, 0x56, 0x45, // WAVE
    0x66, 0x6d, 0x74, 0x20, // fmt
    0x10, 0x00, 0x00, 0x00, // subchunk1size
    0x01, 0x00,             // audio format - pcm
    0x02, 0x00,             // numof channels
    0x44, 0xac, 0x00, 0x00, //, //samplerate 44k1: 0x44, 0xac, 0x00, 0x00       48k: 48000: 0x80, 0xbb, 0x00, 0x00,
    0x10, 0xb1, 0x02, 0x00, //byterate
    0x04, 0x00,             // blockalign
    0x10, 0x00,             // bits per sample - 16
    0x64, 0x61, 0x74, 0x61, // subchunk3id -"data"
    0xFF, 0xFF, 0xFF, 0xFF  // subchunk3size (endless)
};
bool f_bluetoothsink_metadata_received = false;
char bluetooth_media_title[255];
int cnt = 0;

void bluetoothsink_avrc_metadata_callback(uint8_t data1, const uint8_t *data2)
{

    if (data1 == 0x1) {
        strncpy(bluetooth_media_title, (char *)data2, sizeof(bluetooth_media_title) - 1);
    } else if (data1 == 0x2) {
        strncat(bluetooth_media_title, " - ", sizeof(bluetooth_media_title) - 1);
        strncat(bluetooth_media_title, (char *)data2, sizeof(bluetooth_media_title) - 1);
        f_bluetoothsink_metadata_received = true;
    }
}

void bluetoothsink_handle_stream()
{
    if (circBuffer.available()) {
        if (player.data_request()) {
            int bytesRead = circBuffer.read((char *)mp3buff, 80);
            player.playChunk(mp3buff, bytesRead);
        }
    }
}

void bluetoothsink_read_data_stream(const uint8_t *data, uint32_t length)
{
    if (circBuffer.room() > length) {
        if (length > 0) {
            circBuffer.write((char *)data, length);
        }
    }
}

void bluetoothsink_setup()
{
    a2dp_sink.set_stream_reader(bluetoothsink_read_data_stream, false);
    a2dp_sink.set_avrc_metadata_callback(bluetoothsink_avrc_metadata_callback);
}

void bluetoothsink_start()
{
    a2dp_sink.start("iRadio");
    circBuffer.flush();
    delay(100);
    circBuffer.write((char *)bt_wav_header, 44);
    delay(100);
}

void bluetoothsink_end()
{
    a2dp_sink.end(false);
}
