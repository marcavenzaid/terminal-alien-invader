#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
//#include <conio.h>
#include "_conio.h"
#include "conio.h"

#define YMAXCELLS 25
#define XMAXCELLS 55
#define XFIRSTCELL 25
#define YFIRSTCELL 0

void ShipControls();
void PrintElements();
void ShipBulletMovement();
void Borders();
void AliensFormation();
void AliensMovement();
void AlienAttack();
void Collisions();
void Stats();
void CheckWin();
void End();

char Name[20][10];

void HowToPlay();
int MainMenu();
void Normal(int x, int y, char *s);
void Reverse(int x, int y, char *s);
int PlayerNum;

void ChooseShip();

char Dimension[YMAXCELLS][XMAXCELLS+1];        /* Total size of Array is equal to 1375 Cells */
int xCells, yCells;                          /* Total size of Array inside Border is equal to 750 Cells */
int xCenterCell = 40;

char Ship;
int ShipRow = 23;
char ShipBullet;
int ShipReload;
int ShipReloadTime = 8;
char KeyHit;

char Alien = 157;
char AlienBullet = 249;
int AlienReload;
int AlienMoveTimer;
int AliensDirection, Left = 1, Right = 2, Down;
int AliensLeft, CurrentAliens, TotalAliens = 32;

char Explosion = '*';

int Score = 0;
int ShipBulletsFired = 0;
int Life = 3;

int Win = 0;
int StartGame = 0;
int Choose = 0;
int ShipChoice = 0;
int x,y;

/*********************************************** [ Main ] ***********************************************/

/*---------- [ Menu ] ----------*/

main() {
    int PullDown;

    while(1) {
        Score = 0;
        ShipBulletsFired = 0;
        Life = 3;
        Win = 0;
        StartGame = 0;
        Choose = 0;
        ShipChoice = 0;
//        for (yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells ++) {
//            for (xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells ++) {
//                Dimension[yCells][xCells] = ' ';
//            }
//        }
        for (yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells ++) {
            for (xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells ++) {
                Dimension[yCells][xCells] = ' ';
            }
        }

        system("cls");
        c_gotoxy(2,2);
        printf("Enter your name (No spaces and maximum of 10 characters only.)");
        c_gotoxy(2,4);
        scanf("%s", Name);

        while(StartGame != 1) {
            PullDown=MainMenu();
            switch(PullDown) {
            case 1:
                StartGame = 1;
                break;
            case 3:
                HowToPlay();
                break;
            case 5:
                system("cls");
                exit(0);
            }
            system("cls");
        }
        window(1,1,80,25);
        c_textbackground(BLACK);
        system("cls");

        /*---------- [ Choose your Ship ] ----------*/

        while(Choose != 1) {
            ChooseShip();
        }
        system("cls");

        /*---------- [ Game ] ----------*/

        AliensFormation();          /* Prints Aliens first location formation */
        AliensDirection = Left;    /* Aliens First Direction */
        AlienReload = 0;
        AlienMoveTimer = 0;

        Borders();      /* Prints Borders in both sides */

        Dimension[ShipRow][xCenterCell] = Ship;
        ShipReload = 3;

        while(Win != 1 && Win != 2) {
            Stats();                /* Prints Scores, Prints NumberOfAttacks, and Counts Remaining Aliens */
            PrintElements();        /* Prints Elements(char) inside of Dimension(Array) */
            Collisions();           /* Decides what will happen if two elements(char) collide */
            ShipBulletMovement();   /* Moves the Ship Bullet */
            AliensMovement();       /* Decides Aliens Direction then Moves them */
            AlienAttack();          /* Moves the Aliens Bullets, Fires Alien Bullet, and Calculates which Alien will fire */
            ShipControls();         /* Player controls for the Ship. Movement and Attack */
            CheckWin();             /* Checks if Player Win or Lose */
            Sleep(10);             /* Delays Movements */
        }
        End();      /* Prints Player Stats after game ends */
    }
}

/*********************************************** [ (1) Before While Loop ] ***********************************************/

