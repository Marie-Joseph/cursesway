## TODO
The following are future objectives. If you happen to feel like tackling any of them, feel free to do so and open a PR explaining what you've implemented.

* better instructions/proper documentation
* restart simulation (optionally w/ same seed or to initial screen)
* rollover functionality for all sides
* pre-program some neat seeds/common constructs (one easter egg in place already)
* allow loading in states (possibly from image formats)
* allow dumping to an image format
* support commandline arguments for things like:
    * generation length (currently ~1/10 second)
    * dumpfile format
    * premade seeds
    * "alive" cell character
    * worldspace size
* replace `nanosleep` and related calls to achieve ANSI compatibility
