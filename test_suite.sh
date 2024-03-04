#!/bin/bash
#Testing suite for my maze game c file "main.c"

argument_test()
{
    #$1 should be name of c file without extensiuon
    #$2 should be the arguments
    #$3 should be the expected outputs
    local c_file_name="$1.c"
    local output_file="$1_output.txt"
    local expected_output="$3"
    local arguments=$2

    # Run our program with our arguments then write output to $output_file
    "./$1" $arguments > "$output_file"
    if [ $? -ne 0 ]; then # Check if the previous output isnt equal to 0
        echo "Execution failed completely for $c_file_name, return wasnt 0"
        return 2
    fi

    # If the program successfully ran compare the output to our desired output
    if diff <(echo -n "$expected_output") "$output_file" > /dev/null; then # This will discard output from diff
        echo "Test Pass (Args[$arguments]): Output matches for $c_file_name"
    else
        echo "Test Fail (Args[$arguments]): Output differs for $c_file_name"
    fi
}

# Takes one argument : Name of the C file without its extension
compilation_test()
{
    local c_file_name = $1 # Without extension

    # Compile the C program
    gcc "$c_file_name.c" -o "$c_file_name"

    if [ $? -ne 0 ]; then # Check if the previous exit code wasn't 0  (if it wasnt then the program exited unsuccessfully)
        echo "Compilation failed for $c_file"
        return 1
    fi
}

perform_test "main" "testing-material/maze-1" "expected output string"

