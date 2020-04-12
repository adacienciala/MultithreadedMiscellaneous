#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>

#define MS 1000

#define GREEN_FG 1
#define RED_FG 2
#define YELLOW_FG 3
#define BLUE_FG 4
#define GREEN_BG 5
#define RED_BG 6
#define YELLOW_BG 7
#define BLUE_BG 8

int max_x, max_y, speed;

struct player {
    int x;
    int y;
    int alive;
    int score;
    int shield;
} player;

pthread_mutex_t muteksik;

//gwiazdki lecace na ukos i odbijajace sie od scianek
//suchary generowane na dole
//zaliczone punktowo migaja na zielono na chwile

void *move_player(void *id)
{
    player.alive = 1;
    player.score = 0;
    player.shield = 0;
    player.x = 10;
    player.y = max_y/2;

    while (player.alive)
    {
        pthread_mutex_lock(&muteksik);
        attron(COLOR_PAIR(YELLOW_FG));
        mvprintw(player.y, player.x, ">");
        if(player.shield) mvprintw(player.y, player.x+1, ")");
        refresh();
        attroff(COLOR_PAIR(YELLOW_FG));
        pthread_mutex_unlock(&muteksik);
        switch(getch())
        {
            case 'W':
            case 'w':
            case KEY_UP:
                if ((player.y-1)>(max_y/3))
                {
                    pthread_mutex_lock(&muteksik);
                    mvprintw(player.y, player.x, " ");
                    if(player.shield) mvprintw(player.y, player.x+1, " ");
                    refresh();
                    pthread_mutex_unlock(&muteksik);
                    player.y -= 1;
                }
                break;
            case 'S':
            case 's':
            case KEY_DOWN:
                if ((player.y+1)<(max_y/3*2))
                {
                    pthread_mutex_lock(&muteksik);
                    mvprintw(player.y, player.x, " ");
                    if(player.shield) mvprintw(player.y, player.x+1, " ");
                    refresh();
                    pthread_mutex_unlock(&muteksik);
                    player.y += 1;
                }
                break;
            case 'A':
            case 'a':
            case KEY_LEFT:
                if ((player.x-1)>=0)
                {
                    pthread_mutex_lock(&muteksik);
                    mvprintw(player.y, player.x, " ");
                    if(player.shield) mvprintw(player.y, player.x+1, " ");
                    refresh();
                    pthread_mutex_unlock(&muteksik);
                    player.x -= 1;
                }
                break;
            case 'D':
            case 'd':
            case KEY_RIGHT:
                if ((player.x+1)<max_x)
                {
                    pthread_mutex_lock(&muteksik);
                    mvprintw(player.y, player.x, " ");
                    if(player.shield) mvprintw(player.y, player.x+1, " ");
                    refresh();
                    pthread_mutex_unlock(&muteksik);
                    player.x += 1;
                }
        }
    }
    return NULL;
}

void *move_enemy(void *you)
{
    int x = max_x-1;
    int y = rand() % (max_y/3-1) + (max_y/3+1);
    int scored = 0;

    while (x>=0 && player.alive)
    {
        pthread_mutex_lock(&muteksik);
        if (scored) attron(COLOR_PAIR(GREEN_FG));
        else attron(COLOR_PAIR(BLUE_FG));
        mvprintw(y, x, "<");
        refresh();
        if (scored) attroff(COLOR_PAIR(GREEN_FG));
        else attroff(COLOR_PAIR(BLUE_FG));
        if (scored) scored--;
        if ((x-2)==player.x && y==player.y && player.shield)
        {
            player.score++;
            if (!(player.score % 5) && speed>=20) speed -= 10;
            mvprintw(y, x, " ");
            refresh();
            pthread_mutex_unlock(&muteksik);
            return NULL;
        }
        if (x==player.x)
        {
            if (y==player.y)
            {
                player.alive = 0;
                pthread_cancel(*(pthread_t *)you);
                pthread_mutex_unlock(&muteksik);
                return NULL;
            }
            else
            {
                player.score++;
                scored = 6;
                if (!(player.score % 5) && speed>=20) speed -= 10;
            }
        }
        pthread_mutex_unlock(&muteksik);
        usleep(speed*MS);
        pthread_mutex_lock(&muteksik);
        mvprintw(y, x, " ");
        refresh();
        pthread_mutex_unlock(&muteksik);
        --x;
    }
    return NULL;
}

