# 15na AP App

<i>IRONA CSI Uploader for Access Points</i>

This is a program sends `BFEE_NOTIF` packets to `15na-central`.\
This does not include a function of injecting `BFEE_NOTIF` packets.\
This works on Linux environment.

## Requirements

-   **CMake 3.8+**
-   **GCC/G++ 7+**
-   **Boost C++**: `boost`, `boost_sytem`, `boost_date_time`, `boost_random` (for `socket.io-client-cpp`)
-   **OpenSSL Library** (for `socket.io-client-cpp`)

## How To Build

1. `git clone --recurse-submodules https://github.com/widh/15na-ap.git`
2. `cd 15na-ap`
3. `cmake .`
4. `make`

Then, there may be a executable file named `15na-ap` in `bin` directory.

## Notice

IRONA AP is tested and optimized on `BPI-R2 (MT7623N, Cortex A7)`.

## License

This program is under **MPL-2.0** license. Read [LICENSE.md](LICENSE.md) for more informations.

`socket.io-client-cpp` is licensed under [MIT License](https://github.com/socketio/socket.io-client-cpp/blob/master/LICENSE).
