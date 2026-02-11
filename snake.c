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
    int width;
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
    int score;
    bool lose;
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

    player1.score = 0;
    player1.lose = false;
}

void draw() {
    system("cls");

    for (int i = 0; i < map1.height; i++) {
        for (int j = 0; j < map1.width; j++) {

            if (i == 0 || i == map1.height - 1)
                printf("*");
            else if (j == 0 || j == map1.width - 1)
                printf("*");
            else if (i == snake1.heady && j == snake1.headx)
                printf("O");
            else if (i == map1.fruity && j == map1.fruitx)
                printf("$");
            else{
            bool printed = false ;    
            for (int z = 0; z < snake1.tailn;z++)
                {
                    if (snake1.tailx[z] == j && snake1.taily[z] == i)
                    {
                       printf("o");
                       printed = true ;
                       break;
                    }    
                }
             if (!printed)
             {
                printf(" ");
             }
             }
                
        }
        printf("\n");
    }

    printf("Score: %d\n", player1.score);
    printf("Use W A S D to move. X to exit.\n");
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
        }
    }
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
    }
}

int main() {

    setup();

    while (!player1.lose) {
        draw();
        input();
        move();
        Sleep(120);   // slower = better response
    }

    system("cls");
    printf("Game Over!\n");
    printf("Final Score: %d\n", player1.score);
    system("pause");

    return 0;
}
// next step  add colors obstacls and more levels and make it multiplayer , and add a menu and make it more responsive and add a high score system and make it more fun and addictive and add sound effects and music and make it more visually appealing and add more features and make it more enjoyable to play.