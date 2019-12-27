/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// ws.hpp

#include <sio_client.h>
#include <mutex>
#include <vector>
#include <condition_variable>

#include "15na.hpp"

#ifndef IRONA_WS
#define IRONA_WS

namespace ws
{
extern std::mutex it;
extern std::condition_variable_any cond;
extern bool connOK;
extern std::string url;
extern sio::socket::ptr soc;

extern sio::client cli;

void bindListeners(sio::client *);
void bindEvents();
void registerAP(unsigned ph1 = 0, unsigned ph2 = 0);

void init();
void send(unsigned long long, uint8_t *);
void con2Central();
} // namespace ws

#endif
