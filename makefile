all: cursesway

cursesway: cursesway.c
	cc -Wall -Oz -o cursesway -lcurses cursesway.c

dev: cursesway.c
	cc -Wall -Og -g -lcurses -o cursesway cursesway.c

clean:
	rm ./cursesway
	rm ./errors.txt
