/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// window.csi.cpp

#include <cstring>

#include "csi.hpp"
#include "ws.hpp"

using std::endl;
using std::get;
using std::mutex;
using std::thread;
using std::tuple;

mutex stMutex; // Protects pacIter, pacStore, windowAllSize
PacketList csi::pacStore;
unsigned short pacIter = 0;
unsigned long long windowAllSize = 0;
unsigned long long windowCount = 0; // Just for logging

void csi::pushPacket(unsigned short len, uint8_t *bytes)
{
  stMutex.lock();
  pacStore.push_back(Packet(len, bytes));
  windowAllSize += len + 2; // length(2) + message data(len)
  if (++pacIter == csi::ioWindowPkts)
  {
    // Reset counter
    unsigned long long tempWinAllSize = windowAllSize;
    // NOTE: Decreasing windowAllSize will be processed at below (i < IRONA_SEND_CNT)
    // NOTE: This could enlarge the size of csis, there will be no error, and will be successfully deleted.
    pacIter -= csi::ioSlidePkts;
    stMutex.unlock();
    // Create new thread to copy packets
    thread thProcSend([=]() {
      unsigned long long currentWindowCount = windowCount++;
      if (wannaDebugWindow)
      {
        $debug << $ns("csi") << "W" << currentWindowCount << ": Sending started" << endl;
      }
      // Gather window
      uint8_t *csis = (uint8_t *)malloc(tempWinAllSize);
      unsigned long long cursor = 0;
      stMutex.lock();
      Packet& target = pacStore.front();
      PacketList::iterator pacIt = pacStore.begin();
      for (unsigned short i = 0; i < csi::ioWindowPkts; i += 1)
      {
        // Put length in Big Endian way
        uint16_t len = get<0>(*pacIt);
        csis[cursor] = (len >> 8) & 0b0000000011111111;
        csis[cursor + 1] = len & 0b0000000011111111;
        // Copy data
        memcpy(&(csis[cursor + 2]), get<1>(*pacIt), len);
        cursor += len + 2;
        // Iterate
        ++pacIt;
      }
      for (unsigned short i = 0; i < csi::ioSlidePkts; i += 1)
      {
        free(get<1>(pacStore.front()));
        windowAllSize -= get<0>(pacStore.front()) + 2;
        pacStore.pop_front();
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
