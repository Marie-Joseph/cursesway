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

/* Convenience functions */
void printWorld(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT]);
void iterGen(int WIDTH, int HEIGHT, char curMap[WIDTH][HEIGHT]);
int liveCheck(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT]);
void cleanup(void);
/*TODO:void nanosleep(unsigned long int nsecs);*/

/* User commands */
void genRandom(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT]);
int dumpMap(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT]);
void pauseSim(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT]);

int main(int argv, char **argc)
{
    unsigned int ch, i, j, curs_x, curs_y, generation, WIDTH, HEIGHT;
    /*unsigned int gen_time, rand_seed;*/
    struct timespec wait_time;

    wait_time.tv_sec = 0;
    wait_time.tv_nsec = 100000000;

    srand(time(NULL));

    /* Start curses */
    initscr();
    cbreak();
    nodelay(stdscr, TRUE);
    noecho();
    keypad(stdscr, TRUE);

    clear();

    /* Setup for leaving curses */
    atexit(cleanup);

    /* Setup worldmap */
    WIDTH = COLS - 1;
    HEIGHT = LINES - 1;
    char map[WIDTH][HEIGHT];
    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < HEIGHT; j++)
        {
            map[i][j] = ' ';
        }
    }
    mvaddstr(HEIGHT, 0, "Press 'r' to create a random generation and 'space' to start the simulation; alternatively, navigate with WASD and give 'life' to a cell (or remove it) with 'l'");
    printWorld(WIDTH, HEIGHT, map);

    /* Get user input on creating a map or generating a random one */
    curs_x = 0;
    curs_y = 0;
    move(curs_y, curs_x);
    while ((ch = getchar()) != ' ')
    {
        switch (ch)
        {
            case 'r':
                genRandom(WIDTH, HEIGHT, map);
                break;

            case 'l':
                map[curs_x][curs_y] = map[curs_x][curs_y] == '*' ? ' ' : '*';
                printWorld(WIDTH, HEIGHT, map);
                move(curs_y, curs_x);
                break;

            case 'w':
                move(--curs_y, curs_x);
                break;

            case 's':
                move(++curs_y, curs_x);
                break;

            case 'a':
                move(curs_y, --curs_x);
                break;

            case 'd':
                move(curs_y, ++curs_x);
                break;
        }
        refresh();
        getyx(stdscr, curs_y, curs_x);
    }
    clear();

    mvaddstr(HEIGHT, 0, "Press 'e' to end simulation, space to pause");
    mvaddstr(HEIGHT, 45, "Generation 0");
    printWorld(WIDTH, HEIGHT, map);

    generation = 1;
    while (liveCheck(WIDTH, HEIGHT, map))
    {
        nanosleep(&wait_time, NULL);
        if ((ch = getch()) != ERR)
        {
            switch (ch) {
                case 'e':
                    exit(0);
                    break;

                case ' ':
                    /*TODO: instructions*/
                    pauseSim(WIDTH, HEIGHT, map);
                    break;

                case 'r':
                    /*TODO: reset*/
                    break;
            }
        }

        iterGen(WIDTH, HEIGHT, map);
        mvprintw(HEIGHT, 56, "%d", generation);
        /* x coordinate comes from the instruction string, but we don't want to
           have to import string.h just for strlen() */
        printWorld(WIDTH, HEIGHT, map);
        generation++;
    }

    exit(0);
}

/* Print a map onto the screen */
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

/* Iterate the map to the next generation */
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

/* Check if any cells are alive */
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

/* Return the terminal to normal functionality */
void cleanup(void)
{
    nodelay(stdscr, TRUE);
    echo();
    nocbreak();
    endwin();
}

/* Sleep for nsecs nanoseconds. Portable replacement for POSIX function */
/*TODO:void nanosleep(unsigned long int nsecs)*/

/* Create a random starting generation */
void genRandom(int WIDTH, int HEIGHT, char map[WIDTH][HEIGHT])
{
    int i, j;

    for (i = 0; i < WIDTH; i++)
    {
        for (j = 0; j < HEIGHT; j++)
        {
            map[i][j] = rand() % 8 == 0 ? '*' : ' ';
        }
    }

    printWorld(WIDTH, HEIGHT, map);
}

/* Enter a pause state */
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
                /*TODO: reset*/
                break;
        }
    }
}

/* Dump the current generation to a text file */
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
