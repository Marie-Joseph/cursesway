# Cursesway

An ncurses implementation of [John Conway](https://en.wikipedia.org/wiki/John_Horton_Conway)'s [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

## Usage

Cursesway loads into a blank screen with a cursor and instructions along the bottom. You can move the cursor with `wasd` keys, and activate, or give "life" to, a cell with `l`. You can also generate a random seed with `r`. Once you're satisfied with your initial state, press the spacebar to activate the simulation.

While the simulation is running, you can pause it with the spacebar or exit with `e`. If you pause the simulation, you can resume it with the spacebar or "dump" the current generation to a file with `d`. The file will be called `dump.txt`; be aware that this file is overwritten every time you dump the screen, so change the name if you want to save multiple dumps. `e` can be used to exit cleanly from pause.

Please note that if you exit the program with the common `ctrl+c`, your terminal will be left in a non-standard state. Make sure to exit with `e`.

## Quirks

The worldspace of Cursesway is not infinite, as a perfect implementation of Conway's Game of Life would require. Rather, [automata](https://en.wikipedia.org/wiki/Cellular_automaton) are bounded by the screen. Note that all space provided by your terminal is used, and this size is set at launch. In the future, users may be able to specify the size of the worldspace, even beyond the edges of their terminal. Either way, the ultimate functionality will allow automata to "rollover" the edges of the screen.

Currently, the controls for the pause state are not shown to the user. This is due to an unknown issue with printing functions and should be fixed at some point.

## Compilation

Cursesway uses simple C that should be supported by any compiler more ambitious than tcc. Just insure you have a POSIX-friendly C compiler (`clang` is the primary development compiler), a relatively standard `make`, and development files for (n)curses. For example, on a Debian-based Linux distribution (Debian, Ubuntu, Mint, elementary, BunsenLabs...), run:

```
sudo apt update
sudo apt install git build-essential libncurses-dev
```

Change the package management commands as needed for your operating system and/or taste.

Once you have the basic tools, the compilation itself is identical across systems:

```
git clone https://github.com/Marie-Joseph/cursesway.git
cd cursesway
make cursesway
./cursesway
```

## Supported platforms
Cursesway should run on any Unix-like operating system with ncurses (and others with some minor changes!), and has been tested on Fedora 32; Debian Stretch, Buster, Bullseye (testing), and Sid (unstable); Ubuntu 18.04, 19.10, and 20.04; elementary OS Juno; and FreeBSD 12.1. If you've compiled it on other systems - or encounter problems on other systems - I'd love to know!

## Bugs Beyond Scope
These known bugs exist outside of the scope of this project. They may be resolved as a side-effect, but they will not be directly targeted so long as workarounds exist.

* `gcc` 10 fails to link ncurses on Debian Bullseye (testing)
    * earlier versions of `gcc` seem to work fine
    * use `clang` if you want a newer C compiler
        * run `sudo update-alternatives --config cc` and select `clang` OR
        * change the makefile from using `cc` to using `clang`