/* Prints Aliens first location formation */
void AliensFormation() {
    for(yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells = yCells+2) {
        for(xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells = xCells+2) {
            if(xCells >= XFIRSTCELL+7 && xCells <= XMAXCELLS-8 && yCells < 7) {
                Dimension[yCells][xCells] = Alien;
                CurrentAliens++;
            } else {
                Dimension[yCells][xCells] = ' ';
            }
        }
    }
}

/* Prints Borders in both sides */
void Borders() {
    for (yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells ++) {
        c_gotoxy(XFIRSTCELL-1, yCells+1);
        printf("%c", 186);
        c_gotoxy(XMAXCELLS+1,yCells+1);
        printf("%c", 186);
    }
}

/*********************************************** [ (2) While Loop ] ***********************************************/
/** [ Game Functions ] **/
/** [ Runs the following functions in order ] **/

/* Prints Scores, Prints NumberOfAttacks, and Counts Remaining Aliens */
void Stats() {
    int i;
    c_gotoxy( 2,2);
    printf("Player: %s", Name);
    c_gotoxy( 2,4);
    printf("Score: %-5d", Score);
    c_gotoxy(58,2);
    printf("Bullets Fired: %-3d", ShipBulletsFired);
    c_gotoxy(2,24);
    printf("Life: ");
    for(i = 1; i <= Life; i++) {
        c_textcolor(LIGHTRED);
        printf("o ");
    }
    c_textcolor(LIGHTGRAY);
    printf(" ");

    CurrentAliens = 0;
    for(xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells++) {
        for(yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells++) {
            if(Dimension[yCells][xCells] == Alien) {
                CurrentAliens++;
            }
        }
    }
    c_gotoxy(58,4);
    printf("Remaining Aliens: %-3d", CurrentAliens);

    c_gotoxy(58,24);
    printf("Move: Arrow Keys / WASD");
    //c_gotoxy(58,25);
    //printf("Attack: [Space]");
}

/* Prints Elements(char) inside of Dimension(Array) */
void PrintElements() {
    for (yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells ++) {
        c_gotoxy(XFIRSTCELL, yCells+1);
        for (xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells ++) {
            if(Dimension[yCells][xCells] == ShipBullet) {
                if(ShipBullet == 34) {
                    c_textcolor(YELLOW);
                    printf("%c", Dimension[yCells][xCells]);
                } else if(ShipBullet == 94) {
                    c_textcolor(LIGHTRED);
                    printf("%c", Dimension[yCells][xCells]);
                } else {
                    c_textcolor(LIGHTCYAN);
                    printf("%c", Dimension[yCells][xCells]);
                }
            } else if(Dimension[yCells][xCells] == AlienBullet) {
                c_textcolor(LIGHTMAGENTA);
                printf("%c", Dimension[yCells][xCells]);
            } else {
                printf("%c", Dimension[yCells][xCells]);
            }
            c_textcolor(LIGHTGRAY);
        }
    }
}

/* Decides what will happen if two elements(char) collide */
void Collisions() {
    for(xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells++) {
        for(yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells++) {
            if(Dimension[yCells][xCells] == ShipBullet && Dimension[yCells-1][xCells] == Alien) {
                Dimension[yCells-1][xCells] = Explosion;
                Dimension[yCells][xCells] = ' ';
                Score = Score + 39;
            } else if(Dimension[yCells][xCells] == Explosion) {
                Dimension[yCells][xCells] = ' ';
            } else if(Dimension[yCells][xCells] == ShipBullet && Dimension[yCells-1][xCells] == AlienBullet) {
                Dimension[yCells][xCells] = ' ';
            } else if(Dimension[yCells][xCells] == Ship && Dimension[yCells-1][xCells] == AlienBullet) {
                Dimension[yCells-1][xCells] = ' ';
                Life = Life -1;
                if(Life == 0) {
                    Win = 2;
                }
            } else if(Dimension[yCells-1][xCells] == AlienBullet && Dimension[yCells][xCells] == Alien) {
                Dimension[yCells-1][xCells] = ' ';
            }
        }
    }
}

