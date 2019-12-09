# 15na AP App

<i>IRONA CSI Uploader for Access Points</i>

This is a program sends `BFEE_NOTIF` packets to [`15na-server`](https://github.com/wldh-g/15na-server).\
This does not include a function of injecting `BFEE_NOTIF` packets.\
This works on Linux environment.

## Requirements

-   **CMake 3.8+**
-   **GCC/G++ 7+**
-   **Boost C++**: `boost`, `boost_sytem`, `boost_date_time`, `boost_random` (for `socket.io-client-cpp`)
-   **OpenSSL Library** (for `socket.io-client-cpp`)

## How To Build

1. `git clone --recurse-submodules https://github.com/wldh-g/15na-ap.git`
2. `cd 15na-ap`
3. `cmake .`
4. `make`

Then, there may be a executable file named `15na-ap` in `bin` directory.

## How To Run

This application is based on [dhalperi's CSI tool](https://dhalperi.github.io/linux-80211n-csitool/).\
So to use this app, Dan Halperin's CSI tool driver & firmware must be installed and wlan driver must be loaded with flag `connector_log=0x1`.\
Also, on both Tx & Rx device, you may need [DataCollector](https://github.com/wldh-g/BPI-R2-CSITool-DataCollector).

My own experiment device is BPI-R2. Customized kernel & tools for BPI-R2 is available on [BPI-R2-CSITool-Kernel](https://github.com/wldh-g/BPI-R2-CSITool-Kernel) and [BPI-R2-CSITool-DataCollector](https://github.com/wldh-g/BPI-R2-CSITool-DataCollector).

#### On Transmitter

For now, packet injection is not available with this app. This app just captures packets and sends it to the server.
So you have to run `random_packets` binary of [DataCollector](https://github.com/wldh-g/BPI-R2-CSITool-DataCollector).

1. Do `make` on `injection` directory of *DataCollector*.
2. Run `prepare_tx.sh` of root directory of *DataCollector*.
3. Run `./random_packets` of `injection` directory.

#### On Receiver

1. Run `prepare_rx.sh` of *[DataCollector](https://github.com/wldh-g/BPI-R2-CSITool-DataCollector)*.
2. Run `./15na-ap` of `bin` directory of this repository.

## What's Next?

I'll remove dependency with *DataCollector* and integrate Tx features to this app.

## License

This program is under **MPL-2.0** license. Read [LICENSE.md](LICENSE.md) for more informations.

`socket.io-client-cpp` is licensed under [MIT license](https://github.com/socketio/socket.io-client-cpp/blob/master/LICENSE).

`rapidjson` is licensed under [MIT license](https://github.com/Tencent/rapidjson/blob/master/license.txt).
