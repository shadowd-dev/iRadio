#ifndef RADIO_H
#define RADIO_H

void radio_open_url(const char *host, const char *path);
void radio_handle_stream(void);
bool radio_isRunning();

#endif