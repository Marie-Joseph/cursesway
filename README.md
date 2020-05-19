# Cursesway

An ncurses implementation of [John Conway](https://en.wikipedia.org/wiki/John_Horton_Conway)'s [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

## Usage

Cursesway loads into a blank screen with a cursor and instructions along the bottom. You can move the cursor with `wasd` keys, and activate, or give "life" to, a cell with `l`. You can also generate a random seed with `r`. Once you're satisfied with your initial state, press the spacebar to activate the simulation.

Once the simulation is running, you can pause it with the spacebar or exit with `e`. If you pause the simulation, you can resume it with the spacebar or "dump" the current generation to a file with `d`. The file will be called `dump.txt`; be aware that this file is overwritten every time you dump the screen, so change the name if you want to save multiple dumps. `e` can be used to exit cleanly from pause.

Please note that if you exit the program with the common `ctrl+c`, your terminal will be left in a non-standard state. Make sure to exit with `e`.

## Quirks

The worldspace of Cursesway is not infinite, as a perfect implementation of Conway's Game of Life would require. Rather, [automata](https://en.wikipedia.org/wiki/Cellular_automaton) which would affect cells above or below the edge of the screen affect those in the same place relative to the opposite edge, while the right and left are hard borders. For example, if a [glider](https://en.wikipedia.org/wiki/Glider_(Conway%27s_Life)) would travel off the top of the screen, it will instead continue on from the bottom; but if it were to travel off the left of the screen, it would either die or freeze in its last stable state. I have no idea why this works the way it does.

Currently, the controls for the pause state are not shown to the user. This is due to an unknown issue with printing functions and should be fixed at some point.

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

* better instructions/proper documentation
* restart simulation (optionally w/ same seed or to initial screen)
* imitate rollover functionality for right and left
* pre-program some neat seeds/common constructs
* allow loading in states (possibly from image formats)
* allow dumping to an image format
* support commandline arguments for things like:
    * generation length (currently ~1/10 second)
    * dumpfile format
    * premade seeds
    * "alive" cell character