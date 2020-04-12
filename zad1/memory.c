#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>

#define MS 1000

#define GREEN_FG 1
#define RED_FG 2
#define YELLOW_FG 3
#define BLUE_FG 4
#define GREEN_BG 5
#define RED_BG 6
#define YELLOW_BG 7
#define BLUE_BG 8

int max_x, max_y, won=0;
pthread_mutex_t muteksik;
pthread_t amPrinting, amReading, amKilling;
char seq[11] = {0};

void *timer(void *id)
{
    usleep(MS*5000);
    pthread_cancel(amPrinting);
    pthread_cancel(amReading);
    return NULL;
}

void *inputReader (void *word)
{
    int len = strlen(seq);
    char a;

    while ((a=getch()) != ERR) ;
    pthread_mutex_lock(&muteksik);
    mvprintw(0, 0, "YOUR TURN ;)");
    refresh();
    pthread_mutex_unlock(&muteksik);
    for (int i=0; len--; ++i)
    {
        while ((a=getch()) == ERR) ;
        if (a!=*((char *)word+i))
        {
            pthread_cancel(amPrinting);
            return NULL;
        }
    }
    pthread_cancel(amKilling);
    pthread_mutex_lock(&muteksik);
    mvprintw(0, 0, "            ");
    refresh();
    pthread_mutex_unlock(&muteksik);
    return NULL;
}

void *display (void *id)
{
    char *letters = "wsad";

    for (int i=0; i<10; ++i)
    {
        strncat(seq, letters+rand()%4, 1);
        pthread_mutex_lock(&muteksik);
        mvprintw(0, max_x-9, "RUNDA %2d", i+1);
        refresh();
        pthread_mutex_unlock(&muteksik);
        for (int j=0; j<strlen(seq); ++j)
        {
            pthread_mutex_lock(&muteksik);
            mvprintw(0, 0, "%c", seq[j]);
            refresh();
            usleep(MS*500);
            mvprintw(0, 0, " ");
            refresh();
            usleep(MS*500);
            pthread_mutex_unlock(&muteksik);
        }
        pthread_create(&amReading, NULL, inputReader, seq);
        pthread_create(&amKilling, NULL, timer, NULL);
        pthread_join(amReading, NULL);
    }
    won = 1;
    return NULL;
}

int main()
{
    srand(time(NULL));
    initscr();
    noecho();
    curs_set(0);
    start_color();
    timeout (1);

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

    pthread_create(&amPrinting, NULL, display, NULL);
    pthread_join(amPrinting, NULL);

    pthread_mutex_lock(&muteksik);
    if (!won)
    {
        mvprintw(0, 0, "YOU LOST.");
        refresh();
    }
    else
    {
        mvprintw(0, 0, "YOU WON ;)");
        refresh();
    }
    pthread_mutex_unlock(&muteksik);

    usleep(5*10000000);
    endwin();
    return 0;
}