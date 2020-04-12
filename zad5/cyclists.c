#include <stdio.h>
#include <pthread.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DELAY 60000

int max_x, max_y;
int finished=0, unfinished=0;
int dont[10] = {0};
clock_t start, end;

char *names[40] = {"Bakalarski", "Rzepecki", "Wolak", "Wachala", "Oniszczuk", "Wojda", "Glinski", "Rymarczyk", "Soszynski", "Paluchowski", "Golonka", "Kurowski", "Frydrych", "Grzes", "Patora", "Cierpisz", "Jakobczyk", "Wegiel", "Bialon", "Biernat", "Duleba", "Gomulka", "Juszczak", "Michalowicz", "Kus", "Walega", "Zapart", "Taraszkiewicz", "Kasperowicz", "Rzepkowski", "Golebiowski", "Owczarski", "Kopacki", "Gmerek", "Bil", "Debek", "Kuzniewski", "Antosik", "Strojek", "Czaplinski"

};

char *accidents_oops[19] = { "cyclist was ginger", "cyclist has left the chat", "cyclist has a sick daughter", "cyclist fell down", "cyclist changed his mind", "cyclist decided to be a poet", "cyclist died", "cyclist thought through his life choices", "cyclist doesn't love you anymore", "cyclist got lost", "cyclist went home", "cyclist's mom called him for dinner", "cyclist's left his iron on", "cyclist was killed by a murderer", "cyclist got bored", "cyclist lost his will to live", "cyclist won a lottery", "cyclist offended the referee", "cyclist is a dumbass"};

pthread_mutex_t muteksik;

void shuffle(void)
{
    char *temp;
    for (int i=0; i<40; ++i)
    {
        int id = rand()%40;
        temp = names[i];
        names[i] = names[id];
        names[id] = temp;
    }
}

void *sad_accidents (void *tab)
{
    int looser_id;
    while (finished+unfinished<10)
    {
        looser_id = rand()%10;
        if (!dont[looser_id] && (rand()%2000==looser_id))
        {
            if (!pthread_cancel(((pthread_t *)tab)[looser_id]))
            {
                pthread_mutex_lock(&muteksik);
                dont[looser_id] = 1;
                for (int i=0; i<(max_x/4*3); ++i)
                {
                    mvprintw(looser_id, i, " ");
                }
                mvprintw(looser_id, (max_x/4*3)+2, "------");
                mvprintw(looser_id, 2, "~~~%s~~~", accidents_oops[rand()%19]);
                refresh();
                pthread_mutex_unlock(&muteksik);
                unfinished++;
            }
        }
        usleep(2000);
    }
}

void *move_cyclist (void *id)
{
    int x=0, y=*(int *)id;
    int speed[3] = {150, 175, 200};
    float boost[5] = {0.75, 1, 1, 1, 1.25};
    int std_speed = speed[rand()%3]/max_x;

    start = clock();
    while(x<(max_x/4*3))
    {
        pthread_mutex_lock(&muteksik);
        mvprintw(y, x, "o");
        refresh();
        if (!(x%3)) mvprintw(y, max_x/4*3+3+(x%3), "   ");
        mvprintw(y, max_x/4*3+3+(x%3), ".");
        refresh();
        pthread_mutex_unlock(&muteksik);
        usleep(DELAY*(std_speed*boost[rand()%5]));
        pthread_mutex_lock(&muteksik);
        mvprintw(y, x++, " ");
        refresh();
        pthread_mutex_unlock(&muteksik);
    }
    end = clock();

    pthread_mutex_lock(&muteksik);
    dont[y] = 1;
    mvprintw(y, x+2, "%.4lf", (double)(end-start)/CLOCKS_PER_SEC);
    mvprintw(13+finished, 0, "%2d. %s: %.4lf", finished+1, names[finished], (double)(end-start)/CLOCKS_PER_SEC);
    refresh();
    finished += 1;
    pthread_mutex_unlock(&muteksik);

    return NULL;
}

int main()
{
    initscr();
    noecho();
    curs_set(0);

    srand(time(NULL));

    getmaxyx(stdscr, max_y, max_x);

    shuffle();

    pthread_mutex_init(&muteksik, NULL);

    for(int i=0; i<10; ++i)
    {
        mvprintw(i, max_x/4*3, "|");
    }
    for(int i=0; i<(max_x/4*3); ++i)
    {
        mvprintw(10, i, "~");
    }
    refresh();

    pthread_t threads[10], accident_thread;
    int tab[10];

    for(int i=0; i<10; ++i)
    {
        tab[i] = i;
        pthread_create(threads+i, NULL, move_cyclist, tab+i);
    }
    pthread_create(&accident_thread, NULL, sad_accidents, threads);

    for(int i=0; i<10; ++i)
    {
        if (!dont[i]) pthread_join(threads[i], NULL);
    }

    mvprintw(11, max_x/8*3-6, "RACE FINISHED");
    refresh();

    pthread_mutex_destroy(&muteksik);
    usleep(50000000);
    endwin();
    return 0;
}