void *shield_timer (void *id)
{
    player.shield = 1;
    usleep(1000*MS*10);
    player.shield = 0;
    mvprintw(player.y, player.x+1, " ");
    refresh();
    return NULL;
}

void *move_shield(void *timer)
{
    int x = max_x-1;
    int y = rand() % (max_y/3-1) + (max_y/3+1);

    while (x>=0 && player.alive)
    {
        pthread_mutex_lock(&muteksik);
        attron(COLOR_PAIR(GREEN_FG));
        mvprintw(y, x, "0");
        refresh();
        attroff(COLOR_PAIR(GREEN_FG));
        if (x-1==player.x && y==player.y)
        {
            attron(COLOR_PAIR(YELLOW_FG));
            mvprintw(y, x, ")");
            refresh();
            attroff(COLOR_PAIR(YELLOW_FG));
            if (player.shield) pthread_cancel(*(pthread_t *)timer);
            pthread_create((pthread_t *)timer, NULL, shield_timer, NULL);
            pthread_mutex_unlock(&muteksik);
            return NULL;
        }
        pthread_mutex_unlock(&muteksik);
        usleep(speed*MS);
        pthread_mutex_lock(&muteksik);
        mvprintw(y, x, " ");
        refresh();
        pthread_mutex_unlock(&muteksik);
        --x;
    }
    return NULL;
}

void draw_board (void)
{
    clear();
    for (int i=0; i<max_x; ++i)
    {
        pthread_mutex_lock(&muteksik);
        mvprintw(max_y/3, i, "*");
        mvprintw(max_y/3*2, i, "*");
        pthread_mutex_unlock(&muteksik);
    }
    refresh();
}

void *print_score (void *id)
{
    while(player.alive)
    {
        pthread_mutex_lock(&muteksik);
        mvprintw(0, 0, "SCORE:");
        mvprintw(0, 7, "%d", player.score);
        mvprintw(1, 0, "SPEED:");
        mvprintw(1, 7, "%d", speed);
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
    keypad(stdscr, TRUE);

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

    pthread_t enemy, you, scoreboard, shield, timer;

    char c;
    do
    {
        draw_board();
        player.alive = 1;
        speed = 40;

        pthread_create(&you, NULL, move_player, NULL);
        pthread_create(&scoreboard, NULL, print_score, NULL);

        while (pthread_tryjoin_np(you, NULL)) {
            int delay = rand()%3-1;
            usleep(((15*MS*100)+(15*MS*(delay*50)))*speed/40);
            if (rand()%100>80) pthread_create(&shield, NULL, move_shield, &timer);
            else pthread_create(&enemy, NULL, move_enemy, &you);
        }
        draw_board();

        pthread_mutex_lock(&muteksik);
        if (player.score/100) mvprintw(max_y / 3 - 1 , max_x / 2 - 6, "SCORE: %d", player.score);
        else if (player.score/10) mvprintw(max_y / 3 - 1 , max_x / 2 - 5, "SCORE:  %d", player.score);
        else mvprintw(max_y / 3 - 1 , max_x / 2 - 4, "SCORE: %d", player.score);
        refresh();
        attron(COLOR_PAIR(RED_FG));
        mvprintw(max_y / 2 - 2, max_x / 2 - 4, "YOU DIED");
        refresh();
        usleep(MS * 1500);
        mvprintw(max_y / 2 - 1, max_x / 2 - 4, "PATHETIC.");
        refresh();
        usleep(MS * 1500);
        mvprintw(max_y / 2 + 1, max_x / 2 - 10, "...WANNA TRY AGAIN?");
        mvprintw(max_y / 2 + 2, max_x / 2 - 1, "Y/N");
        refresh();
        attroff(COLOR_PAIR(RED_FG));
        pthread_mutex_unlock(&muteksik);
        while ((c = getchar()) != 'y' && c != 'n');
    } while (c=='y');

    draw_board();
    attron(COLOR_PAIR(RED_FG));
    mvprintw(max_y / 2, max_x / 2 - 4, "WHATEV.");
    attroff(COLOR_PAIR(RED_FG));
    refresh();

    pthread_mutex_destroy(&muteksik);

    usleep(5000*MS);
    endwin();
    return 0;
}