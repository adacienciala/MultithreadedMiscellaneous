#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <semaphore.h>

#define MS 1000
#define SIZE 10

#define RED_FG 1
#define GREEN_FG 2
#define YELLOW_FG 3
#define BLUE_FG 4
#define GREEN_BG 5
#define RED_BG 6
#define YELLOW_BG 7
#define BLUE_BG 8

int max_x, max_y, queue, tables[4] = {0}, stats=0;

pthread_mutex_t muteksik, muteksik2;
sem_t semaforek;

char *strrev(char *str)
{
    char *p1, *p2;

    if (! str || ! *str)
        return str;
    for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
};

void *display (void *id)
{
    queue = SIZE;
    while(1)
    {
        pthread_mutex_lock(&muteksik2);
        mvprintw(0, 0, "Students waiting for the exam: %3d", queue);
        mvprintw(max_y*5/8, 0, "ooooo");
        if (queue<5)
        {
            switch (queue)
            {
                case 4:
                    mvprintw(max_y*5/8, 0, " oooo");
                    break;
                case 3:
                    mvprintw(max_y*5/8, 0, "  ooo");
                    break;
                case 2:
                    mvprintw(max_y*5/8, 0, "    oo");
                    break;
                case 1:
                    mvprintw(max_y*5/8, 0, "    o");
                    break;
                case 0:
                    mvprintw(max_y*5/8, 0, "     ");
                    break;
            }
        }
        refresh();
        pthread_mutex_unlock(&muteksik2);
        usleep(15*MS);
    }
}

