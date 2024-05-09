#!/bin/bash
# Testing suite for my COMP1921 Coursework 1

# Global Variables
local valid_input_path="/testing-materials/valid_mazes/user_input_files"
local invalid_input_path="/testing-materials/invalid_mazes/user_input_files"

# Argument test
# - Takes 4 parameters (filename without extension, arguments, user inputs, expected output)
# - Runs the program using arguments
# - Pipes in user input
# - Passes test if output string is what is expected
argument_test()
{
    local c_file_name="$1.c"
    local output_file="$1_output.txt"

    local arguments=$2

    local user_inputs_file="$3"

    local expected_output="$4"

    echo "Running argument test on $c_file_name with expected output of $expected_output, arguments $arguments, and user inputs $user_inputs"

    # Pipe the user inputs from our file into the program and capture the output to the name specified by $output_file
    cat "$user_inputs_file" | "./$1" $arguments > "$output_file"
    if [ $? -ne 0 ]; then
        echo "[!!] $c_file_name's return wasn't 0 (=$?)"
    fi

    # Check if the expected string is in the output
    if grep -q "$expected_output" "$output_file"; then
        echo "Test Pass: '$expected_output' found"
    else
        echo "Test Fail: '$expected_output' not found"
    fi

    # Compare the output to the expected output (exact comparison so not really usable for alot of tests)
    #if diff <(echo -n "$expected_output") "$output_file" > /dev/null; then
    #    echo "Test Pass (Args[$arguments], Inputs[$user_inputs]): Output matches for $c_file_name"
    #    return 0
    #else
    #    echo "Test Fail (Args[$arguments], Inputs[$user_inputs]): Output differs for $c_file_name"
    #    return 1
    #fi
}

# Compilation test
# - Takes 1 parameters (filename without extension)
# - Attempts to compile the file with gcc
# - Passes if compiles successfully
# - Fails if compile fails
compilation_test()
{
    local c_file_name = $1 # Without extension
    
    echo "Performing compilation test for $c_file_name.c"

    # Compile the C program
    gcc "$c_file_name.c" -o "$c_file_name"

    if [ $? -ne 0 ]; then # Check if the previous exit code wasn't 0  (if it wasnt then the program exited unsuccessfully)
        echo "Compilation failed for $c_file_name"
        return 1
    else
        echo "Compilation successful for $c_file_name"
    fi
}

# Begin testing ------------------------------------------------------
compilation_test "main" # Compilation Test

# Maze Loading Functions Testing
# The following 3 tests are performed with the debug maze loading set to true
perform_test "main" "testing-material/valid_mazes/valid_5-5x5.txt true" "Maze loaded successfully" "$valid_input_path/empty.txt" # Confirm valid mazes load properly #1 - uses no user inputs
perform_test "main" "testing-material/valid_mazes/valid_6-10x6.txt true" "Maze loaded successfully" "$valid_input_path/empty.txt" # Confirm valid mazes load properly #2 - uses no user inputs
perform_test "main" "testing-material/valid_mazes/valid_7-15x8.txt true" "Maze loaded successfully" "$valid_input_path/empty.txt" # Confirm valid mazes load properly #2 - uses no user inputs

# Maze Loading Functions Testing for Non Existant Files
# The following 1 tests are performed with the debug maze loading set to true
perform_test "main" "testing-material/valid_mazes/THISFILEDOESNOTEXIST true" "Failed to open maze file!" "$valid_input_path/empty.txt" # Confirm maze files that dont exist return errors - uses no user inputs

# Standard Movement Function Testing
# The following 4 tests are performed with the debug maze loading set to false
perform_test "main" "testing-material/valid_mazes/valid_5-5x5.txt false" "You've successfully finished the maze!" "$valid_input_path/valid_5-basic_path.txt" # Basic small maze making sure movement functionality is correct
perform_test "main" "testing-material/valid_mazes/valid_6-10x6.txt false" "You've successfully finished the maze!" "$valid_input_path/valid_6-basic_path.txt" # Basic larger maze for the same purpose
perform_test "main" "testing-material/valid_mazes/valid_7-15x8.txt false" "You've successfully finished the maze!" "$valid_input_path/valid_7-basic_path.txt" # More complex larger maze with multiple turns for movement testing
perform_test "main" "testing-material/valid_mazes/valid_7-15x8.txt false" "You've successfully finished the maze!" "$valid_input_path/valid_7-complex_path.txt" # Testing all movement directions and going back on self

