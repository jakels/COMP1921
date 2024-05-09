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

/* ----Implementation ----------------------------------------
 * This program takes 3 arguments
 * Arg 1. this arg is always just the filename of the program (filepath)
 * Arg 2. this is the maze we want to play (file path)
 * Arg 3. debug maze loading - used for testing maze loading functionality (true or false)
 */

// ----Libraries----------------------------------------------
#include <stdio.h> // Standard input output
#include <string.h> // String library
#include <stdlib.h> // Standard library

// Define maximum and minimum maze dimensions
#define MAX_MAZE 100 // Maximum width or height of a maze
#define MIN_MAZE 5 // Minimum width or height of a maze

// Define boolean values
#define bool int // Create a fake type of bool
#define true 1 // Create a fake value of true
#define false 0 // Create a fake value of false

// Define maze processing values
#define MAZE_VALIDITY int // Create a fake type of MAZE_VALIDITY
#define MAZE_IS_VALID 0 // Create a fake value of MAZE_IS_VALID
#define MAZE_IS_INVALID 1 // Create a fake value of MAZE_IS_INVALID

// Define function return codes
#define FUNC_SUCCESS 0 // Create a fake return value of FUNC_SUCCESS
#define FUNC_FAILURE 1 // Create a fake return value of FUNC_FAILURE

// Define exit codes specified by CW specification
#define EXIT_SUCCESS 0 // Define successful exit code
#define EXIT_ARG_ERROR 1 // Define argument error exit code
#define EXIT_FILE_ERROR 2 // Define file error exit code
#define EXIT_MAZE_ERROR 3 // Define maze error exit code

// Misc definitions
#define PLAYER_CHAR 'X' // Define the character used as the player

// Debugging definitions
#define DEBUG_MODE false // When set to true - displays messages sent to DebugLog to the standard output

// ----Struct Declarations------------------------------------
typedef struct { // Player struct for storing information relating to the player
    int x;       // X position of the player
    int y;       // Y position of the player
} PLAYER;

typedef struct { // Maze struct for storing processed maze information gathered from a raw maze file
    FILE *file;  // File object that the maze was loaded from
    int mazeWidth; // The width of the maze
    int mazeHeight; // The height of the maze
    char *data; // The maze data i.e. the objects that compose the maze, walls, start, end, path etc
} MAZE;

typedef struct { // Maze detail struct for holding info gathered during the processing stage of opening a maze
    int mazeWidth; // The width of the processed maze
    int mazeHeight; // The height of the processed maze
    int totalCharacters; // The total number of characters in the maze data
    MAZE_VALIDITY mazeValidity; // The validity of the maze
    int startPointX; // The X coordinate of the start point
    int startPointY; // The Y coordinate of the start point
} MAZE_PROCESSING_DETAILS;

// ----Function Declarations----------------------------------
FILE* OpenFile (const char* filename);
MAZE* LoadMaze (const FILE *mazeFileObject, int mazeWidth, int mazeHeight, int totalCharacters);
int GameplayLoop(const MAZE *mazeStruct);
int GameplayStep(MAZE *mazeStruct);
int DisplayMaze (const MAZE *mazeStruct);
MAZE_PROCESSING_DETAILS* MazeIsValid(const FILE *mazeFileObject);
int ProcessPlayerActions(const MAZE *mazeStruct);
int CheckForWinCondition(MAZE *mazeStruct);
int DisplayWinMessage();
int ExitProgram();
void DebugLog(char* message);
char GetObjetAtPosition (const MAZE *mazeStruct, int x, int y);

// Global variables
PLAYER player;

