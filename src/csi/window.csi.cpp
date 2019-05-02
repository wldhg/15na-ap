// Widh Jio
// window.csi.cpp

#include "csi.hpp"
#include "keras.hpp"

using std::thread;
using std::mutex;

mutex psMutex;
PacketQueue csi::pacStore;
unsigned short pacIter = 0;

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
        // Gather window & front <SYAA_SLIDE> items of queue
        PacketVector tmpStore;
        psMutex.lock();
        for (unsigned short i = 0; i < SYAA_WINDOW; i++) {
          tmpStore.push_back(pacStore.front());
          if (i < SYAA_SLIDE)
          {
            pacStore.pop();
          }
        }
        psMutex.unlock();
        // Convert these things to CSI array
        CSIVector& csis = csi::getCSIVector(tmpStore);
        // Do Keras
        keras::predict(csis);
        // Free values
        for (unsigned short i = 0; i < SYAA_SLIDE; i++) {
          free(tmpStore.at(i));
        }
      });
      thProcKeras.detach();
    }
  }
}
