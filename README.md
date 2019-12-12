# 15na AP App

<i>IRONA CSI Uploader for Access Points</i>

This is a program sends & collects & sends `BFEE_NOTIF` packets to [`15na-server`](https://github.com/wldh-g/15na-server).\
This works on Linux environment.

## Requirements

-   **CMake 3.8+**
-   **GCC/G++ 7+**
-   **Boost C++**: `boost`, `boost_sytem`, `boost_date_time`, `boost_random` (for `socket.io-client-cpp`)
-   **OpenSSL Library** (for `socket.io-client-cpp`)
-   **PCAP Library** (for `lorcon-old`)

Other used libraries are included in this repository as a form of submodule.

## How To Build

1. `git clone --recurse-submodules https://github.com/wldh-g/15na-ap.git`
2. `cd 15na-ap`
3. `cmake .`
4. `make`

Then, there may be a executable file named `15na-ap` in `bin` directory.

## How To Run

This application works with [dhalperi's CSI tool](https://dhalperi.github.io/linux-80211n-csitool/).
So to use this app, Dan Halperin's CSI tool driver & firmware must be installed and wlan driver must be loaded with flag `connector_log=0x1`.

My own device is [BPI-R2](http://www.banana-pi.org/r2.html). Customized kernel & tools for BPI-R2 is available on [BPI-R2-CSITool-Kernel](https://github.com/wldh-g/BPI-R2-CSITool-Kernel) and [BPI-R2-CSITool-DataCollector](https://github.com/wldh-g/BPI-R2-CSITool-DataCollector).

#### On Transmitter

1. Run `15na-ap -t`.

#### On Receiver

1. Register new AP and get `APID` from [`15na-server`](https://github.com/wldh-g/15na-server).
2. Run `15na-ap [APID] -p=[PORT of 15na-server] -n=[ADDR of 15na-server]`.

For more information and options, run `15na-ap --help`.

## License

This program is under **MPL-2.0** license. Read [LICENSE.md](LICENSE.md) for more informations and check [here](https://en.wikipedia.org/wiki/Comparison_of_free_and_open-source_software_licenses#General_comparison) for comparison of general licenses.

`socket.io-client-cpp` is licensed under [MIT license](https://github.com/wldh-g/socket.io-client-cpp/blob/master/LICENSE).

`rapidjson` is licensed under [MIT license](https://github.com/Tencent/rapidjson/blob/master/license.txt).

`lorcon` is licensed under [GPL-2.0](https://github.com/wldh-g/lorcon-old/blob/master/COPYING).
