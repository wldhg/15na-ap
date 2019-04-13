<em> <strong>Now Under Development</strong> </em>

# POSCA Server

This is server which does real-time situation classification.

## Requirements

-   Python 3.6 & `keras` or `keras-gpu`
-   CMake 3.7+
-   GCC 4.9+
-   [`frugally-deep`](https://github.com/Dobiasd/frugally-deep/blob/master/INSTALL.md)

## Preparation

I recommend you to do below in WSL or Linux environment.

1. When you installed `frugally-deep` following above link, you may cloned `frugally-deep` repository. Go on that directory and enter into `keras_export` directory.
2. Run `python convert_model.py [Your H5 File] [Output JSON Path]`. For example, enter `python convert_model.py model.h5 model.json`.

## How To Build

1. `git clone https://github.com/widh/posca-server`
2. `cd posca-server`
3. `cmake .`
4. `make`

Then, there must be a binary executable file named `poscas`.

## How To Run

Create `model` directory on root directory of the clone of this repository. Copy your JSONified model file (`(MODEL NAME).json`) into `model` directory. And you can run POSCA server by typing `./poscas (MODEL NAME)`. For more options and detailed instructions, type `./poscas --help`.

## License

This program is under **MPL-2.0** license. Read [LICENSE.md](LICENSE.md) for more informations.
