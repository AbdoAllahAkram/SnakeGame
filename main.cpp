#include <iostream>
using namespace std;
#include <conio.h>
#include <windows.h>
#include <dos.h>
#include <time.h>

#define MAXSNAKESIZE 100
#define MAXFRAMEX 120
#define MAXFRAMEY 30


HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) // set bool visible = 0 - invisible, bool visible = 1 - visible
{
    if(size == 0)
    {
        size = 20;	// default cursor size Changing to numbers from 1 to 20, decreases cursor width
    }
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console,&lpCursor);
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
        if(y < 0)
            y = MAXFRAMEY;
    }
    void MoveDown() {
        y++;
        if(y > MAXFRAMEY)
            y = 0;
    }
    void MoveRight() {
        x++;
        if(x > MAXFRAMEX)
            x = 0;
    }
    void MoveLeft() {
        x--;
        if(x < 0)
            x = MAXFRAMEX;
    }
    void Drow(char ch='0') {
        gotoxy(x, y);
        cout<<ch;
    }
    void Erase() {
        gotoxy(x, y);
        cout<<" ";
    }
    void CopyPos(Point * p) {
        p->x = x;
        p->y = y;
    }
    int IsEqual(Point *p) {
        if(p->x == x && p->y == y)
            return 1;
        return 0;
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
    int state=1; // bool represent the state of the snake, living, dead
    int started=1;
    int blink; // make fruit blink
public:
    Snake() {
        size = 1; // default size of snake
        state = 0;
        cell[0] = new Point(10, 10); // 10,10 is default position of snake
        for(int i=1; i< MAXSNAKESIZE; i++) {
            cell[size] = NULL;
        }
        fruit.SetPoint(rand()%MAXFRAMEX, rand()%MAXFRAMEY);
    }

    void WelcomeScreen() {
        gotoxy(70, 7);
        SetConsoleTextAttribute(console, 15);
        cout<<"\n   _________          _________";
        cout<<"\n  /         \\       /         \\   AbdoAllah Akram";
        cout<<"\n /  /~~~~~\\ \\    /  /~~~~~\\ \\";
        cout<<"\n |  |     |  |     |  |     |  |";
        cout<<"\n |  |     |  |     |  |     |  |";
        cout<<"\n |  |     |  |     |  |     |  |      /";
        cout<<"\n |  |     |  |     |  |     |  |    //";
        cout<<"\n(o  o)    \\ \\_____/  /     \\ \\____/ /";
        cout<<"\n \\_//      \\        /       \\       /";
        cout<<"\n  |        ~~~~~~~~~         ~~~~~~~~";
        cout<<"\n  ^";
        cout<<"\n ";
    }

    void AddCell(int x, int y) {
        cell[size++] = new Point(x, y);
    }
    void TurnUp() {
        if(dir != 's')
            dir = 'w'; // w is control key for turning up
    }
    void TurnDown() {
        if(dir != 'w')
            dir = 's'; // s is control key for turning down
    }
    void TurnRight() {
        if(dir != 'a')
            dir = 'd'; // d is control key for turning right
    }
    void TurnLeft() {
        if(dir != 'd')
            dir = 'a'; // w is control key for turning left
    }
    void Move() {
        // clear screen
        system("cls");

        if(state == 0) {
            if(started) {

                WelcomeScreen();
                cout<< "\nPress any key to start";
                getch();
                started = 1;
                state = 1;
            } else {

                gotoxy(MAXFRAMEX/2-5,MAXFRAMEY/2);
                cout<< "Game Over!";
                gotoxy(MAXFRAMEX/2-11,MAXFRAMEY/2);
                cout<< "\nPress any key to start";
                getch();
                state = 1;
                size =1;
            }

        }

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
            fruit.SetPoint(rand()%MAXFRAMEX, rand()%MAXFRAMEY);
        }

        // collision with snake itself
        if(SelfCollision())
            state =0; started = 0;



        // drawing snake
        for(int i=0; i<size; i++) {
            cell[i]->Drow();
        }
        SetConsoleTextAttribute(console, 242);
        if(!blink)
            fruit.Drow('#');
        blink = !blink;
        SetConsoleTextAttribute(console, 252);
        //Debug();
        Sleep(30);

    }
    int SelfCollision() {
        for(int i=1; i<size; i++)
            if(cell[0]->IsEqual(cell[i]))
                return 1;
        return 0;
    }

    void Debug() {
        for(int i=0; i<size; i++) {
            cell[i]->Debug();
        }
    }



};

int main() {

    setcursor(0, 0);

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
