# countdown

This project is targeting to build a C++ library for thread-safe counting down.

## Features

* Header-only library.
* Terminating with a counting-down timer waiting process in other threads with thread-safe ensurance.

## How to build

1. Clone: `git clone https://github.com/Liam0205/countdown.git`.
2. Change Directory: `cd countdown`.
3. Initialize sub-modules: `git submodule init`.
4. Clone sub-modules: `git submodule update`.
5. Build: `./blade build -p release ...` or `./blade build -p debug ...` for debug mode.
6. Test: `./blade test`.
