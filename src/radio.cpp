#include <WiFiClient.h>
#include "radio.h"
#include "player.h"

WiFiClient webradio_client;

bool dataPanic = false;

void radio_open_url(const char *host, const char *path)
{
    if (webradio_client.connect(host, 80)) {
    }

    circBuffer.flush();
    webradio_client.print(String("GET ") + path + " HTTP/1.0\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");
}

void radio_handle_stream(void)
{
    if (webradio_client.available()) {
        int bytes_read_from_stream = 0;

        if (circBuffer.room() > 999) {
            bytes_read_from_stream = webradio_client.read((uint8_t *)readBuffer, min(1000, (int)webradio_client.available()));
            if (bytes_read_from_stream > 0) {
                circBuffer.write(readBuffer, bytes_read_from_stream);
            }
        }
    }

    if (circBuffer.available()) {
        if (player.data_request()) {
            {
                int bytesRead = circBuffer.read((char *)mp3buff, 32);
                player.playChunk(mp3buff, bytesRead);
            }
        }
    } else {
        if (!dataPanic) {
            dataPanic = true;
        } else {
            dataPanic = false;
        }
    }
    return;
}

bool radio_isRunning()
{
    return webradio_client.connected();
}