/* Moves the Ship Bullet */
void ShipBulletMovement() {
    for(yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells++) {
        for(xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells++) {
            if(Dimension[yCells][xCells] == ShipBullet) {
                Dimension[yCells][xCells] = ' ';
                Dimension[yCells-1][xCells] = ShipBullet;
            }
        }
    }
}

/* Decides Aliens Direction then Moves them */
void AliensMovement() {
    AlienMoveTimer++;
    Down = 0;
    for(yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells++) {
        if(Dimension[yCells][XFIRSTCELL] == Alien) {
            AliensDirection = Right;
            Down = 1;
        } else if(Dimension[yCells][XMAXCELLS] == Alien) {
            AliensDirection = Left;
            Down = 1;
        }
    }
    if(AliensDirection == Right && AlienMoveTimer >= 5) {
        for(xCells = XMAXCELLS; xCells >= XFIRSTCELL; xCells--) {
            for(yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells++) {
                if(Dimension[yCells][xCells] == Alien && Down == 0) {
                    Dimension[yCells][xCells+1] = Dimension[yCells][xCells];
                    Dimension[yCells][xCells] = ' ';
                } else if(Dimension[yCells][xCells] == Alien && Down == 1) {
                    Dimension[yCells+1][xCells+1] = Dimension[yCells][xCells];
                    Dimension[yCells][xCells] = ' ';
                }
                AlienMoveTimer = 0;
            }
        }
    } else if(AliensDirection == Left && AlienMoveTimer >= 5) {
        for(xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells++) {
            for(yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells++) {
                if(Dimension[yCells][xCells] == Alien && Down == 0) {
                    Dimension[yCells][xCells-1] = Dimension[yCells][xCells];
                    Dimension[yCells][xCells] = ' ';
                } else if(Dimension[yCells][xCells] == Alien && Down == 1) {
                    Dimension[yCells+1][xCells-1] = Dimension[yCells][xCells];
                    Dimension[yCells][xCells] = ' ';
                }
                AlienMoveTimer = 0;
            }
        }
    }
}

/* Moves the Aliens Bullets, Fires Alien Bullet, and Calculates which Alien will fire */
void AlienAttack() {
    AlienReload++;
    for(xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells++) {
        for(yCells = YMAXCELLS-1; yCells >= YFIRSTCELL; yCells--) {
            if(Dimension[yCells][xCells] == AlienBullet) {
                Dimension[yCells+1][xCells] = AlienBullet;
                Dimension[yCells][xCells] = ' ';
            }
        }
    }
    for(xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells++) {
        for(yCells = YFIRSTCELL; yCells < YMAXCELLS; yCells++) {
            if(AlienReload > 3 && Dimension[yCells][xCells] == Alien && (rand() %10) > 8 && Dimension[yCells+2][xCells] != Alien && Dimension[yCells+4][xCells] != Alien && Dimension[yCells+6][xCells] != Alien) {
                Dimension[yCells+1][xCells] = AlienBullet;
                AlienReload = 0;
            }
        }
    }
}

