#include <stdio.h>
#include <windows.h>
#define fallspeed 7500 //bigger is slower
#define MAX 200
#define GENSPEED 9500 //bigger is slower
#define GROUND 20

struct drop
{
  char symbol;
  int row, col, speed, free;
  int wait; /* wait for it to drop */   /* wait is like clock to speed when wait becomes zero the speed moves forward */
};
struct drop rain[MAX];

COORD coord = {0,0};
void gotoxy (int x, int y)
{
    coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void free_all_drops()
{
    int i ;
    for ( i = 0 ; i < MAX ; i++) rain[i].free = 1 ; // one means used it
}

int search_free_pos()
{
    int p = 0 ;
    while (rain[p].free == 0 && p < MAX) p++;
    if(p == MAX) return -1;
    else return p ;
}
void generate_drop()
{
    int p ;

    p = search_free_pos();
    if(p != -1)
    {
        rain[p].symbol = rand() %26 + 65;
        /*because there are total 26 alphabets so mod of 26 will give u 25 remainders
        so 25 + 65 will give u random numbers since  A = 65  z = 90, random A to Z*/
        rain[p].row = 0 ;
        rain[p].free = 0 ;
        rain[p].col = rand() %61 ; // window screen between o to  60
        rain[p].speed = (rand() %8 + 3)*fallspeed; // speed between 300000 to 10000000
        rain[p].wait = rain[p].speed; // wtv u random in the speed put it in the speed.
        gotoxy(rain[p].col,rain[p].row);
        printf("%c",rain[p].symbol);
        gotoxy(rain[p].col,rain[p].row);
    }
}

void start_random_engine () // randoming a number
{
  int i, stime;
  long ltime;

  /* get the current calendar time */
  ltime = time(NULL);
  stime = (unsigned) ltime/2;
  srand(stime);
}

int drops_move_down()
{
    int i, count = 0;

    for(i = 0; i < MAX; i++)
    {
        if(!rain[i].free)
        {
            rain[i].wait--;
            if(rain[i].wait == 0)
            {
                rain[i].wait = rain[i].speed;
                gotoxy(rain[i].col, rain[i].row);
                printf(" ");
                rain[i].row ++ ;
                if(rain[i].row < GROUND)
                {
                 gotoxy(rain[i].col,rain[i].row);
                 printf("%c",rain[i].symbol);
                 gotoxy(rain[i].col,rain[i].row);
                }
                else
                {
                    rain[i].free = 1;
                    count = count + 1;
                }
            }
        }
    }
    return count;
}

int process_key()
{
    char key;
    int i, maxrow = -1, p = -1;

    key = toupper(getch());

    for (i = 0; i < MAX; i++)
    {
        if (rain[i].free == 0 && rain[i].symbol == key)
        {
            if (rain[i].row > maxrow)
            {
                p = i;
                maxrow = rain[i].row;
            }
        }
    }
    if (p == -1)
    {
        return -1;
    }
    else
    {
        gotoxy(rain[p].col, rain[p].row);
        printf(" ");
        rain[p].free = 1;

        return 1;
    }
}

int main()
{
    long count_wait_gen;
    int score, lives, dead, i;
    char ans;

    do
    {
        score = 0;
        lives = 5;
        system("cls");
        start_random_engine();
        gotoxy (65,0);
        printf("|");
        gotoxy (65,1);
        printf("|     SCORE");
        gotoxy (65,2);
        printf("|     %5d", score);
        for (i = 3; i < 6; i++)
        {
            gotoxy (65,i);
            printf("|");
        }
        gotoxy (65,6);
        printf("|     LIVES");
        gotoxy (65,7);
        printf("|     %3d", lives);
        for (i = 8; i <= 20; i++)
        {
            gotoxy (65,i);
            printf("|");
        }
        for (i = 0; i < 65; i++)
        {
            gotoxy (i,21);
            printf("^");
            gotoxy (i,22);
            printf("~");
            gotoxy (i,23);
            printf("~");
        }

        free_all_drops();
        count_wait_gen = 500 ;
        while (lives > 0)
        {
            if (kbhit())
            {
                score = score + process_key();
                gotoxy (71,2);
                printf("%5d", score);
            }
            if(count_wait_gen == 0)
            {
                generate_drop();
                count_wait_gen = rand()%GENSPEED + GENSPEED;
            }
            else count_wait_gen--;

            dead = drops_move_down();
            if (dead != 0)
            {
                lives = lives - dead;
                if (lives < 0) lives = 0;
                gotoxy(71,7);
                printf("%3d", lives);
            }

        }
        system("cls");
        gotoxy(33,5); printf("GAME OVER");
        gotoxy(30,7); printf("Your score is  %d", score);
        fflush(stdin);
        gotoxy(29,9); printf("Continue.... (Y/N)");
        ans = toupper(getch());
    } while (ans == 'Y');

    return 0;
}
