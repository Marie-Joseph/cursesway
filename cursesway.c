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
void printWorld();
void iterGen(char **tempMap);
int liveCheck();
void cleanup();

/* User commands */
void genRandom();
int dumpMap();
void pauseSim();

/* Easter egg */
void goodbyeConway();

/* Global MAP and size */
char **MAP;
size_t HEIGHT, WIDTH;

int main()
{
    /*freopen("errors.txt", "a", stderr);*/

    unsigned int ch, i, j, curs_x, curs_y, generation;
    char **tempMap;
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

    /* Setup worldMAP */
    HEIGHT = LINES - 1;
    WIDTH = COLS - 1;
    MAP = (char**)calloc(HEIGHT, sizeof(char*));
    tempMap = (char **)calloc(HEIGHT, sizeof(char*));
    for (i = 0; i < HEIGHT; i++) {
        MAP[i] = (char*)calloc(WIDTH, sizeof(char));
        tempMap[i] = (char *)calloc(WIDTH, sizeof(char));
        for (j = 0; j < WIDTH; j++) {
            MAP[i][j] = ' ';
        }
    }

    mvaddstr(HEIGHT, 0, "Press 'r' to create a random generation and 'space' to start the simulation; alternatively, navigate with WASD and give 'life' to a cell (or remove it) with 'l'");
    printWorld();

    /* Get user input on creating a MAP or generating a random one */
    curs_y = 0;
    curs_x = 0;
    move(curs_y, curs_x);
    while ((ch = getchar()) != ' ')
    {
        switch (ch)
        {
            case 'r':
                genRandom();
                break;

            case 'l':
                MAP[curs_y][curs_x] = MAP[curs_y][curs_x] == '*' ? ' ' : '*';
                printWorld();
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
                goodbyeConway();
                break;
        }
        refresh();
        getyx(stdscr, curs_y, curs_x);
    }
    clear();

    mvaddstr(HEIGHT, 0, "Press 'e' to end simulation, space to pause");
    mvaddstr(HEIGHT, 45, "Generation 0");
    printWorld();

    generation = 1;
    while (liveCheck())
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
                    pauseSim();
                    break;

                case 'r':
                    /*TODO: reset*/
                    break;
            }
        }

        iterGen(tempMap);
        mvprintw(HEIGHT, 56, "%d", generation);
        /* x coordinate comes from the instruction string, but we don't want to
           have to import string.h just for strlen() */
        printWorld();
        generation++;
    }

    exit(0);
}

/* Print a MAP onto the screen */
void printWorld()
{
    int i, j;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            mvaddch(i, j, MAP[i][j]);
        }
    }
    refresh();
}

/* Iterate the MAP to the next generation */
void iterGen(char **tempMap)
{
    int i, j, neighbors;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            neighbors = 0;
           
           if (i < HEIGHT-1) {
                neighbors += MAP[i+1][j]   == '*' ? 1 : 0; 
 
                if (j < WIDTH-1)
                    neighbors += MAP[i+1][j+1] == '*' ? 1 : 0;
                
                if (j > 0)
                    neighbors += MAP[i+1][j-1] == '*' ? 1 : 0;
            }
            
            if (j < WIDTH-1)
                neighbors += MAP[i][j+1]   == '*' ? 1 : 0;
            
            if (j > 0)
                neighbors += MAP[i][j-1]   == '*' ? 1 : 0;
            
            if (i > 0) {
                neighbors += MAP[i-1][j]   == '*' ? 1 : 0;

                if (j > 0)
                    neighbors += MAP[i-1][j-1] == '*' ? 1 : 0;

                if (j < WIDTH-1)
                    neighbors += MAP[i-1][j+1] == '*' ? 1 : 0;
            }

            if (MAP[i][j] == ' ')
            {
                tempMap[i][j] = neighbors == 3 ? '*' : ' ';
            }
            else if (MAP[i][j] == '*')
            {
                tempMap[i][j] = neighbors < 2 || neighbors > 3 ? ' ' : '*';
            }
        }
    }

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            MAP[i][j] = tempMap[i][j];
        }
    }
}

/* Check if any cells are alive */
/* TODO: check for stable state */
int liveCheck()
{
    int i, j;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            if (MAP[i][j] == '*') {return 1;}
        }
    }
    return 0;
}

/* Return the terminal to normal functionality */
void cleanup()
{
    nodelay(stdscr, TRUE);
    echo();
    nocbreak();
    endwin();
}

/* Create a random starting generation */
void genRandom()
{
    int i, j;

    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            MAP[i][j] = rand() % 8 == 0 ? '*' : ' ';
        }
    }

    printWorld();
}

/* Enter a pause state */
void pauseSim()
{
    int ch;

    while ((ch = getchar()) != ' ')
    {
        switch (ch)
        {
            case 'd':
                dumpMap();
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
int dumpMap()
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
            fputc(MAP[i][j], dump_file);
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
void goodbyeConway()
{
    int i, j;
    int x_center = WIDTH / 2;
    int y_center = HEIGHT / 2;
    
    for (i = 0; i < HEIGHT; i++)
    {
        for (j = 0; j < WIDTH; j++)
        {
            MAP[i][j] = ' ';
        }
    }
    
    MAP[y_center - 4][x_center - 1]     = '*';
    MAP[y_center - 4][x_center]         = '*';
    MAP[y_center - 4][x_center + 1]     = '*';
    MAP[y_center - 3][x_center - 1]     = '*';
    MAP[y_center - 3][x_center + 1]     = '*';
    MAP[y_center - 2][x_center - 1]     = '*';
    MAP[y_center - 2][x_center + 1]     = '*';
    MAP[y_center - 1][x_center]         = '*';
    MAP[y_center][x_center - 3]         = '*';
    MAP[y_center][x_center - 1]         = '*';
    MAP[y_center][x_center]             = '*';
    MAP[y_center][x_center + 1]         = '*';
    MAP[y_center + 1][x_center - 2]     = '*';
    MAP[y_center + 1][x_center]         = '*';
    MAP[y_center + 1][x_center + 2]     = '*';
    MAP[y_center + 2][x_center]         = '*';
    MAP[y_center + 2][x_center + 3]     = '*';
    MAP[y_center + 3][x_center - 1]     = '*';
    MAP[y_center + 3][x_center + 1]     = '*';
    MAP[y_center + 4][x_center - 1]     = '*';
    MAP[y_center + 4][x_center + 1]     = '*';
    
    printWorld();
    
    mvaddstr(y_center + 6, x_center - 7, "RIP John Conway");
    mvaddstr(y_center + 7, x_center - 13, "26 Dec. 1937 - 11 Apr. 2020");
}
