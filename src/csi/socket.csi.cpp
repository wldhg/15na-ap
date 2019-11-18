/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// Motivated by `github:dhalperi/linux-80211n-csitool-supplementary`
// socket.csi.cpp

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/netlink.h>
#include <cstring>

#include "csi.hpp"

using std::dec;
using std::endl;
using std::hex;
using std::thread;

// Global Variables
size_t bufSize = 11460;
int soc = -1;
unsigned short csi::ioWindowPkts = (unsigned short) (IRONA_SEND * IRONA_PPS + 0.5);
unsigned short csi::ioSlidePkts = (unsigned short) ((IRONA_SEND - IRONA_WINDOW + IRONA_SLIDE) * IRONA_PPS + 0.5);

void csi::openSocket()
{
  $debug << $ns("csi") << "In one window, " << csi::ioWindowPkts << " packets will be included." << endl;
  $debug << $ns("csi") << "Each windows has " << csi::ioSlidePkts << " of packet interval." << endl;
  $info << $ns("csi") << "Initializing connector socket..." << endl;

  // Create new thread to capture packets
  thread thCapture([=]() {
    // Initialize socket variables
    struct sockaddr_nl *procAddr = new (struct sockaddr_nl);
    procAddr->nl_family = AF_NETLINK;
    procAddr->nl_pid = getpid();
    procAddr->nl_groups = CN_IDX_IWLAGN;

    // Open socket
    soc = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
    if (soc == -1)
    {
      terminateP("Socket socket() Error");
    }

    // Bind addresses to the socket
    if (bind(soc, (struct sockaddr *)procAddr, sizeof(struct sockaddr_nl)) == -1)
    {
      terminateP("Socket bind() Error");
    }

    // Subscribe to netlink group
    if (setsockopt(soc, 270, NETLINK_ADD_MEMBERSHIP, &procAddr->nl_groups, sizeof(procAddr->nl_groups)))
    {
      terminateP("Socket setsockopt() Error");
    }

    $info << $ns("csi") << "Connector successfully binded" << endl;

    char *buf = (char *)malloc(bufSize);
    struct cn_msg *cmsg;
    for (;;)
    {
      if (recv(soc, buf, bufSize, 0) == -1)
      {
        errorP("Socket recv() Error");
        continue;
      }
      cmsg = (struct cn_msg *)NLMSG_DATA(buf);
      unsigned short len = cmsg->len;
      if ((unsigned char)cmsg->data[0] == 187)
      {
        // If BFEE_NOTIF packet
        uint8_t *procBuf = (uint8_t *)malloc((size_t)len);
        memcpy(procBuf, cmsg->data, len);
        csi::pushPacket(len, procBuf);
      }
      if (wannaDebugPacket == true)
      {
        $debug << $ns("csi") << "Packet Received: Len=" << len << " Code=0x" << hex << (int)cmsg->data[0] << dec << endl;
      }
    }
  });
  thCapture.join();
}
