/* IRONA AP Program is subject to the terms of the Mozilla Public License 2.0.
 * You can obtain a copy of MPL at LICENSE.md of root directory. */
// init.core.cpp

#include <fstream>
#include <sys/stat.h>

#include "core.hpp"

using std::ofstream;
using std::endl;

void core::init(bool isTxMode)
{
  // Write configuration sh file in /tmp
  ofstream sh("/tmp/irona-init.sh", std::ios_base::trunc | std::ios_base::out);
  sh << "systemctl stop network-manager 2>/dev/null 1>/dev/null" << endl;
  sh << "systemctl stop wicd 2>/dev/null 1>/dev/null" << endl;
  sh << "systemctl stop rsyslog 2>/dev/null 1>/dev/null" << endl;
  sh << "systemctl stop syslog.socket 2>/dev/null 1>/dev/null" << endl;
  sh << "systemctl stop systemd-journald 2>/dev/null 1>/dev/null" << endl;
  sh << "systemctl stop systemd-journald-dev-log.socket 2>/dev/null 1>/dev/null" << endl;
  sh << "systemctl stop systemd-journald-audit.socket 2>/dev/null 1>/dev/null" << endl;
  sh << "systemctl stop systemd-journald.socket 2>/dev/null 1>/dev/null" << endl;
  sh << "modprobe -r iwldvm" << endl;
  sh << "modprobe iwlwifi " << (isTxMode ? "debug=0x40000" : "connector_log=0x1") << endl;
  sh << "ifconfig " << netInterface << " 2>/dev/null 1>/dev/null" << endl;
  sh << "while [ $? -ne 0 ]; do" << endl;
  sh << "  ifconfig " << netInterface << " 2>/dev/null 1>/dev/null" << endl;
  sh << "done" << endl;
  sh << "ifconfig " << netInterface << " down" << endl;
  if (isTxMode) {
    sh << "iw dev " << netInterface << " interface add mon0 type monitor" << endl;
    sh << "ifconfig mon0 up" << endl;
    sh << "iw dev mon0 set channel 64 HT40-" << endl;
    sh << "echo " << txFlag << " | sudo tee `sudo find /sys -name monitor_tx_rate` 1>/dev/null" << endl;
  } else {
    sh << "iwconfig " << netInterface << " mode monitor 2>/dev/null 1>/dev/null" << endl;
    sh << "while [ $? -ne 0 ]; do" << endl;
    sh << "  iwconfig " << netInterface << " mode monitor 2>/dev/null 1>/dev/null" << endl;
    sh << "done" << endl;
    sh << "ifconfig " << netInterface << " up" << endl;
    sh << "iw " << netInterface << " set channel 64 HT40-" << endl;
  }

  // Launch the initializing script
  if (system("/bin/bash /tmp/irona-init.sh")) {
    if (isTxMode) {
      terminate("Failed to initialize network interface. Check with this script: https://git.io/Je9Ag");
    } else {
      terminate("Failed to initialize network interface. Check with this script: https://git.io/Je9A2");
    }
  }
}
