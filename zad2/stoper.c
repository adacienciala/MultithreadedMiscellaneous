#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <ncurses.h>

enum tryby { wprowadzanie, odliczanie, pauza, finished } trybik;
int czas=0;

void *wprowadzamy (void *id)
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
                if (trybik!=odliczanie) czas += 60;
                break;
            case 'b':
                if (trybik!=odliczanie)
                {
                    if (czas<60) czas = 0;
                    else czas -= 60;
                }
                break;
            case 'j':
                if (trybik!=odliczanie) czas += 1;
                break;
            case 'n':
                if (trybik!=odliczanie)
                {
                    if (czas<1) czas = 0;
                    else czas -= 1;
                }
                break;
            case 'p':
                if (trybik==pauza) trybik=odliczanie;
                else if (odliczanie == trybik) trybik = pauza;
                break;
            case 's':
                if (trybik == wprowadzanie) trybik = odliczanie;
                break;
            case 'r':
                if (trybik==pauza || trybik == wprowadzanie || trybik == finished)
                {
                    clear();
                    refresh();
                    trybik = wprowadzanie;
                    czas = 0;
                }
                break;
            default:
                continue;
        }
    }
}

void *odliczamy (void *id)
{
    int flaga = 1, licz=1;
    while (1)
    {
        if (trybik == odliczanie)
        {
            mvprintw(0, 0, "%02d%c%02d", czas/60, flaga * ':' + !flaga * ' ', czas%60);
            refresh();
            usleep(20000);
            if (czas>0)
            {
                if (!(licz%25))czas -= flaga;
                if (!(licz%25)) flaga =! flaga;
                licz++;
            }
            else
            {
                for (int i=0; i<3; ++i)
                {
                    mvprintw(0, 0, "%02d:%02d", czas/60, czas%60);
                    refresh();
                    usleep(500000);
                    mvprintw(0, 0, "         ", czas/60, czas%60);
                    refresh();
                    usleep(500000);
                }
                usleep(250000);
                clear();
                mvprintw(0, 0, "TIME'S UP!\n");
                refresh();
                usleep(500000);
                mvprintw(1, 0, "to restart, click 'r'");
                refresh();
                licz = 0;
                trybik = finished;
            }
        }
        else
        {
            if (trybik != finished)
            {
                mvprintw(0, 0, "%02d:%02d", czas/60, czas%60);
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

    trybik = wprowadzanie;
    pthread_create(thread_c, NULL, odliczamy, NULL);
    pthread_create(thread_c+1, NULL, wprowadzamy, NULL);
    pthread_join(thread_c[1], NULL);

    endwin();
    return 0;
}
