# Cursesway

An ncurses implementation of [John Conway](https://en.wikipedia.org/wiki/John_Horton_Conway)'s [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

## Overview

Cursesway loads into a blank screen with a cursor and instructions along the bottom. You can move the cursor with `wasd` keys, and activate, or give "life" to, a cell with `l`. You can also generate a random seed with `r`. Once you're satisfied with your initial state, press the spacebar to activate the simulation. To end the simulation, press `ctrl+c`.

## Compilation

Cursesway uses simple C that should be supported by any compiler more ambitious than tcc. Assuming you're running a Unix-like-enough operating system and have a basic development environment, all you'll need is development files for ncurses. As an example setup on a Debian-based Linux distribution, `cd` to the directory where you've stored the sourcefile and run:

```
sudo apt update
sudo apt install build-essential libncurses5-dev
gcc -lcurses -o cursesway cursesway.c
./cursesway
```

## TODO
The following are future objectives. If you happen to feel like implementing them, feel free to do so and open a PR explaining what you've done.

* pause simulation
* restart simulation (optionally w/ same seed or from initial screen)
* dump current state (possibly to image format)