// ----Functions----------------------------------------------
int main( int argc, char *argv[] ) // main() function will hold the main execution line of our game, we will also take in the command line arguments for the maze file
{
    // Initialise player x & y coordinates
    player.x = 0;
    player.y = 0;

    // Check our CL arguments are valid, argv[0] is always the application name
    if(argc < 2) // Less than two arguments, too few
    {
        perror("Too few arguments! (expected a minimum of 1 user argument)");
        return EXIT_ARG_ERROR; // Return 1 : Argument Error
    }
    if(3 < argc) // More than 3 arguments, too many
    {
        perror("Too many arguments! (expected a maximum of 2 user arguments)");
        return EXIT_ARG_ERROR; // Return 1 : Argument Error
    }

    // Declare array to store user provided maze filename in
    char* mazeFilename = argv[1];
    // Declare array to store user provided debug setting for maze loading
    char* debugMazeLoading = "false";
    if(argc == 3) // If we have arg 3 then set debug maze loading to that argument
    {
        debugMazeLoading = argv[2]; // If this = "true" then halt execution after maze loading
        DebugLog("Debug maze loading arg received");
    }

    DebugLog("Opening maze file...");

    // Load the requested maze data file into a file object
    FILE *mazeFileObject = OpenFile(mazeFilename);
    if(mazeFileObject == NULL) // Will equal to null if we failed to open the file
    {
        perror("Failed to open maze file!");
        return EXIT_FILE_ERROR; // Return 2 : File Error
    }

    DebugLog("Validating maze and gathering maze details...");

    // Check if maze is of a valid configuration
    MAZE_PROCESSING_DETAILS* mazeDetails = MazeIsValid(mazeFileObject);
    if(mazeDetails->mazeValidity == MAZE_IS_INVALID) // 0 = valid, 1 = invalid
    {
        perror("Maze is not of a valid configuration!");
        return EXIT_MAZE_ERROR; // Return 3 : Maze Error
    }

    // Set player position to be at the start point
    player.x = mazeDetails->startPointX;
    player.y = mazeDetails->startPointY;

    DebugLog("Processing maze data into struct...");

    // Process our raw maze data file into a usable maze struct
    MAZE *loadedMazeResult = LoadMaze(mazeFileObject, mazeDetails->mazeWidth, mazeDetails->mazeHeight, mazeDetails->totalCharacters);
    if(loadedMazeResult == NULL) // Will equal to null if we failed to compile maze into a MAZE struct
    {
        perror("Failed to load maze into a struct!");
        return EXIT_MAZE_ERROR; // Return 3 : Maze Error
    }

    // Halt execution if the debug maze loading flag is true
    if(strcmp(debugMazeLoading, "true") == 0)
    {
        DebugLog("Aborted process prematurely due to debug maze loading argument being set to 'true'");
        printf("Maze loaded successfully");
        return EXIT_SUCCESS; // Return 0 : Successful run
    }

    // Before starting gameplay loop and getting user input show the maze in its initial configuration
    DisplayMaze(loadedMazeResult);
    // Start the gameplay loop and accepting user input
    GameplayLoop(loadedMazeResult);

    // Upon the end of the game return success exit code
    return EXIT_SUCCESS; // Return 0 : Successful run
}

// ----Debugging Functions------------------------------------
void DebugLog(char* message) // Similar to printf but only prints when debug mode flag is true
{
    if(DEBUG_MODE == false){ return; } // Don't run if debug mode flag is false
    printf("[DEBUG] %s\n", message); // Print debug message to screen with [DEBUG] prefix
}

// ----Gameplay Functions-------------------------------------
int GameplayLoop(const MAZE *mazeStruct)
{
    DebugLog("Starting gameplay loop...");

    int gameActive = 1; // Game active "boolean" if 0 false, if 1 true

    // Repeatedly call gameplay steps until game is no longer active
    while (gameActive == 1)
    {
        gameActive = GameplayStep(mazeStruct);
    }

    DisplayWinMessage();

    return FUNC_SUCCESS; // Return 0 : Func completed successfully
}

int DisplayWinMessage()
{
    printf("You won!\n");
    return FUNC_SUCCESS; // Return 0 : Func completed successfully
}

int GameplayStep(MAZE *mazeStruct)
{
    // Process player movement & map display
    ProcessPlayerActions(mazeStruct);

    // Check if player has met a win condition
    int hasWon = CheckForWinCondition(mazeStruct);

    return hasWon;
}

