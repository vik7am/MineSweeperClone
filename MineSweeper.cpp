#include <iostream>
using namespace std;
#define ROW_SIZE 9
#define COLUMN_SIZE 9
#define NO_OF_MINES 10
#define HIDDEN_CELL '-'
#define FLAGGED_CELL '#'

class Utils{
    public:
    bool validLocation(int row, int col){
        if(row>=0 && col>=0 && row<ROW_SIZE && col<COLUMN_SIZE)
            return true;
        return false;
    }
};

class MineBoard : Utils{
    bool mine[ROW_SIZE][COLUMN_SIZE];

    public:
    void resetMineBoard(){
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

    int getAdjacentMines(int row, int col){
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
};

class MineSweeper : Utils{
    MineBoard mineBoard;
    char board[ROW_SIZE][COLUMN_SIZE];
    bool minesGenerated;
    int hiddenMines;
    int hiddenCells;

    public:
    void resetMineSweeper(){
        mineBoard.resetMineBoard();
        minesGenerated = false;
        hiddenMines = NO_OF_MINES;
        hiddenCells = (ROW_SIZE * COLUMN_SIZE) - NO_OF_MINES;
        for(int i=0; i<ROW_SIZE; i++)
            for(int j=0; j<COLUMN_SIZE; j++)
                board[i][j] = HIDDEN_CELL;
    }

    bool isGameLost(){
        return hiddenMines < NO_OF_MINES;
    }

    bool isGameWon(){
        return hiddenCells == 0;
    }

    void makeMove(int row, int col){
        if(!minesGenerated){
            mineBoard.generateMines(row, col);
            minesGenerated = true;
        }
        else if(board[row][col] == FLAGGED_CELL){
            cout << "Remove Flag to open cell!\n";
            return;
        }
        else if(mineBoard.isMine(row, col)){
            hiddenMines--;
            return;
        }
        openCell(row, col);
    }

    void openCell(int row, int col){
        if(board[row][col] != HIDDEN_CELL && board[row][col] != FLAGGED_CELL)
            return;
        int adjacentMines = mineBoard.getAdjacentMines(row, col);
        hiddenCells--;
        if(adjacentMines > 0){
            board[row][col] = '0' + adjacentMines;
            return;
        }
        board[row][col] = ' ';
        for(int i=-1; i<=1; i++)
            for(int j=-1; j<=1; j++){
                if(i==0 && j==0)
                    continue;
                if(validLocation(row+i, col+j))
                    openCell(row+i, col+j);
        }
    }

    void placeFlag(int row, int col){
        if(board[row][col] == HIDDEN_CELL)
            board[row][col] = FLAGGED_CELL;
        else if(board[row][col] == FLAGGED_CELL)
            board[row][col] = HIDDEN_CELL;
    }

    void showAllMines(){
        for(int i=0; i< ROW_SIZE; i++)
            for(int j=0; j< COLUMN_SIZE; j++)
                if(mineBoard.isMine(i,j))
                    board[i][j] = '*';
    }

    void printBoard(){
        cout << "\n    1 2 3 4 5 6 7 8 9\n";
        cout << "  =====================\n";
        for(int i=0; i<ROW_SIZE; i++){
            cout << i+1 << " | ";
            for(int j=0; j<COLUMN_SIZE; j++)
                cout << board[i][j] << " ";
            cout << "|" << endl;
        }
        cout << "  =====================\n";
    }
};

class GameManager : Utils{
    MineSweeper mineSweeper;
    int row, col, choice;
    bool gameOver;

    public:
    void startGame(){
        do{
            cout << "\nWelcome to MineSweeper\n";
            cout << "1. Start New Game\n2. Rules and Info\n3. Exit\nYour Choice: ";
            cin >> choice;
            switch(choice){
                case 1: startGameLoop(); break;
                case 2: showRulesAndInfo(); break;
                case 3: break;
                default: cout << "Invalid Option!\n";
            }
        }while(choice != 3);
    }

    void startGameLoop(){
        mineSweeper.resetMineSweeper();
        mineSweeper.printBoard();
        gameOver = false;
        while(!gameOver){
            cout << "Enter Row and Column number: ";
            cin >> row >> col;
            if(validLocation(row-1, col-1))
                mineSweeper.makeMove(row-1, col-1);
            else if(validLocation(-row-1, -col-1))
                mineSweeper.placeFlag(-row-1, -col-1);
            else{
                cout << "Invalid Row or Column!\n";
                continue;
            }
            if(mineSweeper.isGameWon() || mineSweeper.isGameLost())
                gameOver = true;
            else
                mineSweeper.printBoard();
        }
        if(mineSweeper.isGameWon()){
            mineSweeper.printBoard();
            cout << "You Won!\n";
        }
        else{
            mineSweeper.showAllMines();
            mineSweeper.printBoard();
            cout << "You Lost!\n";
        }
        cout << "Press any key to continue.. ";
        cin >> choice;
    }

    void showRulesAndInfo(){
        cout << "\n   Rules\n";
        cout << "1. The board is divided into cells, with mines randomly distributed.\n";
        cout << "2. To win, you need to open all the cells.\n";
        cout << "3. The number on a cell shows the number of mines adjacent to it. Using this information, you can determine cells that are safe, and cells that contain mines.\n";
        cout << "4. Interact, evolve and enjoy!\n";

        cout << "\n   How to Play\n";
        cout << "1. Player will Input the row and column of the cell which they want to open.\n";
        cout << "2. Player can place or remove flags on hidden cells by adding a (-) sign to row and column values.\n";
        cout << "\nPress any key to continue..";
        cin >> choice;
    }
};

int main(){
    GameManager game;
    game.startGame();
    return 0;
}