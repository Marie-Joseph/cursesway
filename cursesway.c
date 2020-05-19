/* cursesway.c
 *
 * This is a simple ncurses implementation of Conway's Game of Life. It allows
 * one to either enter a seed pattern or generate one randomly.
 *
 * Author: Marie-Joseph
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>

// convenience functions
void printWorld(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT]);
void iterGen(int WIDTH, int HEIGHT, char curMap[WIDTH][HEIGHT]);
int liveCheck(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT]);
void cleanup(void);

// user commands
void genRandom(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT]);
int dumpMap(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT]);
void pauseSim(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT]);

int main(void)
{
    int WIDTH, HEIGHT;

    srandom(time(NULL));

    // start curses
    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    keypad(stdscr, TRUE);

    clear();

    // setup for leaving curses
    atexit(cleanup);

    // setup worldmap
    WIDTH = COLS - 1;
    HEIGHT = LINES - 1;
    char map[WIDTH][HEIGHT];
    int ch;
    for (int i = 0; i < WIDTH; i++)
    {
        for (int j = 0; j < HEIGHT; j++)
        {
            map[i][j] = ' ';
        }
    }
    mvaddstr(HEIGHT, 0, "Press 'r' to create a random generation and 'space' to start the simulation; alternatively, navigate with WASD and give 'life' to a cell (or remove it) with 'l'");
    printWorld(WIDTH, HEIGHT, map);

    // get user input on creating a map or generating a random one
    int cursX = 0;
    int cursY = 0;
    move(cursY, cursX);
    while ((ch = getchar()) != ' ')
    {
        switch (ch)
        {
            case 'r':
                genRandom(WIDTH, HEIGHT, map);
                break;

            case 'l':
                map[cursX][cursY] = map[cursX][cursY] == '*' ? ' ' : '*';
                printWorld(WIDTH, HEIGHT, map);
                move(cursY, cursX);
                break;

            case 'w':
                move(--cursY, cursX);
                break;

            case 's':
                move(++cursY, cursX);
                break;

            case 'a':
                move(cursY, --cursX);
                break;

            case 'd':
                move(cursY, ++cursX);
                break;
        }
        refresh();
        getyx(stdscr, cursY, cursX);
    }
    clear();

    mvaddstr(HEIGHT, 0, "Press 'e' to end simulation, space to pause");
    mvaddstr(HEIGHT, 45, "Generation 0");
    printWorld(WIDTH, HEIGHT, map);

    int generation = 1;
    while (liveCheck(WIDTH, HEIGHT, map))
    {
        usleep(100000);
        if ((ch = getch()) != ERR)
        {
            switch (ch) {
                case 'e':
                    exit(0);
                    break;

                case ' ':
                    //TODO: instructions
                    pauseSim(WIDTH, HEIGHT, map);
                    break;

                case 'r':
                    //TODO: reset
                    break;
            }
        }

        iterGen(WIDTH, HEIGHT, map);
        mvprintw(HEIGHT, 56, "%d", generation);
        // x coordinate comes from other string, but we don't want to have to
        // import string.h just for strlen()
        printWorld(WIDTH, HEIGHT, map);
        generation++;
    }

    exit(0);
}

/* a function to print a map onto the screen */
void printWorld(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT])
{
    int i, j;

    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < HEIGHT; j++)
        {
            mvaddch(j, i, map[i][j]);
        }
    }
    refresh();
}

/* a function to iterate the map to the next generation */
void iterGen(int WIDTH, int HEIGHT, char curMap[WIDTH][HEIGHT])
{
    int i, j;
    char tempMap[WIDTH][HEIGHT];

    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < HEIGHT; j++)
        {
            int neighbors = 0;
            neighbors += curMap[i+1][j+1] == '*' ? 1 : 0;
            neighbors += curMap[i+1][j] == '*' ? 1 : 0;
            neighbors += curMap[i+1][j-1] == '*' ? 1 : 0;
            neighbors += curMap[i][j+1] == '*' ? 1 : 0;
            neighbors += curMap[i][j-1] == '*' ? 1 : 0;
            neighbors += curMap[i-1][j+1] == '*' ? 1 : 0;
            neighbors += curMap[i-1][j] == '*' ? 1 : 0;
            neighbors += curMap[i-1][j-1] == '*' ? 1 : 0;

            if (curMap[i][j] == ' ')
            {
                tempMap[i][j] = neighbors == 3 ? '*' : ' ';
            }
            else if (curMap[i][j] == '*')
            {
                tempMap[i][j] = neighbors < 2 || neighbors > 3 ? ' ' : '*';
            }
        }
    }

    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < HEIGHT; j++)
        {
            curMap[i][j] = tempMap[i][j];
        }
    }
}

/* checks if any cells are alive */
int liveCheck(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT])
{
    int i, j;

    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < HEIGHT; j++)
        {
            if (map[i][j] == '*') {return 1;}
        }
    }
    return 0;
}

/* return the terminal to normal functionality */
void cleanup(void)
{
    nodelay(stdscr, TRUE);
    echo();
    nocbreak();
    endwin();
}

/* create a random starting generation */
void genRandom(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT])
{
    int i, j;

    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < HEIGHT; j++)
        {
            map[i][j] = random() % 8 == 0 ? '*' : ' ';
        }
    }

    printWorld(WIDTH, HEIGHT, map);
}

/* enter a pause state */
void pauseSim(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT])
{
    int ch;

    while ((ch = getchar()) != ' ')
    {
        switch (ch)
        {
            case 'd':
                dumpMap(WIDTH, HEIGHT, map);
                break;

            case 'e':
                exit(0);
                break;

            case 'r':
                //TODO: reset
                break;
        }
    }
}

/* dump current generation to a text file */
int dumpMap(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT])
{
    int i, j;
    FILE *dumpFile = fopen("dump.txt", "w");
    if (dumpFile == NULL)
    {
        fprintf(stderr, "Unable to create dump file.\n");
        return 1;
    }

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            fputc(map[j][i], dumpFile);
        }
        fputc('\n', dumpFile);
    }

    fclose(dumpFile);
    return 0;
}