int MovePlayer(int xDelta, int yDelta, const MAZE *mazeStruct)
{
    if(GetObjetAtPosition(mazeStruct, player.x + xDelta, player.y + yDelta) == '#')
    {
        printf("Wall!\n");
    }
    else if((player.y <= 0 && yDelta < 0) || (player.x <= 0 && xDelta < 0) || (player.y >= mazeStruct->mazeHeight && yDelta > 0) || (player.x >= mazeStruct->mazeWidth - 1 && xDelta > 0))
    {
        printf("Bounds!\n");
    }
    else
    {
        player.x += xDelta;
        player.y += yDelta;
    }

    return FUNC_SUCCESS;
}

int ProcessPlayerActions(const MAZE *mazeStruct)
{
    // W - UP, A - LEFT, S - DOWN, D - RIGHT, M - SHOW MAP, E - EXIT PROCESS
    char option;
    printf("Enter one of the following options: W (up), A (left), S (down), D (right), M (map) or E (exit): ");
    scanf(" %c", &option);
    printf("\n");
    switch (option) {
        case 'W':
        case 'w':
            // Try to move up
            MovePlayer(0, -1, mazeStruct);
            break;
        case 'A':
        case 'a':
            // Try to move left
            MovePlayer(-1, 0, mazeStruct);
            break;
        case 'S':
        case 's':
            // Try to move down
            MovePlayer(0, 1, mazeStruct);
            break;
        case 'D':
        case 'd':
            // Try to move right
            MovePlayer(1, 0, mazeStruct);
            break;
        case 'M':
        case 'm':
            DisplayMaze(mazeStruct);
            break;
        case 'E':
        case 'e':
            ExitProgram();
            break;
        default:
            printf("Please choose an option that is either W/A/S/D or M!\n");
    }
    return FUNC_SUCCESS; // Return 0 : Func completed successfully
}

int ExitProgram()
{
    exit(EXIT_SUCCESS);
}

int CheckForWinCondition(MAZE *mazeStruct)
{
    if(GetObjetAtPosition(mazeStruct, player.x, player.y) == 'E')
    {
        return 0;
    }
    return 1;
}

// ----Maze Functions-----------------------------------------
MAZE_PROCESSING_DETAILS* MazeIsValid(const FILE *mazeFileObject)
{
    // Setup new maze report
    MAZE_PROCESSING_DETAILS *report = malloc(sizeof(MAZE_PROCESSING_DETAILS));

    // Counters & Booleans for validating maze adheres to the CW spec
    int startPoints = 0; // Number of start points (S characters)
    int exitPoints = 0; // Number of exit points (E characters)

    bool allCharactersAreValid = true; // Boolean that evaluates if all the characters in the file are valid

    int mazeHeight = 0; // Maze height
    int currentRowWidth = 0; // The width of the current row
    int mazeWidth = 0; // The width we are going to compare all subsequent rows to it is the width of the first row
    int totalCharacters = 0;


    // Check if file is not NULL
    if (mazeFileObject == NULL)
    {
        DebugLog("Maze file is null!");
        report->mazeValidity = MAZE_IS_INVALID;
        return report;
    }

    // Loop over every character in our file until we reach EOF end of file
    int newLineCount = 0;
    int characterStream; // This is the current character being evaluated
    while ((characterStream = fgetc((FILE *)mazeFileObject)) != EOF)
    {
        totalCharacters++;
        // Count start and exit points
        if (characterStream == 'S')
        {
            report->startPointY = newLineCount;
            report->startPointX = currentRowWidth;
            startPoints++;
        }
        else if (characterStream == 'E')
        {
            exitPoints++;
        }
        else if (characterStream != ' ' && characterStream != '#' && characterStream != '\n')
        {
            allCharactersAreValid = false;
        }

        // Handle newline characters
        if (characterStream == '\n')
        {
            newLineCount++;
            if (mazeHeight == 0)
            {
                // Set the width of the first row
                mazeWidth = currentRowWidth;
            }
            else
            {
                // Check if the current row width matches the first row's width
                if (currentRowWidth != mazeWidth)
                {
                    printf("Invalid dimensions crw : %i != srw %i\n", currentRowWidth, mazeWidth);
                    report->mazeValidity = MAZE_IS_INVALID;
                    return report;
                }
            }
            mazeHeight++;
            currentRowWidth = 0;
        }
        else
        {
            currentRowWidth++;
        }
    }

    // If there is no trailing new line then we need to add one to the height value artificially
    if (currentRowWidth > 0)
    {
        if (currentRowWidth != mazeWidth)
        {
            report->mazeValidity = MAZE_IS_INVALID;
            return report;
        }
        mazeHeight++;
    }

    // Check the required counts and sizes
    if (startPoints != 1 || exitPoints != 1)
    {
        printf("Start or exit counts\n");
        report->mazeValidity = MAZE_IS_INVALID;
        return report;
    }
    if (allCharactersAreValid == false)
    {
        printf("Invalid characters\n");
        report->mazeValidity = MAZE_IS_INVALID;
        return report;
    }
    if (mazeHeight < MIN_MAZE || mazeHeight > MAX_MAZE || mazeWidth < MIN_MAZE || mazeWidth > MAX_MAZE)
    {
        printf("Too small or too big\n");
        report->mazeValidity = MAZE_IS_INVALID;
        return report;
    }

    // If all conditions are met, return maze valid
    DebugLog("Maze valid report generating...");
    report->mazeValidity = MAZE_IS_VALID;
    report->mazeHeight = mazeHeight;
    report->mazeWidth = mazeWidth;
    report->totalCharacters = totalCharacters;
    DebugLog("Returning report...");
    return report;
}

