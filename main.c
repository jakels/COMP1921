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
FILE* OpenFile (const char *filename);
MAZE* LoadMaze (const FILE *mazeFileObject);
int GameplayLoop(const MAZE *mazeStruct);
int GameplayStep(MAZE *mazeStruct);
int DisplayMaze (const MAZE *mazeStruct);

// ----Global Variables---------------------------------------
MAZE *currentMaze = NULL;

// ----Functions----------------------------------------------
int main() // main() function will hold the main execution line of our game
{
    // Greeting message
    printf("--COMP 1921: Maze Game Coursework 1-----------\n");

    // Declare array to store user provided maze filename in
    char mazeFilename[100];

    // Prompt the user to enter the filename
    printf("Enter the filename of the maze you wish to load: ");

    // Get the user's input
    scanf("%99s", mazeFilename);

    // Load the requested maze data file into a file object
    FILE *mazeFileObject = OpenFile(mazeFilename);
    if(mazeFileObject == NULL)
    {
        perror("Failed to load maze file object!");
        return 1;
    }

    // Set the current maze variable to our newly loaded maze
    MAZE *loadedMazeResult = LoadMaze(mazeFileObject);
    if(loadedMazeResult == NULL)
    {
        perror("Failed to load maze into a struct!");
        return 1;
    }

    // Before starting gameplay loop and getting user input show the maze in its initial configuration
    DisplayMaze(loadedMazeResult);

    // Start the gameplay loop and accepting user input
    GameplayLoop(loadedMazeResult);

    return 0;
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

    return 0;
}

int GameplayStep(MAZE *mazeStruct)
{
    // TODO: Implement gameplay step logic of taking user input for movement and displaying the map and then processing these
    return 1;
}

// ----Maze Functions-----------------------------------------
MAZE* LoadMaze (const FILE *mazeFileObject) // Loads the maze data into a maze struct from our maze file object
{
    // TODO: Load maze into struct
    MAZE *newMaze;
    return newMaze;
}

int DisplayMaze (const MAZE *mazeStruct)
{
    // TODO: Display maze to user with player character overdrawn
    return 0;
}

// ----Utility Functions--------------------------------------
FILE* OpenFile (const char *filename) // Takes a file name and returns a file object for that filename
{
    FILE *fileObject = fopen(filename, "r"); // Open filename in read mode

    if (fileObject == NULL)
    {
        perror("Error opening file!");
        return NULL; // Return NULL if the file open fails
    }

    return fileObject; // If successful then return the file object
}