/* Player controls for the Ship. Movement and Attack */
void ShipControls() {
    if(c_kbhit()) {
        KeyHit = c_getch();
    } else {
//        KeyHit = -1;
    }
    ShipReload++;

    /* Left Arrow Key - Move Left */
    if(KeyHit == 75 || KeyHit == 'a') {
//        for(xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells++) {
//            if(Dimension[ShipRow][xCells] == Ship && xCells != XFIRSTCELL) {
//                Dimension[ShipRow][xCells-1] = Ship;
//                Dimension[ShipRow][xCells] = ' ';
//            }
//        }
        for(int i = XFIRSTCELL; i <= XMAXCELLS; i++) {
            if(Dimension[ShipRow][i] == Ship && i != XFIRSTCELL) {
                Dimension[ShipRow][i-1] = Ship;
                Dimension[ShipRow][i] = ' ';
            }
        }
    }
    /* Right Arrow Key - Move Right */
    else if(KeyHit == 77 || KeyHit == 'd') {
//        for(xCells = XMAXCELLS; xCells >= XFIRSTCELL; xCells--) {
//            if(Dimension[ShipRow][xCells] == Ship && xCells != XMAXCELLS) {
//                Dimension[ShipRow][xCells+1] = Ship;
//                Dimension[ShipRow][xCells] = ' ';
//            }
//        }
        for(int i = XMAXCELLS; i >= XFIRSTCELL; i--) {
            if(Dimension[ShipRow][i] == Ship && i != XMAXCELLS) {
                Dimension[ShipRow][i+1] = Ship;
                Dimension[ShipRow][i] = ' ';
            }
        }
    }

    if(ShipReload > ShipReloadTime) {
        for(xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells++) {
            if(Dimension[ShipRow][xCells] == Ship) {
                Dimension[ShipRow-1][xCells] = ShipBullet;
                ShipBulletsFired++;
            }
        }
        ShipReload = 0;
    }
}

/* Checks if Player Win or Lose */
void CheckWin() {
    if(CurrentAliens == 0) {
        Win = 1;
    }
    for(xCells = XFIRSTCELL; xCells <= XMAXCELLS; xCells++) {
        if(Dimension[YMAXCELLS-1][xCells] == Alien) {
            Win = 2;
        }
    }
}

/*********************************************** [ (3) After While Loop ] ***********************************************/

/* Prints Player Stats after game ends*/
void End() {
    if(Win == 1) {
        system("cls");
        c_gotoxy(35,12);
        printf("YOU WIN!");
        Sleep(3000);
        system("cls");
        c_gotoxy(30,5);
        printf("Player: %s", Name);
        Sleep(500);
        c_gotoxy(30,8);
        printf("Score: %d", Score);
        Sleep(500);
        c_gotoxy(30,10);
        printf("Bullets Fired: %d", ShipBulletsFired);
        Sleep(500);
        c_gotoxy(30,12);
        printf("%d - %d", Score, ShipBulletsFired);
        Score = Score - ShipBulletsFired;
        if(Score < 0) {
            Score = 0;
        }
        Sleep(500);

        c_gotoxy(30,14);
        printf("Winner Bonus + 1000");
        Score = Score + 1000;
        Sleep(500);
        c_gotoxy(30,16);
        printf("Total Score: %d", Score);
        Sleep(500);
        c_gotoxy(30,19);
        printf("Press any key to continue.");
        c_getch();
        system("cls");
    } else if(Win == 2) {
        system("cls");
        c_gotoxy(35,12);
        printf("YOU LOSE!");
        Sleep(3000);
        system("cls");
        c_gotoxy(30,5);
        printf("Player: %s", Name);
        Sleep(500);
        c_gotoxy(30,8);
        printf("Score: %d", Score);
        Sleep(500);
        c_gotoxy(30,10);
        printf("Bullets Fired: %d", ShipBulletsFired);
        Sleep(500);
        c_gotoxy(30,12);
        printf("%d - %d", Score, ShipBulletsFired);
        Score = Score - ShipBulletsFired;
        if(Score < 0) {
            Score = 0;
        }
        Sleep(500);

        c_gotoxy(30,14);
        printf("Total Score: %d", Score);
        Sleep(1000);
        c_gotoxy(30,17);
        printf("Press any key to continue.");
        c_getch();
        system("cls");
    }
}

/*********************************************** [ Main Menu ] ***********************************************/

