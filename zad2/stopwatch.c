#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>

enum modes_t { inputing, counting, paused, finished } gameMode;
int timer=0;

void *amInputing (void *id)
{
    char key;
    while (1)
    {
        key = getchar();
        switch(key)
        {
            case 'e':
                return NULL;
            case 'h':
                if (gameMode!=counting) timer += 60;
                break;
            case 'b':
                if (gameMode!=counting)
                {
                    if (timer<60) timer = 0;
                    else timer -= 60;
                }
                break;
            case 'j':
                if (gameMode!=counting) timer += 1;
                break;
            case 'n':
                if (gameMode!=counting)
                {
                    if (timer<1) timer = 0;
                    else timer -= 1;
                }
                break;
            case 'p':
                if (gameMode==paused) gameMode=counting;
                else if (counting == gameMode) gameMode = paused;
                break;
            case 's':
                if (gameMode == inputing) gameMode = counting;
                break;
            case 'r':
                if (gameMode==paused || gameMode == inputing || gameMode == finished)
                {
                    clear();
                    refresh();
                    gameMode = inputing;
                    timer = 0;
                }
                break;
            default:
                continue;
        }
    }
}

void *amCounting (void *id)
{
    int flag = 1, counter=1;
    while (1)
    {
        if (gameMode == counting)
        {
            mvprintw(0, 0, "%02d%c%02d", timer/60, flag * ':' + !flag * ' ', timer%60);
            refresh();
            usleep(20000);
            if (timer>0)
            {
                if (!(counter%25))timer -= flag;
                if (!(counter%25)) flag =! flag;
                counter++;
            }
            else
            {
                for (int i=0; i<3; ++i)
                {
                    mvprintw(0, 0, "%02d:%02d", timer/60, timer%60);
                    refresh();
                    usleep(500000);
                    mvprintw(0, 0, "         ", timer/60, timer%60);
                    refresh();
                    usleep(500000);
                }
                usleep(250000);
                clear();
                mvprintw(0, 0, "TIME'S UP!\n");
                refresh();
                usleep(500000);
                mvprintw(1, 0, "to restart, press 'r'");
                refresh();
                counter = 0;
                gameMode = finished;
            }
        }
        else
        {
            if (gameMode != finished)
            {
                mvprintw(0, 0, "%02d:%02d", timer/60, timer%60);
                refresh();
            }
        }
    }
}

int main(void)
{
    initscr();
    noecho();
    curs_set(0);

    pthread_t thread_c[2];

    gameMode = inputing;
    pthread_create(thread_c, NULL, amCounting, NULL);
    pthread_create(thread_c+1, NULL, amInputing, NULL);
    pthread_join(thread_c[1], NULL);

    endwin();
    return 0;
}
