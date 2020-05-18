# Cursesway

An ncurses implementation of [John Conway](https://en.wikipedia.org/wiki/John_Horton_Conway)'s [Game of Life](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life).

Cursesway uses simple C that should be supported by any C compiler more admbitious than tcc. Assuming you're running a Unix-like-enough operating system and have a basic development environment, all you'll need is development files for ncurses. As an example on a Debian-based Linux distribution, `cd` to the directory where you've stored the sourcefile and run:

```
sudo apt update
sudo apt install build-essential libncurses5-dev
gcc -lcurses -o cursesway cursesway.c
```