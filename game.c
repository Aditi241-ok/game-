#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

#define INITIAL_LIVES 3
#define INITIAL_SPEED 150   // initial delay in ms
#define MIN_SPEED 60        // fastest speed

int playGame(void);
int loadHighScore(void);
void saveHighScore(int highScore);

int main() {
    srand((unsigned int)time(0));

    int highScore = loadHighScore();
    int quit = 0;

    while (!quit) {
        system("cls");
        printf("======== DODGE GAME ========\n");
        printf("Use LEFT and RIGHT arrow keys to move.\n");
        printf("Avoid the obstacles and survive as long as you can!\n\n");
        printf("Current High Score: %d\n\n", highScore);
        printf("1. Start Game\n");
        printf("2. Quit\n");
        printf("\nChoose an option: ");

        char choice = getch();

        if (choice == '1') {
            int score = playGame();

            if (score > highScore) {
                highScore = score;
                saveHighScore(highScore);
                printf("\nNEW HIGH SCORE: %d!\n", highScore);
            }

            printf("\nPlay again? (Y/N): ");
            char again;
            do {
                again = (char)toupper(getch());
            } while (again != 'Y' && again != 'N');

            if (again == 'N') {
                quit = 1;
            }
        } else if (choice == '2' || choice == 27) { // '2' or ESC
            quit = 1;
        }
    }

    return 0;
}

int playGame(void) {
    int x = 1;                    // player horizontal position (0 to 2)
    int step = 0;                 // obstacle vertical movement
    int obstaclePos = rand() % 3; // 0,1,2 lane
    int lives = INITIAL_LIVES;
    int score = 0;
    int speed = INITIAL_SPEED;

    int running = 1;

    while (running) {

        // ---- INPUT ----
        if (_kbhit()) {
            char ch = getch();

            if (ch == 75 && x > 0)        // LEFT arrow
                x--;

            if (ch == 77 && x < 2)        // RIGHT arrow
                x++;
        }

        // ---- DRAW ----
        system("cls");
        printf("|--- --- ---|\n");

        int i;
        for (i = 0; i < 10; i++) {
            if (i == step) {
                if (obstaclePos == 0)
                    printf("| %c        |\n", 1);
                else if (obstaclePos == 1)
                    printf("|     %c    |\n", 1);
                else if (obstaclePos == 2)
                    printf("|        %c |\n", 1);
            } else {
                printf("|           |\n");
            }
        }

        // ---- PLAYER ----
        if (x == 0)
            printf("| %c        |\n", 6);
        else if (x == 1)
            printf("|     %c    |\n", 6);
        else if (x == 2)
            printf("|        %c |\n", 6);

        printf("Score: %d   Lives: %d\n", score, lives);

        // ---- COLLISION ----
        if (step == 10 && x == obstaclePos) {
            lives--;

            if (lives <= 0) {
                printf("\nGAME OVER! Final Score: %d\n", score);
                running = 0;
            } else {
                printf("\nOuch! You lost a life. Lives left: %d\n", lives);
                Sleep(800);
                // reset obstacle position
                step = 0;
                obstaclePos = rand() % 3;
            }
        }

        if (!running) {
            break;
        }

        Sleep(speed);

        // Move obstacle down
        step++;

        // Reset when reaches bottom (player row passed safely)
        if (step > 10) {
            step = 0;
            obstaclePos = rand() % 3; // new lane
            score++;                  // increase score for successful dodge

            // ---- DIFFICULTY INCREASE ----
            if (speed > MIN_SPEED && (score % 5) == 0) {
                speed -= 10; // speed up every 5 points
            }
        }
    }

    printf("\nPress any key to return to menu...");
    getch();

    return score;
}

int loadHighScore(void) {
    FILE *fp = fopen("highscore.txt", "r");
    int highScore = 0;

    if (fp != NULL) {
        if (fscanf(fp, "%d", &highScore) != 1) {
            highScore = 0;
        }
        fclose(fp);
    }

    return highScore;
}

void saveHighScore(int highScore) {
    FILE *fp = fopen("highscore.txt", "w");
    if (fp == NULL) {
        return; // if saving fails, just ignore
    }

    fprintf(fp, "%d", highScore);
    fclose(fp);
}
