/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// Modified from `github:dhalperi/linux-80211n-csitool-supplementary`
// lorcon.csi.cpp

#include <functional>
#include <ctime>
#include <unistd.h>
#include <tx80211.h>
#include <tx80211_packet.h>
//#include "../../lib/lorcon/tx80211.h"
//#include "../../lib/lorcon/tx80211_packet.h"

#include "csi.hpp"

using std::endl;

typedef struct _lorcon_packet {
  __le16 fc;
  __le16 dur;
  u_char addr1[6];
  u_char addr2[6];
  u_char addr3[6];
  __le16 seq;
  u_char payload[0];
} __attribute__((packed)) lorcon_packet;

void csi::makePacket()
{
  $info << $ns("csi") << "Preparing transmission...";
  tx80211_t tx;
  tx80211_packet_t txPacket;
  uint8_t *payloadBuffer;
  lorcon_packet *packet;

  // Initialize lorcon tx struct
  int driverType = tx80211_resolvecard("iwlwifi");
  if (tx80211_init(&tx, "mon0", driverType) < 0)
  {
    $error << $ns("csi") << "LORCON error: " << tx80211_geterrstr(&tx) << endl;
    terminate("Failed to initialize lorcon.");
  }
  if (tx80211_open(&tx) < 0)
  {
    terminate("Failed to open LORCON interface: mon0");
  }

  // Initialize lorcon tx packet struct
  tx80211_initpacket(&txPacket);

  // Initialize payload buffer
  unsigned int payloadBufferSize = pktSize * 10000;
  payloadBuffer = (uint8_t*)malloc(payloadBufferSize);
  if (payloadBuffer == NULL) {
    terminate("Failed to allocate payload buffer.");
  }
  uint32_t seed = 0x1f3d5b79;
  auto lsfr = [](uint32_t s) {
    return (s << 1) | (((s >> 31) ^ (s >> 29) ^ (s >> 25) ^ (s >> 24)) & 1);
  };
  for (unsigned short i = 0; i < payloadBufferSize; i += 1) {
    seed = lsfr(seed);
    seed = lsfr(seed);
    seed = lsfr(seed);
    payloadBuffer[i] = seed & 0xff;
  }

  // Initialize a packet
  packet = (lorcon_packet*)malloc(sizeof(*packet) + pktSize);
  if (packet == NULL) {
    terminate("Failed to allocate packet.");
  }
  packet->fc = (0x08 | (0x0 << 8));
  packet->dur = 0xffff;
  memcpy(packet->addr1, "\x00\x16\xea\x12\x34\x56", 6);
  memcpy(packet->addr2, "\x00\x16\xea\x12\x34\x56", 6);
  memcpy(packet->addr3, "\xff\xff\xff\xff\xff\xff", 6);
  packet->seq = 0;
  txPacket.packet = (uint8_t*)packet;
  txPacket.plen = sizeof(*packet) + pktSize;

  $success << $ns("csi") << "Transmission prepared. Starts to broadcast..." << endl;

  // Send packets
  struct timespec startTime, nowTime;
  __uint64_t timeDiff;
  __uint64_t pkti = 0;
  if (pktDelay > 0) {
    clock_gettime(CLOCK_MONOTONIC, &startTime);
  }
  while (true) {
    // Copy payload to packet
    for (unsigned short i = 0; i < pktSize; i += 1) {
      packet->payload[i] = payloadBuffer[(pkti * pktSize + i) % payloadBufferSize];
    }

    // Make a delay
    if (pktDelay > 0) {
      clock_gettime(CLOCK_MONOTONIC, &nowTime);
      timeDiff = (nowTime.tv_sec - startTime.tv_sec) * 1000000
                 + (nowTime.tv_nsec - startTime.tv_nsec + 500) / 1000;
      timeDiff = pktDelay * pkti - timeDiff;
      if (timeDiff > 0 && timeDiff < pktDelay) usleep(timeDiff);
    }

    // Send packet!
    if (tx80211_txpacket(&tx, &txPacket) < 0) {
      $warn << $ns("csi") << "Failed to transmit a packet." << endl;
    } else if (((pkti + 1) % 5000) == 0) {
      $info << $ns("csi") << (pkti + 1) / 1000 << " k th packet emitted." << endl;
    }

    pkti += 1;
  }
}