int MainMenu() {
    int i, Tab;
    char ch;
    char MenuTabs[][30]= {"   Start Game   ", "----------------", "  How to Play   ", "----------------", "      Exit      ", "::::::::::::::::"};
    system("cls");

    x = c_wherex();
    y = c_wherey();
    window(x,y,x+50,y+16);    /* Window for Menu */
    c_textbackground(BLACK);
    system("cls");

    /* Borders for Menu */
    c_gotoxy(1,1);
    printf("%c",201);
    for(i=1; i<=18; i++) {
        printf("%c",205);
    }
    printf("%c",187);
    for(i=1; i<=11; i++) {
        c_gotoxy( 1,i+1);
        printf("%c",186);
    }
    for(i=1; i<=11; i++) {
        c_gotoxy(20,i+1);
        printf("%c",186);
    }
    c_gotoxy(1,13);
    printf("%c",200);
    for(i=1; i<=18; i++) {
        printf("%c",205);
    }
    printf("%c",188);

    /* Borders for Guide */
    c_gotoxy(1,14);
    printf("%c",201);
    for(i=1; i<=27; i++) {
        printf("%c",205);
    }
    printf("%c",187);
    for(i=1; i<=2; i++) {
        c_gotoxy( 1,i+14);
        printf("%c",186);
    }
    for(i=1; i<=2; i++) {
        c_gotoxy(29,i+14);
        printf("%c",186);
    }
    c_gotoxy(1,17);
    printf("%c",200);
    for(i=1; i<=27; i++) {
        printf("%c",205);
    }
    printf("%c",188);

    /* Prints Guide */
    c_gotoxy(2,15);
    printf(" Choose: Arrow Keys / WASD ");
    c_gotoxy(2,16);
    printf(" Select: [Enter]");

    c_textcolor(BLACK);
    c_textbackground(WHITE);
    c_gotoxy(3,2);
    printf("      MENU      ");
    c_gotoxy(3,4);
    printf("::::::::::::::::");

    /* Prints pull down Tabs */
    for(Tab = 0; Tab < 6; Tab++) {
        Normal(3, 5+Tab, MenuTabs[Tab]);
    }

    /* Reverse the first tab */
    Tab = 0;
    Reverse(3, 5+Tab, MenuTabs[Tab]);

    ch = c_getch();
    while(ch != 13) {
        if(ch == 80) {
            Normal(3, 5+Tab, MenuTabs[Tab]);
            Tab = (Tab+2)%6;
            Reverse(3, 5+Tab, MenuTabs[Tab]);
        } else if(ch == 72) {
            Normal(3, 5+Tab, MenuTabs[Tab]);
            if(Tab < 1) {
                Tab = 6;
            }
            Tab=(Tab-2);
            Reverse(3, 5+Tab, MenuTabs[Tab]);
        }
        ch = c_getch();
    }
    return(Tab+1);
}

void Normal(int x, int y, char *s) {
    c_textcolor(BLACK);
    c_textbackground(WHITE);
    c_gotoxy(x,y);
    printf("%s", s);
}

void Reverse(int x, int y, char *s) {
    c_textcolor(WHITE);
    c_textbackground(BLACK);
    c_gotoxy(x,y);
    printf("%s", s);
}

/*********************************************** [ How to Play ] ***********************************************/
void HowToPlay() {
    window(1,1,80,25);
    c_textbackground(BLACK);
    system("cls");
    c_gotoxy(2,1);
    printf(" Controls:");
    c_gotoxy(2,3);
    printf(" * Use left and right Arrow Keys / WASD to move your ship.");
    c_gotoxy(2,5);
    printf(" * Your ship will automatically shoot.");
    c_gotoxy(2,8);
    printf(" Objective:");
    c_gotoxy(2,10);
    printf(" * Kill all the Aliens to win the game.");
    c_gotoxy(2,12);
    printf(" * You lose if your Life is zero or if Aliens reached your location.");
    c_gotoxy(2,15);
    printf(" Scoring:");
    c_gotoxy(2,17);
    printf(" * Killing an Alien will add 39 points to your score.");
    c_gotoxy(2,19);
    printf(" * Your total number of bullets fired will be subtracted to your score.");
    c_gotoxy(2,21);
    printf(" * If you Win, an additional 1000 points will be added to your score.");
    c_gotoxy(2,24);
    printf(" Press any key to continue.");
    c_getch();
}

