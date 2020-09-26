all: cursesway

cursesway: cursesway.c
	cc -Wall -Oz -lcurses -o cursesway cursesway.c

dev: cursesway.c
	cc -Wall -Og -g -lcurses -o cursesway cursesway.c

clean:
	rm ./cursesway
