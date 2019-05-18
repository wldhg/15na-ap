# 15na AP App

<i>15na Access Point Program</i>

This is a program sends `BFEE_NOTIF` packets to `15na-server`.\
I recommend you to do below in Linux environment.

## Requirements

-   **CMake 3.8+**
-   **GCC/G++ 7+**
-   **Boost C++**: `boost`, `boost_sytem`, `boost_date_time`, `boost_random`
-   **OpenSSL Library**

## How To Build

1. `git clone --recurse-submodules https://github.com/widh/15na-ap.git`
2. `cd 15na-ap`
3. `cmake .`
4. `make`

Then, there must be a binary executable file named `15na-ap` in `bin` directory.

## Notice

For now, configurations in `CMakeLists.txt` is set for `BPI-R2 (MT7623N, Cortex A7)`.\
You may change this to fit to your device.

## License

This program is under **MPL-2.0** license. Read [LICENSE.md](LICENSE.md) for more informations.
