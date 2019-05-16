// Widh Jio
// window.csi.cpp

#include <cstring>
#include <arpa/inet.h>

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
unsigned long long windowAllSize = 0;

void csi::pushPacket(unsigned short len, uint8_t *bytes)
{
  stMutex.lock();
  pacStore.push(Packet(len, bytes));
  windowAllSize += len + 2;
  if (++pacIter == IRONA_WINDOW)
  {
    // Reset counter
    unsigned long long tempWinAllSize = windowAllSize;
    windowAllSize = 0;
    stMutex.unlock();
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
      uint8_t *csis = (uint8_t *)malloc(tempWinAllSize);
      stMutex.lock();
      unsigned long long cursor = 0;
      for (unsigned short i = 0; i < IRONA_WINDOW; i++)
      {
        uint16_t len = get<0>(pacStore.front());
        csis[cursor] = (len >> 8) & 0b0000000011111111;
        csis[cursor + 1] = len & 0b0000000011111111;
        memcpy(&(csis[cursor + 2]), get<1>(pacStore.front()), len);
        free(get<1>(pacStore.front()));
        pacStore.pop();
        cursor += len + 2;
      }
      stMutex.unlock();
      if (wannaDebugWindow)
      {
        $debug << $ns("csi") << "W" << currentWindowCount << ": Packet collected" << endl;
      }
      // Request socketio sending
      ws::send(tempWinAllSize, csis);
    });
    thProcSend.detach();
  } else {
    stMutex.unlock();
  }
}
