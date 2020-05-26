#!/bin/bash
ARG=$1

if [[ $ARG == "clean" ]]
then
    rm example1
    rm example2
else
    cc -o example1 example1.c -Wall -Wextra -pedantic
    cc -o example2 example2.c -Wall -Wextra -pedantic
fi
