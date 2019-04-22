// Widh Jio
// window.csi.cpp

#include "csi.hpp"
#include "keras.hpp"

using std::thread;

PacketQueue csi::pacStore;
unsigned short pacIter = 0;

void csi::pushPacket(csi::BBPacket* packet) {
  pacStore.push(packet);
  if (++pacIter == SYAA_SLIDE)
  {
    // Reset counter
    pacIter = 0;
    if (csi::pacStore.size() >= SYAA_WINDOW) {
      // Create new thread to copy packets and process Keras
      // Note: New thread and this thread shares <pacStore> without mutex,
      //       But no property is hazarded logically.
      thread thProcKeras([=]() {
        // Gather window & front <SYAA_SLIDE> items of queue
        PacketVector tmpStore;
        for (unsigned short i = 0; i < SYAA_WINDOW; i++) {
          tmpStore.push_back(pacStore.front());
          if (i < SYAA_SLIDE)
          {
            pacStore.pop();
          }
        }
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
