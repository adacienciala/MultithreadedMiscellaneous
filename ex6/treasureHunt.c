#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>

#define MS 1000
#define MAX_SCORE 5

#define GREEN_FG 1
#define RED_FG 2
#define YELLOW_FG 3
#define BLUE_FG 4
#define GREEN_BG 5
#define RED_BG 6
#define YELLOW_BG 7
#define BLUE_BG 8
#define TP 4
#define ZIUM 1
#define OJOJ 2
#define OHMY 3


enum types { WALL, NERF, EMPTY, BOOST, TELEPORT, TREASURE};
int max_x, max_y;
int **board;
int winner=0;
struct treasure {
    int x;
    int y;
    int found;
} treasure;

pthread_mutex_t muteksik;

void generateBoard (int x, int y)
{
    board = (int **)calloc(y-5, sizeof(int *));
    int probability;
    for (int i = 0; i <  y-5; ++i)
    {
        *(board+i) = (int *)calloc(x, sizeof(int));
    }
    for (int i = 0; i < y-5; ++i)
    {
        for (int j = 0; j < x; ++j)
        {
            if (!i || i ==(y-6) || !j || j==(x-1)) 
                *(*(board+i)+j) = WALL;
            else
            {
                probability = rand()%100;
                if (probability < 1) 
                    *(*(board+i)+j) = NERF;
                else if (probability < 2) 
                    *(*(board+i)+j) = BOOST;
                else if (probability < 3) 
                    *(*(board+i)+j) = TELEPORT;
                else 
                    *(*(board+i)+j) = EMPTY;
            }
        }
    }
    for (int i = 0; i < y-5; ++i)
    {
        for (int j = 0; j < x; ++j)
        {
            if (*(*(board+i)+j) == NERF)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(OJOJ));
                mvprintw(i, j, "*");
                refresh();
                attron(COLOR_PAIR(OJOJ));
                pthread_mutex_unlock(&muteksik);
            }
            else if (*(*(board+i)+j) == BOOST)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(ZIUM));
                mvprintw(i, j, "*");
                refresh();
                attron(COLOR_PAIR(ZIUM));
                pthread_mutex_unlock(&muteksik);
            }
            else if (*(*(board+i)+j) == TELEPORT)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(TP));
                mvprintw(i, j, "*");
                refresh();
                attron(COLOR_PAIR(TP));
                pthread_mutex_unlock(&muteksik);
            }
        }
    }
    pthread_mutex_lock(&muteksik);
    attron(COLOR_PAIR(TP));
    mvprintw(y-4, x-24, "* - TELEPORT");
    attron(COLOR_PAIR(TP));
    attron(COLOR_PAIR(ZIUM));
    mvprintw(y-3, x-24, "* - BOOST");
    attron(COLOR_PAIR(ZIUM));
    attron(COLOR_PAIR(OJOJ));
    mvprintw(y-2, x-24, "* - NERF");
    attron(COLOR_PAIR(OJOJ));
    attron(COLOR_PAIR(OHMY));
    mvprintw(y-1, x-24, "o - TREASURE");
    attron(COLOR_PAIR(OHMY));
    refresh();
    pthread_mutex_unlock(&muteksik);
}

void clearBoard(int x, int y)
{
    for (int i = 1; i < (y-6); ++i)
    {
        for (int j = 1; j < (x-1); ++j)
        {
            pthread_mutex_lock(&muteksik);
            mvprintw(i, j, " ");
            pthread_mutex_unlock(&muteksik);
        }
    }
    refresh();
}

void *drawTreasure (void *id)
{
    int rand_x, rand_y;
    while (1)
    {
        rand_x = rand()%(max_x-2)+1;
        rand_y = rand()%(max_y-5-2)+1;
        *(*(board+rand_y)+rand_x) = TREASURE;
        treasure.x = rand_x;
        treasure.y = rand_y;
        treasure.found = 0;
        pthread_mutex_lock(&muteksik);
        attron(COLOR_PAIR(OHMY));
        mvprintw(treasure.y, treasure.x, "o");
        attron(COLOR_PAIR(OHMY));
        refresh();
        pthread_mutex_unlock(&muteksik);
        while (!treasure.found)
        {
            usleep(15*MS);
        }
        if (winner) break;
    }
    return NULL;
}

