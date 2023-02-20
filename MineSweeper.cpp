#include <iostream>
using namespace std;
#define ROW_SIZE 9
#define COLUMN_SIZE 9
#define NO_OF_MINES 10

enum TileState{HIDDEN, FLAGGED, VISIBLE};
enum TileType{UNKNOWN, EMPTY, NUMBER, MINE};

class Tile{
    public:
    char name;
    short adjacentMines;
    TileState state;
    TileType type;

    void initializeTile(){
        state = HIDDEN;
        type = UNKNOWN;
        name = '-';
        adjacentMines = 0;
    }
};

class Board{
    protected:
    Tile tile[ROW_SIZE][COLUMN_SIZE];

    public:
    void initializeBoard(){
        for(int i=0; i< ROW_SIZE; i++)
            for(int j=0; j< COLUMN_SIZE; j++)
                tile[i][j].initializeTile();
    }

    void updateTileName(int row, int col){
        char name = '-';
        switch(tile[row][col].state){
            case HIDDEN: name = '-'; break;
            case FLAGGED: name = '#'; break;
            case VISIBLE:
                switch(tile[row][col].type){
                    case EMPTY: name = ' '; break;
                    case NUMBER: name = '0' + tile[row][col].adjacentMines; break;
                    case MINE: name = '*'; break;
                }
        }
        tile[row][col].name = name;
    }

    int adjacentTilesWithMines(int row, int col){
        int count = 0;
        for(int i=-1; i<=1; i++)
            for(int j=-1; j<=1; j++){
                if(i==0 && j==0)
                    continue;
                if(validLocation(row+i, col+j))
                    if(tile[row+i][col+j].type == MINE)
                        count++;
            }
        return count;
    }

    bool validLocation(int row, int col){
        if(row>=0 && col>=0 && row<ROW_SIZE && col<COLUMN_SIZE)
            return true;
        return false;
    }

    void printBoard(){
        cout << "\n    1 2 3 4 5 6 7 8 9\n";
        cout << "  =====================\n";
        for(int i=0; i<ROW_SIZE; i++){
            cout << i+1 << " | ";
            for(int j=0; j<COLUMN_SIZE; j++)
                cout << tile[i][j].name << " ";
            cout << "|" << endl;
        }
        cout << "  =====================\n";
    }
};

class MineSweeper : public Board{
    bool minesGenerated;
    int hiddenMines;
    int hiddenTiles;

    public:
    void initializeMineSweeper(){
        initializeBoard();
        minesGenerated = false;
        hiddenMines = NO_OF_MINES;
        hiddenTiles = (ROW_SIZE * COLUMN_SIZE) - NO_OF_MINES;
    }

    void generateMines(int row, int col){
        int count = 0;
        int i,j;
        //Placing mine at the tile selected by player to prevent mine spawn
        tile[row][col].type = MINE; 
        srand(time(0));
        while(count < NO_OF_MINES){
            i = rand()%ROW_SIZE;
            j = rand()%COLUMN_SIZE;
            if(tile[i][j].type == MINE)
                continue;
            tile[i][j].type = MINE;
            count++;
        }
        // removing mine from the tile selected by player
        tile[row][col].type == UNKNOWN;
    }

    void makeMove(int row, int col){
        if(!minesGenerated){
            generateMines(row, col);
            minesGenerated = true;
        }
        else if(tile[row][col].state == FLAGGED){
            cout << "Remove Flag to open tile!\n";
            return;
        }
        else if(tile[row][col].type == MINE){
            hiddenMines--;
            return;
        }
        openTile(row, col);
    }

    void openTile(int row, int col){
        if(tile[row][col].state == VISIBLE)
            return;
        int adjacentMines = adjacentTilesWithMines(row, col);
        hiddenTiles--;
        tile[row][col].state = VISIBLE;
        if(adjacentMines > 0){
            tile[row][col].type = NUMBER;
            tile[row][col].adjacentMines = adjacentMines;
            updateTileName(row, col);
            return;
        }
        tile[row][col].type = EMPTY;
        updateTileName(row, col);
        for(int i=-1; i<=1; i++)
            for(int j=-1; j<=1; j++){
                if(i==0 && j==0)
                    continue;
                if(validLocation(row+i, col+j))
                    openTile(row+i, col+j);
        }
    }

    void toggleFlag(int row, int col){
        if(tile[row][col].state == HIDDEN)
            tile[row][col].state = FLAGGED;
        else if(tile[row][col].state == FLAGGED)
            tile[row][col].state = HIDDEN;
        updateTileName(row, col);
    }

    void showAllMines(){
        for(int i=0; i< ROW_SIZE; i++)
            for(int j=0; j< COLUMN_SIZE; j++)
                if(tile[i][j].type == MINE){
                    tile[i][j].state = VISIBLE;
                    updateTileName(i,j);
                }
    }

    bool isGameLost(){
        return hiddenMines < NO_OF_MINES;
    }

    bool isGameWon(){
        return hiddenTiles == 0;
    }
};

class GameManager{
    MineSweeper mineSweeper;
    int row, col, choice;
    string input;
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
        mineSweeper.initializeMineSweeper();
        mineSweeper.printBoard();
        gameOver = false;
        while(!gameOver){
            cout << "Enter Row and Column number: ";
            cin >> row >> col;
            if(mineSweeper.validLocation(row-1, col-1))
                mineSweeper.makeMove(row-1, col-1);
            else if(mineSweeper.validLocation(-row-1, -col-1))
                mineSweeper.toggleFlag(-row-1, -col-1);
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
        cin >> input;
    }

    void showRulesAndInfo(){
        cout << "\n   Rules\n";
        cout << "1. The board is divided into tiles, with mines randomly distributed.\n";
        cout << "2. To win, you need to open all the tiles.\n";
        cout << "3. The number on a tile shows the number of mines adjacent to it. Using this information, you can determine tiles that are safe, and tiles that contain mines.\n";
        cout << "4. Interact, evolve and enjoy!\n";

        cout << "\n   How to Play\n";
        cout << "1. Player will Input the row and column of the tile which they want to open.\n";
        cout << "2. Player can place or remove flags on hidden tiles by adding a (-) sign to row and column values.\n";
        cout << "\nPress any key to continue..";
        cin >> input;
    }
};

int main(){
    GameManager game;
    game.startGame();
    return 0;
}