/*********************************************** [ Choose Ship ] ***********************************************/
void ChooseShip() {
    int i;

    c_gotoxy(2,2);
    printf("Choose your Ship: ");
    /*---------------------------------------- First Box ----------------------------------------*/
    c_gotoxy(2,5);
    printf("%c", 218);
    for(i = 1; i <= 3; i++) {
        printf("%c", 196);
    }
    printf("%c", 191);
    for(i = 1; i <= 3; i++) {
        c_gotoxy(2,5+i);
        printf("%c", 179);
    }

    c_gotoxy(4,6);
    c_textcolor(LIGHTCYAN);
    printf("%c", 179);   /* Print Bullet */
    c_gotoxy(4,8);
    c_textcolor(WHITE);
    printf("%c", 207);   /* Print Ship */
    c_gotoxy(8,6);
    printf("Bullet A");
    c_gotoxy(8,8);
    printf("Ship: 1");

    for(i = 1; i <= 3; i++) {
        c_gotoxy(6,5+i);
        printf("%c", 179);
    }
    c_gotoxy(2,9);
    printf("%c", 192);
    for(i = 1; i <= 3; i++) {
        printf("%c", 196);
    }
    printf("%c", 217);

    /*---------------------------------------- Second Box ----------------------------------------*/
    c_gotoxy(30,5);
    printf("%c", 218);
    for(i = 1; i <= 3; i++) {
        printf("%c", 196);
    }
    printf("%c", 191);
    for(i = 1; i <= 3; i++) {
        c_gotoxy(30,5+i);
        printf("%c", 179);
    }

    c_gotoxy(32,6);
    c_textcolor(LIGHTRED);
    printf("%c", 94);   /* Print Bullet */
    c_gotoxy(32,8);
    c_textcolor(WHITE);
    printf("%c", 65);   /* Print Ship */
    c_gotoxy(36,6);
    printf("Bullet B");
    c_gotoxy(36,8);
    printf("Ship: 2");

    for(i = 1; i <= 3; i++) {
        c_gotoxy(34,5+i);
        printf("%c", 179);
    }
    c_gotoxy(30,9);
    printf("%c", 192);
    for(i = 1; i <= 3; i++) {
        printf("%c", 196);
    }
    printf("%c", 217);

    /*---------------------------------------- Third Box ----------------------------------------*/
    c_gotoxy(58,5);
    printf("%c", 218);
    for(i = 1; i <= 3; i++) {
        printf("%c", 196);
    }
    printf("%c", 191);
    for(i = 1; i <= 3; i++) {
        c_gotoxy(58,5+i);
        printf("%c", 179);
    }

    c_gotoxy(60,6);
    c_textcolor(YELLOW);
    printf("%c", 34);    /* Print Bullet */
    c_gotoxy(60,8);
    c_textcolor(WHITE);
    printf("%c", 72);   /* Print Ship */
    c_gotoxy(64,6);
    printf("Bullet C");
    c_gotoxy(64,8);
    printf("Ship: 3");

    for(i = 1; i <= 3; i++) {
        c_gotoxy(62,5+i);
        printf("%c", 179);
    }
    c_gotoxy(58,9);
    printf("%c", 192);
    for(i = 1; i <= 3; i++) {
        printf("%c", 196);
    }
    printf("%c", 217);
    /*-------------------------------------------------------------------------------------------*/

    /* Scanf the chosen ship */
    c_gotoxy(2,18);
    printf("Enter the number of your Choice:");
    c_gotoxy(2,20);
    printf("Ship: ");
    scanf("%d", &ShipChoice);
    fflush(stdin);
    if(ShipChoice == 1) {
        Ship = 207;
        ShipBullet = 179;
        Choose = 1;
    } else if(ShipChoice == 2) {
        Ship = 65;
        ShipBullet = 94;
        Choose = 1;
    } else if(ShipChoice == 3) {
        Ship = 72;
        ShipBullet = 34;
        Choose = 1;
    } else {
        c_gotoxy(2,22);
        printf("Invalid number, Choose from 1 - 3 only.");
        c_getch();
        system("cls");
    }
    c_textcolor(LIGHTGRAY);
}
