all: cursesway

cursesway: clean cursesway.c
	gcc -Wall -o cursesway -lcurses cursesway.c

clean:
	rm ./cursesway
	rm ./errors.txt
