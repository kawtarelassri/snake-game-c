#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
// ---------- ENUM ----------
enum direction { STOP = 0, UP, DOWN, LEFT, RIGHT };

// ---------- STRUCTS ----------
struct map {
    int width ;
    int height;
    int fruitx;
    int fruity;
};

struct snake {
    int headx;
    int heady;
    int tailx[50];
    int taily[50];
    int tailn;
    enum direction dir;
};

struct player {
    int score ;
    bool lose;
    int level;
    int speed;
};

// ---------- GLOBAL ----------
struct snake snake1;
struct map map1;
struct player player1;


// ---------- FUNCTIONS ----------
void shift_right( int arr[], int size){
    for (int i = size -2; i>= 0; i--)
    {
        arr[ i+1] = arr[i];
    }
    
}
void generate_fruit() {
    map1.fruitx = rand() % (map1.width - 2) + 1;
    map1.fruity = rand() % (map1.height - 2) + 1;
}

void setup() {

     
    map1.width = 40;
    map1.height = 20;

    srand(time(0));
    generate_fruit();

    snake1.headx = map1.width / 2;
    snake1.heady = map1.height / 2;
    snake1.dir = STOP;   // Wait for player input first
    player1.speed = 170; // Initial speed (ms delay)
    player1.score = 0;
    player1.lose = false;
    player1.level = 1;

// Hide cursor for better visuals
CONSOLE_CURSOR_INFO cursorInfo;
cursorInfo.dwSize = 100;
cursorInfo.bVisible = FALSE;
SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

}

void draw() {
    //  Move cursor to top-left (prevents shaking)
    COORD coord = {0, 0};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);//Give me access to the console output (the screen)

    //  Draw map
    for (int i = 0; i < map1.height; i++) {
        for (int j = 0; j < map1.width; j++) {

            // Top/bottom walls
            if (i == 0 || i == map1.height - 1) {
                printf("*");
            }
            // Left/right walls
            else if (j == 0 || j == map1.width - 1) {
                printf("*");
            }
            // Snake head
            else if (i == snake1.heady && j == snake1.headx) {
                printf("O");
            }
            // Fruit
            else if (i == map1.fruity && j == map1.fruitx) {
                printf("$");
            }
            // Snake tail
            else {
                bool printed = false;
                for (int z = 0; z < snake1.tailn; z++) {
                    if (snake1.tailx[z] == j && snake1.taily[z] == i) {
                        printf("o");
                        printed = true;
                        break;
                    }
                }
                if (!printed) printf(" ");
            }
        }
        printf("\n"); // end of row
    }

    //  Display score and level
    printf("Score: %d   Level: %d\n", player1.score, player1.level);

    //  Display paused message snake movement 
        printf("Press W/A/S/D to resume\n x to exit \n space to pause\n");

    
    
}


void input() {
    if (_kbhit()) {
        char c = _getch();

        switch (c) {
        case 'w':
        case 'W':
            if (snake1.dir != DOWN)
                snake1.dir = UP;
            break;

        case 's':
        case 'S':
            if (snake1.dir != UP)
                snake1.dir = DOWN;
            break;

        case 'a':
        case 'A':
            if (snake1.dir != RIGHT)
                snake1.dir = LEFT;
            break;

        case 'd':
        case 'D':
            if (snake1.dir != LEFT)
                snake1.dir = RIGHT;
            break;

        case 'x':
        case 'X':
            player1.lose = true;
            break;
        case ' ':
        snake1.dir = STOP;  // Pause the game
            break;
        }
    }
}
void update_level()
{
    //to resize each new level
     int old_level = player1.level;
    //  Update level based on score
    player1.level = player1.score / 10 + 1; // 1 level every 10 points

    //  Set speed (Sleep delay in ms)
    player1.speed = 180- (player1.level * 10);  // faster as level increases
    if (player1.speed < 140){ player1.speed = 140;};  // minimum delay


    if(player1.level != old_level){
    // Adjust map size per level
    switch(player1.level)
    {
        case 1:
            map1.width = 40;
            map1.height = 20;
            break;
        case 2:
            map1.width = 50;
            map1.height = 22;
            break;
        case 3:
            map1.width = 60;
            map1.height = 25;
            break;
        case 4:
            map1.width = 70;
            map1.height = 28;
            break;
        default:
    
        map1.width = 70;
        map1.height = 28;
            break;

    }}
   

    //  Optional: Reposition snake safely if needed
    if (snake1.headx >= map1.width - 1) snake1.headx = map1.width / 2;
    if (snake1.heady >= map1.height - 1) snake1.heady = map1.height / 2;

    //  Regenerate fruit in case old fruit is outside new map
    if (map1.fruitx >= map1.width - 1 || map1.fruity >= map1.height - 1)
        generate_fruit();
}
void move() {

    shift_right(snake1.tailx,50);
    shift_right(snake1.taily,50);
    snake1.tailx[0] = snake1.headx;
    snake1.taily[0] = snake1.heady;

    switch (snake1.dir) {
    case UP:    snake1.heady--; break;
    case DOWN:  snake1.heady++; break;
    case LEFT:  snake1.headx--; break;
    case RIGHT: snake1.headx++; break;
    case STOP:  break;
    }

    // Wall collision
    if (snake1.headx <= 0 || snake1.headx >= map1.width - 1 ||
        snake1.heady <= 0 || snake1.heady >= map1.height - 1)
        player1.lose = true;

    // Fruit collision
    if (snake1.headx == map1.fruitx &&
        snake1.heady == map1.fruity) {

        player1.score++;
        snake1.tailn++;
        generate_fruit();
        update_level();  // Update level and adjust game parameters 
    }
}

int main() {

    setup();

    while (!player1.lose) {
        draw();
        input();
        move();
        Sleep(player1.speed);   // slower = better response
    }

    system("cls");
    printf("Game Over!\n");
    printf("Final Score: %d\n", player1.score);
    system("pause");

    return 0;
}
