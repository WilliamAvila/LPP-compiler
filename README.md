# LPP-compiler
## Getting Started

1.First you need to install the dependencies
* [**flex**](https://www.gnu.org/software/flex/) on Ubuntu 16.04 run `sudo apt install flex` or on MAC OSX you can install it with [homebrew](https://brew.sh/) `brew install flex`

* [**lemon**](http://www.hwaci.com/sw/lemon/) to compile it run `gcc lemon.c -o lemon`

2.Create the executable running the Makefile with `make`

3.Run the samples with `./expr < samples/test1.txt`

To run the test 1 run `make test`
If you want remove all the generated files run `make clean`

