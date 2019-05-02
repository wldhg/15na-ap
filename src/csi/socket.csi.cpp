// Widh Jio
// socket.csi.cpp
// Motivated by `github:dhalperi/linux-80211n-csitool-supplementary`

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/netlink.h>
#include <cstring>

#include "csi.hpp"

using std::endl;
using std::thread;

// Global Variables
size_t bufSize = 4096;
int soc = -1;
unsigned short pacCount = 0;

void csi::openSocket()
{
  // Initialize socket variables
  char *buf = (char *)calloc(bufSize, sizeof(char));
  struct sockaddr_nl *procAddr = new (struct sockaddr_nl);
  procAddr->nl_family = PF_NETLINK;
  procAddr->nl_pid = getpid();
  procAddr->nl_groups = CN_IDX_IWLAGN;

  $info << $ns("csi") << "Initializing connector socket..." << endl;

  // Open socket
  soc = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
  if (soc == -1)
  {
    terminateP("Socket Open Error");
  }

  // Bind addresses to the socket
  if (bind(soc, (struct sockaddr *)procAddr, sizeof(struct sockaddr_nl)) == -1)
  {
    terminateP("Socket Bind Error");
  }

  // Subscribe to netlink group
  if (setsockopt(soc, 270, NETLINK_ADD_MEMBERSHIP, &procAddr->nl_groups, sizeof(procAddr->nl_groups)))
  {
    terminateP("Socket Option Error");
  }

  $info << $ns("csi") << "Connector successfully binded" << endl;

  // Create new thread to capture packets
  thread thCapture([=]() {
    struct cn_msg *cmsg;
    unsigned short windowTenSize = SYAA_WINDOW * 10;
    $info << $ns("csi") << "Started to receive from socket" << endl;
    for (;;)
    {
      if (recv(soc, buf, sizeof(buf), 0) == -1)
      {
        errorP("Socket Receive Error");
      }
      cmsg = (struct cn_msg *)NLMSG_DATA(buf);
      uint16_t len = htons(cmsg->data[0]);
      uint8_t code = cmsg->data[2];
      if (code == 187)
      {
        // If BFEE_NOTIF packet
        csi::BBPacket *procBuf = (csi::BBPacket *)calloc(sizeof(uint8_t), len - 1);
        memcpy(procBuf, &(cmsg->data[3]), sizeof(uint8_t) * (len - 1));
        csi::be2lePacket(procBuf);
        csi::pushPacket(procBuf);
        if (++pacCount % windowTenSize == 0)
        {
          pacCount = 0;
          $debug << $ns("csi") << "W10 Reached" << endl;
        }
      }
      if (wannaDebugPacket == true)
      {
        $debug << $ns("csi") << "Packet Received: Len=" << (int)len << " Code=" << (int)code << endl;
      }
    }
  });
  thCapture.join();
}

void csi::be2lePacket(csi::BBPacket * pkt) {
  // Do nothing because csi calculation doesn't use uint16_t or bigger variable.
}
