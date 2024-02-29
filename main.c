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
 */

// Libraries
#include <stdio.h>

// main() function will hold the main execution line of our game
int main()
{
    printf("Hello, World!\n");
    return 0;
}

