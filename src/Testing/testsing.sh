#!/bin/bash

# How many tests will there be (i.e. different runs of the program)
NUMBER_OF_TESTS=100000

# How many will the operation be inside one test
SIZE_OF_TEST=1000

while [ $# -ne 0 ]; do
    case $1 in
        --number|-n)
            NUMBER_OF_TESTS=$2
            shift 2
            ;;
        --size|-s)
            SIZE_OF_TEST=$2
            shift 2
            ;;
        *)
            echo "Invalid arguments!"
            echo "Use --number or -n to set the number of tests!"
            echo "Use --size or -s to set size of test!"
            exit 1
            ;;
    esac
done

rm -f differenceOut

for (( i = 0; i < $NUMBER_OF_TESTS; i++)); do
    ./gen.o > generatedTest $SIZE_OF_TEST
    ./ans.o < generatedTest > ansOut
    ./fib.o < generatedTest > fibOut
    diff ansOut fibOut > differenceOut
    if [ -s differenceOut ]; then
        echo "Error, the tests failed!"
        echo "Take a look at the differenceOut!"
        exit 1
    fi
    echo Succes!
done