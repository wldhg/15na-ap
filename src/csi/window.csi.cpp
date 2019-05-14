// Widh Jio
// window.csi.cpp

#include <cstring>

#include "csi.hpp"
#include "ws.hpp"

using std::endl;
using std::get;
using std::mutex;
using std::thread;
using std::tuple;

mutex stMutex;
mutex wcMutex;
PacketQueue csi::pacStore;
unsigned short pacIter = 0;
unsigned long long windowCount = 0;

void csi::pushPacket(unsigned short len, uint8_t *bytes)
{
  stMutex.lock();
  pacStore.push(Packet(len, bytes));
  stMutex.unlock();
  if (++pacIter == IRONA_WINDOW)
  {
    // Reset counter
    pacIter = 0;
    // Create new thread to copy packets
    thread thProcSend([=]() {
      wcMutex.lock();
      unsigned long long currentWindowCount = windowCount++;
      wcMutex.unlock();
      if (wannaDebugWindow)
      {
        $debug << $ns("csi") << "W" << currentWindowCount << ": Sending started" << endl;
      }
      // Gather window
      uint8_t *csis = (uint8_t *)malloc(215 * IRONA_WINDOW); // 213(data) + 2(len)
      stMutex.lock();
      for (unsigned short i = 0; i < IRONA_WINDOW; i++)
      {
        uint16_t len = get<0>(pacStore.front());
        csis[215 * i] = len >> 8;
        csis[215 * i + 1] = len & 0b11111111;
        memcpy(&(csis[215 * i + 2]), get<1>(pacStore.front()), 213);
        free(get<1>(pacStore.front()));
        pacStore.pop();
      }
      stMutex.unlock();
      if (wannaDebugWindow)
      {
        $debug << $ns("csi") << "W" << currentWindowCount << ": Packet collected" << endl;
      }
      // Request socketio sending
      ws::send(csis);
    });
    thProcSend.detach();
  }
}
