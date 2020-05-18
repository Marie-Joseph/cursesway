# Cursesway

An ncurses implementation of [John Conway](https://en.wikipedia.org/wiki/John_Horton_Conway)'s [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

## Overview

Cursesway loads into a blank screen with a cursor and instructions along the bottom. You can move the cursor with `wasd` keys, and activate, or give "life" to, a cell with `l`. You can also generate a random seed with `r`. Once you're satisfied with your initial state, press the spacebar to activate the simulation. To end the simulation, press `ctrl+c`.

## Quirks

The worldspace of Cursesway is not infinite, as a perfect implementation of Conway's Game of Life would require. Rather, [automata](https://en.wikipedia.org/wiki/Cellular_automaton) which would affect cells above or below the edge of the screen affect those in the same place relative to the opposite edge, while the right and left are hard borders. For example, if a [glider](https://en.wikipedia.org/wiki/Glider_(Conway%27s_Life)) would travel off the top of the screen, it will instead continue on from the bottom; but if it were to travel off the left of the screen, it would either die or freeze in its last stable state. I have no idea why this works the way it does.

## Compilation

Cursesway uses simple C that should be supported by any compiler more ambitious than tcc. Assuming you're running a Unix-like-enough operating system and have a basic development environment, all you'll need is development files for ncurses. As an example setup on a Debian-based Linux distribution, `cd` to the directory where you've stored the sourcefile and run:

```
sudo apt update
sudo apt install build-essential libncurses5-dev
gcc -lcurses -o cursesway cursesway.c
./cursesway
```

## TODO
The following are future objectives. If you happen to feel like tackling any of them, feel free to do so and open a PR explaining what you've implemented.

* pause simulation
* restart simulation (optionally w/ same seed or from initial screen)
* dump current state (possibly to image formats)
* imitate rollover functionality for right and left
* pre-program some neat seeds/common constructs
* allow loading in states (possibly from image formats)