void *movePlayer (void *id)
{
    int x = rand()%(max_x-2)+1;
    int y = rand()%(max_y-5-2)+1;
    double surprise=1;
    int score=0, licznik=0;
    pthread_mutex_lock(&muteksik);
    attron(COLOR_PAIR(*(int *)id));
    mvprintw(max_y-(5-*(int *)id), 10, "%d", score);
    attroff(COLOR_PAIR(*(int *)id));
    refresh();
    pthread_mutex_unlock(&muteksik);
    while (1)
    {
        pthread_mutex_lock(&muteksik);
        attron(COLOR_PAIR(*(int *)id));
        mvprintw(y, x, "x");
        attroff(COLOR_PAIR(*(int *)id));
        refresh();
        pthread_mutex_unlock(&muteksik);
        if (*(*(board+y)+x) == TELEPORT)
        {
            pthread_mutex_lock(&muteksik);
            *(*(board+y)+x) = EMPTY;
            attron(COLOR_PAIR(*(int *)id));
            mvprintw(max_y-(5-*(int *)id), 15, "                        ");
            mvprintw(max_y-(5-*(int *)id), 15, "PLAYER %d TELEPORTED", *(int *)id);
            attroff(COLOR_PAIR(*(int *)id));
            pthread_mutex_unlock(&muteksik);

            for (int i = 0; i < 3; ++i)
            {
                //BLINK OUT
                mvprintw(y, x, " ");
                refresh();
                usleep(500*MS);
                //BLINK IN
                attron(COLOR_PAIR(*(int *)id));
                mvprintw(y, x, "x");
                refresh();
                attroff(COLOR_PAIR(*(int *)id));
                usleep(500*MS);
            }
            //OUT
            mvprintw(y, x, " ");
            refresh();

            x = rand()%(max_x-2)+1;
            y = rand()%(max_y-5-2)+1;
        }
        else
        {
            switch (*(*(board+y)+x))
            {
                case BOOST:
                    surprise = 0.5;
                    pthread_mutex_lock(&muteksik);
                    mvprintw(max_y-(5-*(int *)id), 15, "                        ");
                    attron(COLOR_PAIR(*(int *)id));
                    mvprintw(max_y-(5-*(int *)id), 15, "PLAYER %d BOOSTED", *(int *)id);
                    attroff(COLOR_PAIR(*(int *)id));
                    refresh();
                    pthread_mutex_unlock(&muteksik);
                    break;
                case NERF:
                    surprise = 2;
                    pthread_mutex_lock(&muteksik);
                    mvprintw(max_y-(5-*(int *)id), 15, "                        ");
                    attron(COLOR_PAIR(*(int *)id));
                    mvprintw(max_y-(5-*(int *)id), 15, "PLAYER %d NERFED", *(int *)id);
                    attroff(COLOR_PAIR(*(int *)id));
                    refresh();
                    pthread_mutex_unlock(&muteksik);
                    break;
                case TREASURE:
                    score += 1;
                    treasure.found = 1;
                    attron(COLOR_PAIR(*(int *)id));
                    pthread_mutex_lock(&muteksik);
                    mvprintw(max_y-(5-*(int *)id), 10, "%d", score);
                    mvprintw(max_y-(5-*(int *)id), 15, "                        ");
                    mvprintw(max_y-(5-*(int *)id), 15, "PLAYER %d SCORED", *(int *)id);
                    refresh();
                    pthread_mutex_unlock(&muteksik);
                    attroff(COLOR_PAIR(*(int *)id));
                    if (score == MAX_SCORE)
                    {
                        winner = 1;
                        attron(COLOR_PAIR(*(int *)id));
                        pthread_mutex_lock(&muteksik);
                        mvprintw(max_y-4, 15, "                         ");
                        mvprintw(max_y-3, 15, "                         ");
                        mvprintw(max_y-2, 15, "                         ");
                        mvprintw(max_y-1, 15, "                         ");
                        mvprintw(max_y-(5-*(int *)id), 15, "PLAYER %d HAS WON", *(int *)id);
                        refresh();
                        pthread_mutex_unlock(&muteksik);
                        attroff(COLOR_PAIR(*(int *)id));
                        return NULL;
                    }
            }
            if (surprise!=1 && !licznik) licznik = 6;
            else if (licznik)
            {
                licznik--;
                if (!licznik) surprise = 1;
            }
            *(*(board+y)+x) = EMPTY;
            usleep(200*MS*surprise);
            pthread_mutex_lock(&muteksik);
            mvprintw(y, x, " ");
            refresh();
            pthread_mutex_unlock(&muteksik);
            if (x < treasure.x) ++x;
            else if (x > treasure.x) --x;
            else
            {
                if (y < treasure.y) ++y;
                else if (y > treasure.y) --y;
            }
        }
    }
}

