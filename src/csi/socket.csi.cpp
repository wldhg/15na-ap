// Widh Jio
// socket.csi.cpp
// Motivated by `github:dhalperi/linux-80211n-csitool-supplementary`

#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <linux/netlink.h>

#include "csi.hpp"

using std::thread;

// Global Variables
size_t bufSize = 4096;
int soc = -1;

void csi::openSocket() {
  // Initialize socket variables
  struct sockaddr_nl *procAddr = new (struct sockaddr_nl);
  procAddr->nl_family = PF_NETLINK;
  procAddr->nl_pid = getpid();
  procAddr->nl_groups = CN_IDX_IWLAGN;

  // Initialize connector variables
  struct cn_msg *cmsg;
  char* buf = (char*) calloc(bufSize, sizeof(char));

  // Open socket
  soc = socket(PF_NETLINK, SOCK_DGRAM, NETLINK_CONNECTOR);
  if (soc == -1) {
    terminateP("Socket Open Error");
  }

  // Bind addresses to the socket
  if (bind(soc, (struct sockaddr *) procAddr, sizeof(struct sockaddr_nl)) == -1) {
    terminateP("Socket Bind Error");
  }

  // Subscribe to netlink group
  if (setsockopt(soc, 270, NETLINK_ADD_MEMBERSHIP, &procAddr->nl_groups, sizeof(procAddr->nl_groups))) {
    terminateP("Socket Option Error");
  }

  // Create new thread to capture packets
  thread thCapture([=]() {
    for (;;) {
      if (recv(soc, buf, sizeof(buf), 0) == -1) {
        terminateP("Socket Receive Error");
      }
      // cmsg = NLMSG_DATA(buf);
    }
  });
  thCapture.detach();
}
