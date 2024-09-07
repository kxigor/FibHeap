#!/bin/bash

# Fib heap program name
PNAME=
# Tests generator program name
TNAME=
# Answer generator program name
ANAME=

GEN_TEST_NAME="generatedTest.out"
ANS_NAME="ans.out"
FIB_NAME="fib.out"
DIF_NAME="difference.out"

# How many tests will there be (i.e. different runs of the program)
NUMBER_OF_TESTS=100000

# How many will the operation be inside one test
SIZE_OF_TEST=1000

while [ $# -ne 0 ]; do
    case $1 in
        --pname|-p)
            PNAME=$2
            shift 2
            ;;
        --tname|-t)
            TNAME=$2
            shift 2
            ;;
        --aname|-a)
            ANAME=$2
            shift 2
            ;;
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
            echo "Use --pname or -p to set the program name!"
            echo "Use --tname or -t to set the tests generator name!"
            echo "Use --aname or -a to set the answer generator name!"
            echo "Use --number or -n to set the number of tests!"
            echo "Use --size or -s to set size of test!"
            exit 1
            ;;
    esac
done

rm -f $DIF_NAME

for (( i = 0; i < $NUMBER_OF_TESTS; i++)); do
    ./$TNAME > $GEN_TEST_NAME $SIZE_OF_TEST
    ./$ANAME < $GEN_TEST_NAME > $ANS_NAME
    ./$PNAME < $GEN_TEST_NAME > $FIB_NAME
    diff $ANS_NAME $FIB_NAME > $DIF_NAME
    if [ -s $DIF_NAME ]; then
        echo "Error, the tests failed!"
        echo "Take a look at the differenceOut!"
        exit 1
    fi
    echo "Succes!"
done