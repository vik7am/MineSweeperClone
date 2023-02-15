#include <iostream>
using namespace std;
#define ROW_SIZE 9
#define COLUMN_SIZE 9

class MineSweeper{
    char board[ROW_SIZE][COLUMN_SIZE];
    public:
    MineSweeper(){
        for(int i=0; i<ROW_SIZE; i++)
            for(int j=0; j<COLUMN_SIZE; j++)
                board[i][j] = '-';
    }
    void printBoard(){
        for(int i=0; i<ROW_SIZE; i++){
            for(int j=0; j<COLUMN_SIZE; j++){
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
    }
};

int main(){
    MineSweeper game;
    game.printBoard();
    return 0;
}