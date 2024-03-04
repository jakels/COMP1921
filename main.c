// COMP 1921 _ Maze Game Coursework 1 (Due 29th February 2024)
/* ----Specification -----------------------------------------
 * When provided with a file containing ascii data in the shape
 * of a maze we need to be able to load in the file and read
 * the data before then displaying the maze to the screen.
 *
 * Our maze file has several unique characters representing
 * different types of objects in our game environment:
 *
 * '#' = Wall, impassable
 * ' ' = Path, traversable
 * 'S' = Start point for player character
 * 'E' = Maze end point for player character (goal)
 *
 * Our maze also has several restraints on its characteristics:
 * - The maze dimensions can not exceed 100 x or 100 y
 * - The maze dimensions can not be less than 5 x or 5 y
 * - Height and width does not have to be equal
 * - All rows should be the same size
 * - All columns should be the same size
 *
 * Our player character has certain characteristics:
 * - Move with W/A/S/D
 * - Player cannot move past walls
 * - Player cannot move outside maze dimensions
 * - Player can press M to see image of map
 * - Player shown by an 'X'
 * - Upon reaching 'E' game closes and shows win message
 *
 * ----End of specification-----------------------------------
 */

// ----Libraries----------------------------------------------
#include <stdio.h>

// ----Struct Declarations------------------------------------
typedef struct { // Player character struct
    int x;       // X position of the player
    int y;       // Y position of the player
} PLAYER;

typedef struct { // Loaded maze struct
    FILE *file;  // File object for the maze
    char *name;  // Loaded maze data (ASCII characters)
} MAZE;

// ----Function Declarations----------------------------------
FILE* OpenFile (const char filename);
MAZE* LoadMaze (const FILE *mazeFileObject);
int GameplayLoop(const MAZE *mazeStruct);
int GameplayStep(MAZE *mazeStruct);
int DisplayMaze (const MAZE *mazeStruct);
int MazeIsValid(const FILE *mazeFileObject);
int ProcessPlayerActions(const MAZE *mazeStruct);
int CheckForWinCondition(MAZE *mazeStruct);
int DisplayWinMessage();

// ----Functions----------------------------------------------
int main( int argc, char *argv[] ) // main() function will hold the main execution line of our game, we will also take in the command line arguments for the maze file
{
    // Check our CL arguments are valid
    if(argc > 2) // The first CL argument is the application name
    {
        perror("Too many arguments! (expected 1)");
        return 1;
    }

    if(argc < 2) // The first CL argument is the application name
    {
        perror("Too few arguments! (expected 1)");
        return 1;
    }

    // Declare array to store user provided maze filename in
    char mazeFilename = *argv[1];

    // Load the requested maze data file into a file object
    FILE *mazeFileObject = OpenFile(mazeFilename);
    if(mazeFileObject == NULL) // Will equal to null if we failed to open the file
    {
        perror("Failed to open maze file!");
        return 1;
    }

    // Check if maze is of a valid configuration
    int mazeIsValid = MazeIsValid(mazeFileObject);
    if(mazeIsValid == 1) // 0 = valid, 1 = invalid
    {
        perror("Maze is not of a valid configuration!");
        return 1;
    }

    // Load our maze into a maze struct for use in our gameplay functions
    MAZE *loadedMazeResult = LoadMaze(mazeFileObject);
    if(loadedMazeResult == NULL) // Will equal to null if we failed to compile maze into a MAZE struct
    {
        perror("Failed to load maze into a struct!");
        return 1;
    }

    // Before starting gameplay loop and getting user input show the maze in its initial configuration
    DisplayMaze(loadedMazeResult);

    // Start the gameplay loop and accepting user input
    GameplayLoop(loadedMazeResult);

    return 0; // Return success on game finished
}

// ----Gameplay Functions-------------------------------------
int GameplayLoop(const MAZE *mazeStruct)
{
    int gameActive = 1; // Game active "boolean" if 0 false, if 1 true

    // Repeatedly call gameplay steps until game is no longer active
    while (gameActive == 1)
    {
        gameActive = GameplayStep(mazeStruct);
    }

    DisplayWinMessage();

    return 0;
}

int DisplayWinMessage()
{
    // TODO: Show a win message
    return 0;
}

int GameplayStep(MAZE *mazeStruct)
{
    // Process player movement & map display
    ProcessPlayerActions(mazeStruct);

    // Check if player has met a win condition
    int hasWon = CheckForWinCondition(mazeStruct);

    return hasWon;
}

int ProcessPlayerActions(const MAZE *mazeStruct)
{
    // TODO: Implement the process of accepting user input and then changing the player position if possible

    // W - UP, A - LEFT, S - DOWN, D - RIGHT, M - SHOW MAP
    char option;
    printf("Enter one of the following options: W, A, S, D, or X: ");
    scanf(" %c", &option);

    switch (option) {
        case 'W':
        case 'w':
            // Try to move up
            break;
        case 'A':
        case 'a':
            // Try to move left
            break;
        case 'S':
        case 's':
            // Try to move down
            break;
        case 'D':
        case 'd':
            // Try to move right
            break;
        case 'X':
        case 'x':
            DisplayMaze(mazeStruct);
            break;
        default:
            printf("Please choose an option that is either W/A/S/D or X!\n");
    }
    return 0;
}

int CheckForWinCondition(MAZE *mazeStruct)
{
    // TODO: Implement the process of checking for a win condition i.e. player meets end of maze
    return 0;
}

// ----Maze Functions-----------------------------------------
int MazeIsValid(const FILE *mazeFileObject)
{
    // TODO: Check if a provided maze is valid:
    /* Maze must be less than or equal to a size of 100x100
     * Maze must be greater than or equal to a size of 5x5
     * Maze rows should be uniform, maze columns should be uniform
     * Maze should contain an S character
     */
    return 0;
}

MAZE* LoadMaze (const FILE *mazeFileObject) // Loads the maze data into a maze struct from our maze file object
{
    // TODO: Load maze into struct
    MAZE *newMaze;
    // Struct construction here...
    return newMaze;
}

int DisplayMaze (const MAZE *mazeStruct)
{
    // TODO: Display maze to user with player character overdrawn
    return 0;
}

// ----Utility Functions--------------------------------------
FILE* OpenFile (const char filename) // Takes a file name and returns a file object for that filename
{
    FILE *fileObject = fopen(&filename, "r"); // Open filename in read mode

    if (fileObject == NULL)
    {
        perror("Error opening file!");
        return NULL; // Return NULL if the file open fails
    }

    return fileObject; // If successful then return the file object
}

