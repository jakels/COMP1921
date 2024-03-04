#!/bin/bash
#Testing suite for my maze game c file "main.c"

argument_test()
{
    local c_file_name="$1.c"
    local output_file="$1_output.txt"

    local arguments=$2

    local user_inputs="$3"

    local expected_output="$4"

    echo "Running argument test on $c_file_name with expected output of $expected_output, arguments $arguments, and user inputs $user_inputs"

    # Pipe the user inputs into the program and capture the output
    echo -e "$user_inputs" | "./$1" $arguments > "$output_file"
    if [ $? -ne 0 ]; then
        echo "Execution failed completely for $c_file_name, return wasn't 0"
        return 2
    fi

    # Compare the output to the expected output
    if diff <(echo -n "$expected_output") "$output_file" > /dev/null; then
        echo "Test Pass (Args[$arguments], Inputs[$user_inputs]): Output matches for $c_file_name"
    else
        echo "Test Fail (Args[$arguments], Inputs[$user_inputs]): Output differs for $c_file_name"
    fi
}

# Takes one argument : Name of the C file without its extension
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



perform_test "main" "testing-material/reg_5x5_VALID.txt false" "You've successfully finished the maze!" "S\nS\nD" # Basic small maze making sure movement functionality is correct
perform_test "main" "testing-material/reg_10x6_VALID.txt false" "You've successfully finished the maze!" "S\nS\nS\nD\nD\nD" # Basic larger maze for the same purpose
perform_test "main" "testing-material/reg_15x8_VALID.txt false" "You've successfully finished the maze!" "S\nS\nS\nS\nS\nS\nD\nD\nD\nD\nD\nD\nW\nW" # More complex larger maze with multiple turns for movement testing
perform_test "main" "testing-material/reg_15x8_VALID.txt false" "You've successfully finished the maze!" "S\nS\nS\nS\nS\nS\nD\nD\nD\nD\nD\nD\nW\nS\nS\nD\nA\nW\nW" # Testing all movement directions and going back on self

