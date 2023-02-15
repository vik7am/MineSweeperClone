#include <iostream>
using namespace std;
#define ROW_SIZE 9
#define COLUMN_SIZE 9
#define NO_OF_MINES 10

class MineBoard{
    bool mine[ROW_SIZE][COLUMN_SIZE];
    public:
    MineBoard(){
        for(int i=0; i< ROW_SIZE; i++)
            for(int j=0; j< COLUMN_SIZE; j++)
                mine[i][j] = false;
    }
    void generateMines(){
        int count = 0;
        srand(time(0));
        while(count < NO_OF_MINES){
            int i = rand()%ROW_SIZE;
            int j = rand()%COLUMN_SIZE;
            if(mine[i][j])
                continue;
            mine[i][j] = true;
            count++;
        }
    }
    bool isMine(int row, int col){
        return mine[row][col];
    }
    int adjacentMines(int row, int col){
        int count = 0;
        for(int i=-1; i<=1; i++)
            for(int j=-1; j<=1; j++){
                if(i==0 && j==0)
                    continue;
                if(validLocation(row+i, col+j) && isMine(row, col))
                    count++;
            }
        return count;
    }
    bool validLocation(int i, int j){
        if(i>=0 && j>=0 && i<ROW_SIZE && j<COLUMN_SIZE)
            return true;
        return false;
    }
};

class MineSweeper{
    MineBoard mineBoard;
    char board[ROW_SIZE][COLUMN_SIZE];
    bool minesGenerated;
    bool gameOver;
    public:
    MineSweeper(){
        for(int i=0; i<ROW_SIZE; i++)
            for(int j=0; j<COLUMN_SIZE; j++)
                board[i][j] = '-';
        minesGenerated = false;
        gameOver = false;
    }
    void makeMove(int row, int col){
        if(!minesGenerated)
            mineBoard.generateMines();
        else if(mineBoard.isMine(row, col)){
            gameOver = true;
            return;
        }
        checkAdjacentMines(row, col);
    }
    void checkAdjacentMines(int row, int col){
        int mines = mineBoard.adjacentMines(row, col);
    }
    bool isMine(int row, int column){

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

class GameManager{
    bool gameOver;
    int row, col;
    MineSweeper mineSweeper;
    public:
    void startGame(){
        cout << "Welcome to MineSweeper\n";
    }
    void startGameLoop(){
        gameOver = false;
        while(!gameOver){
            cout << "Enter Row and Column: ";
            cin >> row >> col;
            if(!validLocation(row, col)){
                cout << "Invalid Row or Column!\n";
                continue;
            }
            mineSweeper.makeMove(row, col);
            mineSweeper.printBoard();
        }
    }
    bool validLocation(int i, int j){
        if(i>=0 && j>=0 && i<ROW_SIZE && j<COLUMN_SIZE)
            return true;
        return false;
    }
};

int main(){
    GameManager game;
    game.startGame();
    return 0;
}