# Non-Standard Movement Function Testing
# The following 4 tests are performed with the debug maze loading set to false
perform_test "main" "testing-material/valid_mazes/valid_1-15x76.txt false" "You cannot walk out of maze bounds!" "$valid_input_path/valid_1-out-of-bounds-left.txt" # Test to see if we can walk outside of the map bounds left
perform_test "main" "testing-material/valid_mazes/valid_1-15x76.txt false" "You cannot walk out of maze bounds!" "$valid_input_path/valid_1-out-of-bounds-right.txt" # Test to see if we can walk outside of the map bounds right
perform_test "main" "testing-material/valid_mazes/valid_1-15x76.txt false" "You cannot walk out of maze bounds!" "$valid_input_path/valid_1-out-of-bounds-up.txt" # Test to see if we can walk outside of the map bounds up
perform_test "main" "testing-material/valid_mazes/valid_1-15x76.txt false" "You cannot walk out of maze bounds!" "$valid_input_path/valid_1-out-of-bounds-down.txt" # Test to see if we can walk outside of the map bounds down

# Maze Display Functions Testing
# The following 1 tests are performed with the debug maze loading set to false
perform_test "main" "testing-material/valid_mazes/valid_8-100x100.txt false" "$(cat 'testing-material/valid_mazes/valid_8-100x100-GAMEPLAY_VIEW.txt')" "$valid_input_path/valid_8-show_map.txt" # This test opens the map and then exits the program, our output should contain the maze in its entirety with the player where the S used to be

# Maze Loading Functions Testing for Irregular Mazes
# The following 2 tests are performed with the debug maze loading set to true
perform_test "main" "testing-material/invalid_mazes/invalid_5-irregular_height.txt true" "Maze is not of a valid configuration!" "$invalid_input_path/empty.txt" # Confirm that irregular height mazes are classed as invalid
perform_test "main" "testing-material/invalid_mazes/invalid_6-irregular_width.txt true" "Maze is not of a valid configuration!" "$invalid_input_path/empty.txt" # Confirm that irregular width mazes are classed as invalid

# Maze Loading Functions Testing for invalid start/end point configurations
# The following 4 tests are performed with the debug maze loading set to true
perform_test "main" "testing-material/invalid_mazes/invalid_4-multiple_start_points.txt true" "Maze is not of a valid configuration!" "$invalid_input_path/empty.txt" # Confirm that mazes with multiple start points are classed as invalid
perform_test "main" "testing-material/invalid_mazes/invalid_3-no_start_or_end_point.txt true" "Maze is not of a valid configuration!" "$invalid_input_path/empty.txt" # Confirm that mazes with no start or end points are classed as invalid
perform_test "main" "testing-material/invalid_mazes/invalid_2-no_end_point.txt true" "Maze is not of a valid configuration!" "$invalid_input_path/empty.txt" # Confirm that mazes with no end points are classed as invalid
perform_test "main" "testing-material/invalid_mazes/invalid_1-no_start_point.txt true" "Maze is not of a valid configuration!" "$invalid_input_path/empty.txt" # Confirm that mazes with no start points are classed as invalid

# Maze Loading Functions Testing for mazes that are incorrect dimensions
# The following 2 tests are performed with the debug maze loading set to true
perform_test "main" "testing-material/invalid_mazes/invalid_7-too-big.txt true" "Maze is not of a valid configuration!" "$invalid_input_path/empty.txt" # Confirm that mazes which are larger than 100x100 are classed as invalid
perform_test "main" "testing-material/invalid_mazes/invalid_7-too-small.txt true" "Maze is not of a valid configuration!" "$invalid_input_path/empty.txt" # Confirm that mazes which are less than 5x5 are classed as invalid

# Standard Movement Function Testing Passed Walls
# The following 4 tests are performed with the debug maze loading set to false
perform_test "main" "testing-material/valid_mazes/valid_5-5x5.txt false" "You've cannot walk past walls!" "$valid_input_path/valid_5-pass_wall-up.txt" # Make sure player cannot pass walls upwards
perform_test "main" "testing-material/valid_mazes/valid_5-5x5.txt false" "You've cannot walk past walls!" "$valid_input_path/valid_5-pass_wall-down.txt" # Make sure player cannot pass walls down
perform_test "main" "testing-material/valid_mazes/valid_5-5x5.txt false" "You've cannot walk past walls!" "$valid_input_path/valid_5-pass_wall-right.txt" # Make sure player cannot pass walls right
perform_test "main" "testing-material/valid_mazes/valid_5-5x5.txt false" "You've cannot walk past walls!" "$valid_input_path/valid_5-pass_wall-left.txt" # Make sure player cannot pass walls left


