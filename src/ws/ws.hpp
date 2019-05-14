// Widh Jio
// ws.hpp

#include <sio_client.h>
#include <mutex>
#include <vector>
#include <condition_variable>

#include "15na.hpp"

#ifndef POSCA_WS
#define POSCA_WS

namespace ws
{
extern std::mutex it;
extern std::condition_variable_any cond;
extern bool connOK;
extern std::string url;
extern sio::socket::ptr soc;

extern sio::client cli;

void bindListeners(sio::client *);
void bindEvents(sio::socket::ptr);

void init();
void send(uint8_t *);
void con2Central();
} // namespace ws

#endif