MAZE* LoadMaze (const FILE *mazeFileObject, int mazeWidth, int mazeHeight, int totalCharacters) // Loads the maze data into a maze struct from our maze file object
{
    MAZE *maze = malloc(sizeof(MAZE));

    // Initialize maze properties
    maze->mazeWidth = mazeWidth;
    maze->mazeHeight = mazeHeight;

    // Allocate space for the maze data
    maze->data = malloc(totalCharacters * sizeof(char));
    if (!maze->data) {
        fclose(mazeFileObject);
        free(maze);
        perror("Failed to allocate memory for maze data\n");
        return NULL;
    }

    // Read the file into the maze data
    rewind(mazeFileObject);
    fread(maze->data, sizeof(char), totalCharacters, mazeFileObject);
    return maze;
}

char GetObjetAtPosition (const MAZE *mazeStruct, int x, int y)
{
    char indexedChar = mazeStruct->data[y * (mazeStruct->mazeWidth + 1) + x];
    return indexedChar;
}

int DisplayMaze (const MAZE *mazeStruct)
{
    if (mazeStruct == NULL || mazeStruct->data == NULL)
    {
        printf("Invalid maze data.\n");
        return FUNC_FAILURE;
    }

    putchar('\n');

    // Loop through each row
    for (int row = 0; row < mazeStruct->mazeHeight; row++) {
        // Loop through each column in the current row
        for (int col = 0; col < mazeStruct->mazeWidth; col++) {
            // Access the character in the 1D array that represents the 2D maze
            char indexedChar = GetObjetAtPosition(mazeStruct, col, row);
            if(player.x == col && player.y == row){
                indexedChar = PLAYER_CHAR;
            }
            printf("%c", indexedChar);
        }
        putchar('\n');  // Print a newline after each row
    }
    putchar('\n');
    //printf("Player %ix%i Maze %ix%i\n", player.x, player.y, mazeStruct->mazeWidth, mazeStruct->mazeHeight);
    return FUNC_SUCCESS; // Assuming FUNC_SUCCESS is defined as 0
}

// ----Utility Functions--------------------------------------
FILE* OpenFile (const char* filename) // Takes a file name and returns a file object for that filename
{
    FILE *fileObject = fopen(filename, "r"); // Open filename in read mode

    if (fileObject == NULL)
    {
        perror("Error opening file!");
        return NULL; // Return NULL if the file open fails
    }

    return fileObject; // If successful then return the file object
}