void freeBoard (int x, int y)
{
    for (int i = 0; i < y-5; ++i)
    {
        free(*(board+i));
    }
    free(board);
}

int main()
{
    srand(time(NULL));
    initscr();
    noecho();
    curs_set(0);
    start_color();

    init_color(COLOR_BLUE, 100, 400, 700);
    init_color(COLOR_YELLOW, 1000, 1000, 0);

    init_pair(GREEN_FG, COLOR_GREEN, COLOR_BLACK);
    init_pair(RED_FG, COLOR_RED, COLOR_BLACK);
    init_pair(YELLOW_FG, COLOR_YELLOW, COLOR_BLACK);
    init_pair(BLUE_FG, COLOR_BLUE, COLOR_BLACK);
    init_pair(BLUE_BG, COLOR_WHITE, COLOR_BLUE);
    init_pair(GREEN_BG, COLOR_WHITE, COLOR_GREEN);
    init_pair(RED_BG, COLOR_WHITE, COLOR_RED);
    init_pair(YELLOW_BG, COLOR_WHITE, COLOR_RED);

    getmaxyx(stdscr, max_y, max_x);

    pthread_mutex_init(&muteksik, NULL);

    pthread_t player[4], precious;
    int tab[4];

    //SCORE BOARD
    for (int i = 4, j = 1; i != 0; --i, ++j)
    {
        pthread_mutex_lock(&muteksik);
        attron(COLOR_PAIR(j));
        mvprintw(max_y-i, 0, "Player %d:", j);
        attroff(COLOR_PAIR(j));
        pthread_mutex_unlock(&muteksik);
    }
    //GAME BOX
    for (int i = 0; i < max_x; ++i)
    {
        pthread_mutex_lock(&muteksik);
        mvprintw(0, i, "*");
        mvprintw(max_y-6, i, "*");
        pthread_mutex_unlock(&muteksik);
    }
    for (int i = 1; i < max_y-6; ++i)
    {
        pthread_mutex_lock(&muteksik);
        mvprintw(i, 0, "*");
        mvprintw(i, max_x-1, "*");
        pthread_mutex_unlock(&muteksik);
    }
    refresh();

    generateBoard(max_x, max_y);

    pthread_create(&precious, NULL, drawTreasure, NULL);

    for(int i = 0; i < 4; ++i)
    {
        tab[i] = i+1;
        pthread_create(player+i, NULL, movePlayer, (void *)(tab+i));
    }

    pthread_join(precious, NULL);
    for(int i = 0; i < 4; ++i)
    {
        pthread_cancel(player[i]);
    }

    clearBoard(max_x, max_y);

    attron(COLOR_PAIR(YELLOW_FG));
    mvprintw((max_y-7)/2-1, max_x/2-9, "THE HUNT IS OVER!");
    refresh();
    usleep(2000*MS);
    mvprintw((max_y-7)/2+1, max_x/2-18, "CHECK THE SCOREBOARD FOR THE WINNER");
    refresh();
    usleep(2000*MS);
    mvprintw((max_y-7)/2+2, max_x/2-11, "AIN'T DOIN' IT FOR YA");
    refresh();
    usleep(3000*MS);
    mvprintw((max_y-7)/2+4, max_x/2-6, "YA LAZY BI-");
    refresh();
    usleep(50*MS);
    mvprintw((max_y-7)/2+4, max_x/2-6, "            ");
    refresh();
    attroff(COLOR_PAIR(YELLOW_FG));

    pthread_mutex_destroy(&muteksik);

    freeBoard(max_x, max_y);
    usleep(5*10000000);
    endwin();
    return 0;
}