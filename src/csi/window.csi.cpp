// Widh Jio
// window.csi.cpp

#include "csi.hpp"
#include "keras.hpp"

using std::thread;
using std::mutex;
using std::endl;

mutex psMutex;
mutex wcMutex;
PacketQueue csi::pacStore;
unsigned short pacIter = 0;
unsigned long long windowCount = 0;

void csi::pushPacket(csi::BBPacket* packet) {

  psMutex.lock();
  pacStore.push(packet);
  psMutex.unlock();
  if (++pacIter == SYAA_SLIDE)
  {
    // Reset counter
    pacIter = 0;
    psMutex.lock();
    bool shouldPredict = csi::pacStore.size() >= SYAA_WINDOW;
    psMutex.unlock();
    if (shouldPredict)
    {
      // Create new thread to copy packets and process Keras
      thread thProcKeras([=]() {
        wcMutex.lock();
        unsigned long long currentWindowCount = windowCount++;
        wcMutex.unlock();
        if (wannaDebugWindow)
        {
          $debug << $ns("csi") << "W" << currentWindowCount << ": processing started" << endl;
        }
        // Gather window & front <SYAA_SLIDE> items of queue
        PacketVector tmpStore;
        psMutex.lock();
        for (unsigned short i = 0; i < SYAA_WINDOW; i++)
        {
          tmpStore.push_back(pacStore.front());
          if (i < SYAA_SLIDE)
          {
            pacStore.pop();
          }
        }
        psMutex.unlock();
        if (wannaDebugWindow)
        {
          $debug << $ns("csi") << "W" << currentWindowCount << ": Packet collected" << endl;
        }
        // Convert these things to CSI array
        CSIVector& csis = csi::getCSIVector(tmpStore);
        // Do Keras
        keras::predict(csis);
        // Free values
        for (unsigned short i = 0; i < SYAA_SLIDE; i++)
        {
          free(tmpStore.at(i));
        }
        if (wannaDebugWindow)
        {
          $debug << $ns("csi") << "W" << currentWindowCount << ": Memory unallocated" << endl;
        }
      });
      thProcKeras.detach();
    }
  }
}
