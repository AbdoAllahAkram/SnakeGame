#include <iostream>
using namespace std;
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>

#define MAXSNAKESIZE 100

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

class Point {
private:
    int x;
    int y;
public:
    Point() {
        x = y = 10;
    }
    Point(int x, int y) {
        this->x = x;
        this->y = y;
    }
    void SetPoint(int x, int y) {
        this->x = x;
        this->y = y;
    }
    void FllowPoint() {
        ++x;
        ++y;
    }
    int GetX() {
        return x;
    }
    int GetY() {
        return y;
    }
    void MoveUp() {
        y--;
    }
    void MoveDown() {
        y++;
    }
    void MoveRight() {
        x++;
    }
    void MoveLeft() {
        x--;
    }
    void Drow() {
        gotoxy(x, y);
        cout<<"*";
    }
    void Erase() {
        gotoxy(x, y);
        cout<<" ";
    }
    void CopyPos(Point * p) {
        p->x = x;
        p->y = y;
    }
    void Debug() {
        cout<<"("<<x<<", "<<y<<")";
    }

};

class Snake {
private:
    Point * cell[MAXSNAKESIZE]; // array of Points to represent snake
    int size; // current size of snake
    char dir; // current direction of snake
    Point fruit;
public:
    Snake() {
        size = 1; // default size of snake
        cell[0] = new Point(10, 10); // 10,10 is default position of snake
        for(int i=1; i< MAXSNAKESIZE; i++) {
            cell[size] = NULL;
        }
        fruit.SetPoint(rand()%50, rand()%25);
    }

    void AddCell(int x, int y) {
        cell[size++] = new Point(x, y);
    }
    void TurnUp() {
        dir = 'w'; // w is control key for turning up
    }
    void TurnDown() {
        dir = 's'; // s is control key for turning down
    }
    void TurnRight() {
        dir = 'd'; // d is control key for turning right
    }
    void TurnLeft() {
        dir = 'a'; // w is control key for turning left
    }
    void Move() {
        // clear screen
        system("cls");

        // making snake body follow its head
        for(int i=size-1; i>0; i--) {
            cell[i-1]->CopyPos(cell[i]);
            }

        // turning snake's head
       // for (int i=0; i<size; i++) {
            switch (dir) {
                case 'w':
                    // for(int i=0; i< size; i++)
                    cell[0]->MoveUp();
                    break;
                case 's':
                    //for(int i=0; i< size; i++)
                    cell[0]->MoveDown();
                    break;
                case 'd':
                    //for(int i=0; i< size; i++)
                    cell[0]->MoveRight();
                    break;
                case 'a':
                    //for(int i=0; i< size; i++)
                    cell[0]->MoveLeft();
                    break;
            }
       // }

        //collision with fruit
        if(fruit.GetX() == cell[0]->GetX() && fruit.GetY() == cell[0]->GetY()) {
            AddCell(cell[size-1]->GetX()-1, cell[size-1]->GetY());
            //AddCell(0, 0);
            fruit.SetPoint(rand()%50, rand()%25);
        }

        // drawing snake
        for(int i=0; i<size; i++) {
            cell[i]->Drow();
        }
        fruit.Drow();
        //Debug();
        Sleep(100);

    }

    void Debug() {
        for(int i=0; i<size; i++) {
            cell[i]->Debug();
        }
    }



};

int main() {
    //random no generation
    srand( (unsigned)time(NULL));

    // testing snake
    Snake snake;
    char op ='l';
    do {
        if(kbhit()) {
            op = getch();
            }
            switch (op) {
                case 'w':
                case 'W':
                    snake.TurnUp();
                    break;
                case 's':
                case 'S':
                    snake.TurnDown();
                    break;
                case 'd':
                case 'D':
                    snake.TurnRight();
                    break;
                case 'a':
                case 'A':
                    snake.TurnLeft();
                    break;
            }
            snake.Move();
    }while(op != 'e');
    return 0;
    // comment
}
