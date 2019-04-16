<em> <strong>Now Under Development</strong> </em>

# POSCA Server

This is server which does real-time situation classification.\
I recommend you to do below in Linux environment.

## Requirements

-   **Python 3.6** & **`keras`** or **`keras-gpu`**
-   **CMake 3.8+**
-   **GCC/G++ 7+**
-   **Boost C++**: `system`, `thread`, `coroutine`, `context`
-   **Boost.Asio**
-   **OpenSSL Library**
-   **Eigen3**

## How To Build

1. `git clone --recurse-submodules https://github.com/widh/posca-server`
2. `cd posca-server`
3. `cmake .`
4. `sudo make`

Then, there must be a binary executable file named `poscas` in `bin` directory.

## How To Run

1. After build `poscas`, you may have `frugally-deep` directory in `lib`. Go on that directory and enter into `keras_export` directory.
2. Run `python convert_model.py [Your H5 File] [Output JSON Path]`. For example, enter `python convert_model.py model.h5 model.json`.
3. Copy your JSONified model file (`(MODEL NAME).json`) into `bin` directory, where ELF binary `poscas` exists. And you can run POSCA server by typing `./poscas (MODEL NAME).json`. For more options and detailed instructions, type `./poscas --help`.

## License

This program is under **MPL-2.0** license. Read [LICENSE.md](LICENSE.md) for more informations.
