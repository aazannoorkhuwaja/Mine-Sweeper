#include <iostream>
#include <time.h>
using namespace std;

const int WIDTH = 5;
const int HEIGHT = 5;
const int TOTAL_MINES = 3;
const char COVERED_BOX = '#';
const char UNCOVERED_BOX = ' ';
const char MINE_BOX = '*';

void incrementToCell(int state[HEIGHT][WIDTH], int mineX, int mineY)
{
    if (state[mineY][mineX] == MINE_BOX)
        return;
    state[mineY][mineX]++;
}
void incrementToMineNeighbors(int state[HEIGHT][WIDTH], int mineX, int mineY)
{
    // top cell
    if (mineY > 0)
    {
        incrementToCell(state, mineX, mineY - 1);
        // top left
        if (mineX > 0)
            incrementToCell(state, mineX - 1, mineY - 1);
        // top right
        if (mineX < WIDTH - 1)
            incrementToCell(state, mineX + 1, mineY - 1);
    }
    // left cell
    if (mineX > 0)
        incrementToCell(state, mineX - 1, mineY);
    // bottom cell
    if (mineY < HEIGHT - 1)
    {
        incrementToCell(state, mineX, mineY + 1);
        // bottom left
        if (mineX > 0)
            incrementToCell(state, mineX - 1, mineY + 1);
        // bottom right
        if (mineX < WIDTH - 1)
            incrementToCell(state, mineX + 1, mineY + 1);
    }
    // right cell
    if (mineX < WIDTH - 1)
        incrementToCell(state, mineX + 1, mineY);
}
void initializeGameState(int state[HEIGHT][WIDTH],int currentGameState[HEIGHT][WIDTH])
{
    // clear state and set current game state
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
            state[i][j] = '\0',
            currentGameState[i][j] = COVERED_BOX;
    }

    // put mines
    for (int i = 0; i < TOTAL_MINES; i++)
    {
        int mineX = rand() % WIDTH;
        int mineY = rand() % HEIGHT;
        if (state[mineY][mineX] == MINE_BOX)
        {
            // Mine already exists, Generate new coord for this mine
            i--;
            continue;
        }

        state[mineY][mineX] = MINE_BOX;

        incrementToMineNeighbors(state, mineX, mineY);
    }
}

char getCellRepresentation(int cellState)
{
    if (cellState == 0)
        return UNCOVERED_BOX;
    if (cellState >= 1 && cellState <= 8)
        return char(cellState + 48);

    return char(cellState);
}

void printXCoordinates(){

    for (int i = 0; i < WIDTH; i++)
        cout << "--";
    cout << "------" << endl;
    cout << "  |";
    for (int i = 0; i < WIDTH; i++)
        cout << " " << i / 10;
    cout << "|" << endl;

    cout << "  |";
    for (int i = 0; i < WIDTH; i++)
        cout << " " << i % 10;
    cout << "|" << endl;

    for (int i = 0; i < WIDTH; i++)
        cout << "--";
    cout << "------" << endl;
}
void renderGameState(int state[HEIGHT][WIDTH])
{
    printXCoordinates();

    for (int i = 0; i < HEIGHT; i++)
    {
        cout << (i > 9 ? "" : " ") << i << "|";
        for (int j = 0; j < WIDTH; j++)
            cout << ' ' << getCellRepresentation(state[i][j]);
        cout << "|" << i << endl;
    }

    printXCoordinates();
}

void revealBoxes(int x, int y,bool visited[HEIGHT][WIDTH], int state[HEIGHT][WIDTH], int currGameState[HEIGHT][WIDTH],int * safe_boxes){
    for(int i = -1; i < 2; i++){
        int curr_y = y+i;
        for(int j = -1 ; j < 2; j++){
            int curr_x = x+j;
            if(curr_y != -1 && curr_x !=-1 && curr_y < HEIGHT && curr_x< WIDTH && !visited[curr_y][curr_x]){
            visited[curr_y][curr_x] = true;
               
                if(state[curr_y][curr_x] == 0){
                    currGameState[curr_y][curr_x] = 0;
                    (*safe_boxes)--;
                    revealBoxes(curr_x,curr_y,visited,state,currGameState,safe_boxes);
                }else{
                    (*safe_boxes)--;
                    currGameState[curr_y][curr_x] = state[curr_y][curr_x];
                }
                    
            }
        }
    }

}
int onUserInput(int x, int y,int state[HEIGHT][WIDTH],int currGameState[HEIGHT][WIDTH], int * safe_boxes){
    int gameOutput = 0;

    if(x < 0 || y < 0 || x >= WIDTH || y >= HEIGHT){
        cout<<"Coordinate out of bounds"<<endl;
        return 0;
    }
    int curr_box = state[y][x];

        bool visited[HEIGHT][WIDTH];

        for(int i = 0; i < HEIGHT;i++){
            for(int j = 0; j < WIDTH; j++){
                visited[i][j] = false;
            }
        }
    switch (curr_box)
    {
    case MINE_BOX:
        gameOutput = -1;
        break;

    case 0:
        currGameState[y][x] = 0;
        (*safe_boxes)--;
        visited[y][x] = true;
        revealBoxes(x,y,visited,state,currGameState, safe_boxes);
        break;
    default:
    currGameState[y][x] = state[y][x];
        (*safe_boxes)--;
        break;
    }

    if(*safe_boxes == 0) gameOutput = 1;
    return gameOutput;
}

int main()
{
    srand(time(0));
    int gameState[HEIGHT][WIDTH];
    int currGameState[HEIGHT][WIDTH];
    int safe_boxes = HEIGHT*WIDTH-TOTAL_MINES;

    initializeGameState(gameState,currGameState);

    bool running = true;

    while (running)
    {

        renderGameState(currGameState);

        int x, y;

        cout << "Enter Coordinates (x y): ";
        cin >> x >> y;
        int gameOutput = onUserInput(x,y,gameState,currGameState,&safe_boxes);
        if( gameOutput == 0 ) continue;
        else{
            running = false;
            if(gameOutput == 1)
                cout<<"You won"<<endl;
            else if(gameOutput == -1)
                cout <<"You lost"<<endl;
            renderGameState(gameState);
        }
    }

    return 0;
}