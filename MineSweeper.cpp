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
    void generateMines(int row, int col){
        int count = 0;
        int i,j;
        mine[row][col] = true;
        srand(time(0));
        while(count < NO_OF_MINES){
            i = rand()%ROW_SIZE;
            j = rand()%COLUMN_SIZE;
            if(mine[i][j])
                continue;
            mine[i][j] = true;
            count++;
        }
        mine[row][col] = false;
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
                if(validLocation(row+i, col+j) && isMine(row+i, col+j))
                    count++;
            }
        return count;
    }
    bool validLocation(int i, int j){
        if(i>=0 && j>=0 && i<ROW_SIZE && j<COLUMN_SIZE)
            return true;
        return false;
    }
    void printMineBoard(){
        for(int i=0; i<ROW_SIZE; i++){
            for(int j=0; j<COLUMN_SIZE; j++){
                cout << mine[i][j] << " ";
            }
            cout << endl;
        }
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
        if(!minesGenerated){
            mineBoard.generateMines(row, col);
            mineBoard.printMineBoard();
            minesGenerated = true;
        }
        else if(mineBoard.isMine(row, col)){
            gameOver = true;
            return;
        }
        revealLocation(row, col);
    }
    void revealLocation(int row, int col){
        //cout << "row:" << row << " col:" << col << endl;
        if(board[row][col] != '-')
            return;
        int mines = mineBoard.adjacentMines(row, col);
        //cout << "Mines found: " << mines << endl;
        //board[row][col] = '0' + mines;
        if(mines){
            board[row][col] = '0' + mines;
            return;
        }
        board[row][col] = ' ';
        for(int i=-1; i<=1; i++)
                for(int j=-1; j<=1; j++){
                    if(i==0 && j==0)
                        continue;
                    if(validLocation(row+i, col+j))
                        revealLocation(row+i, col+j);
            }
    }
    void printBoard(){
        cout << "    0 1 2 3 4 5 6 7 8\n";
        cout << "    _ _ _ _ _ _ _ _ _\n";
        for(int i=0; i<ROW_SIZE; i++){
            cout << i << " | ";
            for(int j=0; j<COLUMN_SIZE; j++){
                cout << board[i][j] << " ";
            }
            cout << endl;
        }
        
    }
    bool validLocation(int i, int j){
        if(i>=0 && j>=0 && i<ROW_SIZE && j<COLUMN_SIZE)
            return true;
        return false;
    }
    bool isGameOver(){
        return gameOver;
    }
    void testGame(){
        mineBoard.generateMines(0,0);
        mineBoard.printMineBoard();
    }
};

class GameManager{
    bool gameOver;
    int row, col;
    MineSweeper mineSweeper;
    public:
    void startGame(){
        cout << "Welcome to MineSweeper\n";
        mineSweeper.printBoard();
        startGameLoop();
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
            gameOver = mineSweeper.isGameOver();
        }
    }
    bool validLocation(int i, int j){
        if(i>=0 && j>=0 && i<ROW_SIZE && j<COLUMN_SIZE)
            return true;
        return false;
    }
    void testGame(){
        mineSweeper.testGame();
    }
};

int main(){
    GameManager game;
    //game.testGame();
    game.startGame();
    return 0;
}