void *student (void *id)
{
    int delayOpt = rand()%3-1;
    sem_wait(&semaforek);
    pthread_mutex_lock(&muteksik);
    --queue;
    int tableID;
    for (tableID=0; tableID<4; ++tableID)
    {
        if (!tables[tableID]) break;
    }
    tables[tableID] = 1;
    mvprintw(tableID+1, 0, "Student %d is answering questions...", *(int *)id);
    refresh();
    pthread_mutex_unlock(&muteksik);
    switch (tableID)
    {
        case 0:
            //GO TO THE SE TABLE
            for (int i=5; i<(max_x*3/7+max_x/14); ++i)
            {
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*5/8, i, "o");
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*5/8, i, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            for (int i=max_y*5/8; i<max_y*3/4; ++i)
            {
                pthread_mutex_lock(&muteksik);
                mvprintw(i, max_x*3/7+max_x/14, "o");
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(i, max_x*3/7+max_x/14, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            pthread_mutex_lock(&muteksik);
            mvprintw(max_y*3/4-1, max_x*3/7+max_x/14, "o");
            refresh();
            pthread_mutex_unlock(&muteksik);
            break;
        case 1:
            //GO TO THE SW TABLE
            for (int i=5; i<=(max_x*2/7-max_x/14); ++i)
            {
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*5/8, i, "o");
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*5/8, i, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            for (int i=max_y*5/8; i<max_y*3/4; ++i)
            {
                pthread_mutex_lock(&muteksik);
                mvprintw(i, max_x*2/7-max_x/14, "o");
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(i, max_x*2/7-max_x/14, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            pthread_mutex_lock(&muteksik);
            mvprintw(max_y*3/4-1, max_x*2/7-max_x/14, "o");
            refresh();
            pthread_mutex_unlock(&muteksik);
            break;
        case 2:
            //GO TO THE NE TABLE
            for (int i=5; i<(max_x*2/7+max_x/14); ++i)
            {
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*5/8, i, "o");
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*5/8, i, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            for (int i=max_y*5/8; i>=(max_y/2-1); --i)
            {
                pthread_mutex_lock(&muteksik);
                mvprintw(i, max_x*2/7+max_x/14, "o");
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(i, max_x*2/7+max_x/14, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            for (int i=(max_x*2/7+max_x/14); i<(max_x*3/7+max_x/14); ++i)
            {
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y/2-1, i, "o");
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y/2-1, i, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            pthread_mutex_lock(&muteksik);
            mvprintw(max_y/2-1, max_x*3/7+max_x/14, "o");
            refresh();
            pthread_mutex_unlock(&muteksik);
            break;
        case 3:
            //GO TO THE NW TABLE
            for (int i=5; i<(max_x*2/7+max_x/14); ++i)
            {
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*5/8, i, "o");
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*5/8, i, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            for (int i=max_y*5/8; i>=(max_y/2-1); --i)
            {
                pthread_mutex_lock(&muteksik);
                mvprintw(i, max_x*2/7+max_x/14, "o");
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(i, max_x*2/7+max_x/14, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            for (int i=(max_x*2/7+max_x/14); i>=(max_x*2/7-max_x/14); --i)
            {
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y/2-1, i, "o");
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y/2-1, i, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            pthread_mutex_lock(&muteksik);
            mvprintw(max_y/2-1, max_x*2/7-max_x/14, "o");
            refresh();
            pthread_mutex_unlock(&muteksik);
            break;
    }
    usleep(100*MS*(125+delayOpt*25));
    sem_post(&semaforek);
    pthread_mutex_lock(&muteksik);
    int passed = !(rand()%4);
    stats += passed;
    mvprintw(tableID+1, 0, "                                          ");
    attron(COLOR_PAIR(passed+1));
    mvprintw(tableID+5, 0, "Student %d has answered the questions!", *(int *)id);
    attroff(COLOR_PAIR(passed+1));
    refresh();
    tables[tableID] = 0;
    pthread_mutex_unlock(&muteksik);
    switch (tableID)
    {
        case 0:
            //GO OUT FROM THE SE TABLE
            for (int i=0; i<2; ++i)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(passed+1));
                mvprintw(max_y*3/4-1-i, max_x*3/7+max_x/14, "o");
                attroff(COLOR_PAIR(passed+1));
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*3/4-1-i, max_x*3/7+max_x/14, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            for (int i=max_x*3/7+max_x/14; i<max_x; ++i)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(passed+1));
                mvprintw(max_y*3/4-2, i, "o");
                attroff(COLOR_PAIR(passed+1));
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*3/4-2, i, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            break;
        case 1:
            //GO OUT FROM THE SW TABLE
            for (int i=0; i<2; ++i)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(passed+1));
                mvprintw(max_y*3/4-1-i, max_x*2/7-max_x/14, "o");
                attroff(COLOR_PAIR(passed+1));
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*3/4-1-i, max_x*2/7-max_x/14, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            for (int i=max_x*2/7-max_x/14; i<max_x; ++i)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(passed+1));
                mvprintw(max_y*3/4-2, i, "o");
                attroff(COLOR_PAIR(passed+1));
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y*3/4-2, i, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            break;
        case 2:
            //GO OUT FROM THE NE TABLE
            for (int i=0; i<2; ++i)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(passed+1));
                mvprintw(max_y/2-1-i, max_x*3/7+max_x/14, "o");
                attroff(COLOR_PAIR(passed+1));
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y/2-1-i, max_x*3/7+max_x/14, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            for (int i=max_x*3/7+max_x/14; i<max_x; ++i)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(passed+1));
                mvprintw(max_y/2-2, i, "o");
                attroff(COLOR_PAIR(passed+1));
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y/2-2, i, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            break;
        case 3:
            //GO OUT OF THE NW TABLE
            for (int i=0; i<2; ++i)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(passed+1));
                mvprintw(max_y/2-1-i, max_x*2/7-max_x/14, "o");
                attroff(COLOR_PAIR(passed+1));
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y/2-1-i, max_x*2/7-max_x/14, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            for (int i=max_x*2/7-max_x/14; i<max_x; ++i)
            {
                pthread_mutex_lock(&muteksik);
                attron(COLOR_PAIR(passed+1));
                mvprintw(max_y/2-2, i, "o");
                attroff(COLOR_PAIR(passed+1));
                refresh();
                pthread_mutex_unlock(&muteksik);
                usleep(MS*100);
                pthread_mutex_lock(&muteksik);
                mvprintw(max_y/2-2, i, " ");
                refresh();
                pthread_mutex_unlock(&muteksik);
            }
            break;
    }
    return NULL;
}

void drawEndcard (void)
{
    char dr_duch[12][30]= {
            "            ##(#/            ",
            "          #(,*/*,*#((        ",
            "         @,,*****,*@&        ",
            "         %/%@##&@&*#&        ",
            "         (/#%/,%&%(/&*       ",
            "         /*/##%%#(/(/        ",
            "          /##%&@&(#(         ",
            "           ((##((#(          ",
            "           (###%%%(          ",
            "        ..*/((#%##(/*,..     ",
            "   ,,,/..,*(((#(##(*,,,,,..  ",
            "*((/,/*,,*//(((((/*,//*/*,,,.",
    };
    char dr_jaworski[12][32] = {
            "           (**//((##           ",
            "         (%#*/////**/#         ",
            "         #&(//*//**//%         ",
            "         %@((%@%@&@&/%         ",
            "         (((/(((/(((/(         ",
            "           #((%%@@&%(          ",
            "           (#%#/@@#@           ",
            "           /#&@&@@@%           ",
            "        &@@/%(#%&@%#/*         ",
            "   %&@&@&@@*&&((%#%*%@@@@@%    ",
            " @@@@@@@@@@@@@&(((&*%@@@@@@@@@ ",
            "@@@@@@@@@@@@@@&(((&*%@@@@@@@@@@"
    };
    while (1)
    {
        clear();
        //STATISTICS
        mvprintw(max_y/5-2, max_x/2-5, "STATISTICS:");
        attron(COLOR_PAIR(GREEN_FG));
        mvprintw(max_y*4/5+2, 15, "PASSED:   %d", stats);
        attroff(COLOR_PAIR(GREEN_FG));
        attron(COLOR_PAIR(RED_FG));
        mvprintw(max_y*4/5+2, max_x-26, "FAILED:   %d", SIZE-stats);
        attroff(COLOR_PAIR(RED_FG));
        //DANCING PROFFS
        for (int i=0; i<max_x; ++i)
        {
            mvprintw(max_y/5, i, "*");
            mvprintw(max_y*4/5, i, "*");
        }
        for (int i=0; i<12; ++i)
        {
            mvprintw(max_y/5+2+i, 5, "%s", *(dr_jaworski+i));
        }
        for (int i=0; i<12; ++i)
        {
            mvprintw(max_y/5+2+i, max_x-35, "%s", *(dr_duch+i));
        }
        refresh();
        usleep(MS*400);
        clear();
        for (int i=0; i<max_x; ++i)
        {
            mvprintw(max_y/5, i, "*");
            mvprintw(max_y*4/5, i, "*");
        }
        refresh();
        for (int i=0; i<12; ++i)
        {
            strrev(*(dr_jaworski+i));
            strrev(dr_duch[i]);
        }
        for (int i=0; i<12; ++i)
        {
            mvprintw(max_y/5+2+i, 5, "%s", *(dr_jaworski+i));
        }
        for (int i=0; i<12; ++i)
        {
            mvprintw(max_y/5+2+i, max_x-35, "%s", *(dr_duch+i));
        }
        refresh();
    }
}

void drawClass (void)
{
    int a=0, b=-1, c, d;
    for (int i=0; i<4; ++i)
    {
        if (!(i%2)) {a+=2; b+=2; c=2; d=1;}
        else {c=4; d=3;}
        pthread_mutex_lock(&muteksik);
        for (int j=max_x*b/7; j<max_x*a/7; ++j)
        {
            mvprintw(max_y/c*d, j, "_");
        }
        mvprintw(max_y/c*d+1, max_x/7*b, "|");
        mvprintw(max_y/c*d+1, max_x/7*(b+1), "|");
        refresh();
        pthread_mutex_unlock(&muteksik);
    }
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
    pthread_mutex_init(&muteksik2, NULL);
    sem_init(&semaforek, 0, 4);

    pthread_t students[SIZE], queue;
    int tab[SIZE];

    drawClass();

    pthread_create(&queue, NULL, display, NULL);
    usleep(MS*100);

    for(int i=0; i<SIZE; ++i)
    {
        tab[i] = i;
        pthread_create(students+i, NULL, student, tab+i);
        usleep(MS*500);
    }

    for(int i=0; i<SIZE; ++i)
    {
        pthread_join(students[i], NULL);
    }
    pthread_cancel(queue);

    drawEndcard();

    pthread_mutex_destroy(&muteksik);
    pthread_mutex_destroy(&muteksik2);
    sem_destroy(&semaforek);

    getchar();
    endwin();
    return 0;
}