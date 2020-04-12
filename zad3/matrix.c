#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>

#define DELAY 30000

int max_x, max_y;

pthread_mutex_t muteksik;

void *falling_digit(void *id)
{
    int opcje[3] = {25, 50, 100};
    int digit = rand()%10;
    int x = rand() % max_x, y = 0;
    int jednostki = opcje[rand()%2];
    while (y<=max_y)
    {
        attron(COLOR_PAIR(1));
        pthread_mutex_lock(&muteksik);
        mvprintw(y, x, "%d", digit);
        refresh();
        pthread_mutex_unlock(&muteksik);
        attroff(COLOR_PAIR(1));
        usleep((DELAY*jednostki)/max_y);
        pthread_mutex_lock(&muteksik);
        mvprintw(y++, x, "  ");
        refresh();
        pthread_mutex_unlock(&muteksik);
    }
    return NULL;
}

int main()
{
    srand(time(NULL));
    initscr();
    noecho();
    curs_set(0);
    start_color();

    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    getmaxyx(stdscr, max_y, max_x);

    pthread_mutex_init(&muteksik, NULL);

    pthread_t thread;
    while (1)
    {
        pthread_create(&thread, NULL, falling_digit, NULL);
        usleep(50000);
    }

    pthread_mutex_destroy(&muteksik);

    usleep(5*10000000);
    endwin();
    return 0;
}