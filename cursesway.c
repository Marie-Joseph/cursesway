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
void printWorld(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH]);
void iterGen(int HEIGHT, int WIDTH, char curMap[HEIGHT][WIDTH]);
int liveCheck(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH]);
void cleanup(void);

/* User commands */
void genRandom(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH]);
int dumpMap(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH]);
void pauseSim(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH]);

/* Easter egg */
void goodbyeConway(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH]);

int main(int argv, char **argc)
{
    /*freopen("errors.txt", "a", stderr);*/

    unsigned int ch, i, j, curs_x, curs_y, generation;
    /*unsigned int gen_time, rand_seed;*/
    struct timespec wait_time;

    wait_time.tv_sec = 0;
    wait_time.tv_nsec = 100000000;

    srand(time(NULL));

    /* Start curses */
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

    clear();

    /* Setup for leaving curses */
    atexit(cleanup);

    /* Setup worldmap */
    register const unsigned int HEIGHT = LINES - 1;
    register const unsigned int WIDTH = COLS - 1;
    char map[HEIGHT][WIDTH];
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            map[i][j] = ' ';
        }
    }
    mvaddstr(HEIGHT, 0, "Press 'r' to create a random generation and 'space' to start the simulation; alternatively, navigate with WASD and give 'life' to a cell (or remove it) with 'l'");
    printWorld(HEIGHT, WIDTH, map);

    /* Get user input on creating a map or generating a random one */
    curs_y = 0;
    curs_x = 0;
    move(curs_y, curs_x);
    while ((ch = getchar()) != ' ')
    {
        switch (ch)
        {
            case 'r':
                genRandom(HEIGHT, WIDTH, map);
                break;

            case 'l':
                map[curs_y][curs_x] = map[curs_y][curs_x] == '*' ? ' ' : '*';
                printWorld(HEIGHT, WIDTH, map);
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

            case 'c':
                goodbyeConway(HEIGHT, WIDTH, map);
                break;
        }
        refresh();
        getyx(stdscr, curs_y, curs_x);
    }
    clear();

    mvaddstr(HEIGHT, 0, "Press 'e' to end simulation, space to pause");
    mvaddstr(HEIGHT, 45, "Generation 0");
    printWorld(HEIGHT, WIDTH, map);

    generation = 1;
    while (liveCheck(HEIGHT, WIDTH, map))
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
                    pauseSim(HEIGHT, WIDTH, map);
                    break;

                case 'r':
                    /*TODO: reset*/
                    break;
            }
        }

        iterGen(HEIGHT, WIDTH, map);
        mvprintw(HEIGHT, 56, "%d", generation);
        /* x coordinate comes from the instruction string, but we don't want to
           have to import string.h just for strlen() */
        printWorld(HEIGHT, WIDTH, map);
        generation++;
    }

    exit(0);
}

/* Print a map onto the screen */
void printWorld(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH])
{
    int i, j;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            mvaddch(i, j, map[i][j]);
        }
    }
    refresh();
}

/* Iterate the map to the next generation */
void iterGen(int HEIGHT, int WIDTH, char curMap[HEIGHT][WIDTH])
{
    int i, j, neighbors;
    char tempMap[HEIGHT][WIDTH];

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            neighbors = 0;
            neighbors += curMap[i+1][j+1] == '*' ? 1 : 0;
            neighbors += curMap[i+1][j]   == '*' ? 1 : 0;
            neighbors += curMap[i+1][j-1] == '*' ? 1 : 0;
            neighbors += curMap[i][j+1]   == '*' ? 1 : 0;
            neighbors += curMap[i][j-1]   == '*' ? 1 : 0;
            neighbors += curMap[i-1][j+1] == '*' ? 1 : 0;
            neighbors += curMap[i-1][j]   == '*' ? 1 : 0;
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

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            curMap[i][j] = tempMap[i][j];
        }
    }
}

/* Check if any cells are alive */
/* TODO: check for stable state */
int liveCheck(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH])
{
    int i, j;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
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

/* Create a random starting generation */
void genRandom(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH])
{
    int i, j;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            map[i][j] = rand() % 8 == 0 ? '*' : ' ';
        }
    }

    printWorld(HEIGHT, WIDTH, map);
}

/* Enter a pause state */
void pauseSim(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH])
{
    int ch;

    while ((ch = getchar()) != ' ')
    {
        switch (ch)
        {
            case 'd':
                dumpMap(HEIGHT, WIDTH, map);
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
int dumpMap(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH])
{
    int i, j;
    FILE *dump_file = fopen("dump.txt", "w");
    if (dump_file == NULL)
    {
        fprintf(stderr, "Unable to create dump file.\n");
        return 1;
    }

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            fputc(map[i][j], dump_file);
        }
        fputc('\n', dump_file);
    }

    fclose(dump_file);
    return 0;
}

/* A wave goodbye to Conway */
/*
                 ***
                 * *
                 * *
                  *
               * ***
                * * *
                  *  *
                 * *
                 * *





           RIP John Conway
       1937-12-26 - 2020-04-11
*/
void goodbyeConway(int HEIGHT, int WIDTH, char map[HEIGHT][WIDTH])
{
    int i, j;
    int x_center = WIDTH / 2;
    int y_center = HEIGHT / 2;
    
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            map[i][j] = ' ';
        }
    }
    
    map[y_center - 4][x_center - 1]     = '*';
    map[y_center - 4][x_center]         = '*';
    map[y_center - 4][x_center + 1]     = '*';
    map[y_center - 3][x_center - 1]     = '*';
    map[y_center - 3][x_center + 1]     = '*';
    map[y_center - 2][x_center - 1]     = '*';
    map[y_center - 2][x_center + 1]     = '*';
    map[y_center - 1][x_center]         = '*';
    map[y_center][x_center - 3]         = '*';
    map[y_center][x_center - 1]         = '*';
    map[y_center][x_center]             = '*';
    map[y_center][x_center + 1]         = '*';
    map[y_center + 1][x_center - 2]     = '*';
    map[y_center + 1][x_center]         = '*';
    map[y_center + 1][x_center + 2]     = '*';
    map[y_center + 2][x_center]         = '*';
    map[y_center + 2][x_center + 3]     = '*';
    map[y_center + 3][x_center - 1]     = '*';
    map[y_center + 3][x_center + 1]     = '*';
    map[y_center + 4][x_center - 1]     = '*';
    map[y_center + 4][x_center + 1]     = '*';
    
    printWorld(HEIGHT, WIDTH, map);
    
    mvaddstr(y_center + 6, x_center - 7, "RIP John Conway");
    mvaddstr(y_center + 7, x_center - 13, "26 Dec. 1937 - 11 Apr. 2020");
}
