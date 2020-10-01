#!/bin/sh

# put in folder with homework directories

# need to install make and valgrind
# makefile should have clean option

# homework directory names (split with space)
export DIRS="c201 c203 c206"

# script
for FILE in $DIRS; do
    if [ "$1" = "clean" ]; then
        cd "$FILE" 2>/dev/null || echo "Directory '$FILE' doesn't exist or you don't have permissions"
        if [ ! -f "Makefile" ]; then continue; fi
        printf "\n"
        echo "==================-$FILE-============================================================="
        echo "-------------------clean------------------"
        make clean >/dev/null
        rm -f "$FILE"-my.output 
        echo Clean ok
        cd ..
        printf "\n"
        continue
    fi

    if [ -z "$1" ] || [ "$FILE" = "$1" ]; then
        cd "$FILE" 2>/dev/null || echo "Directory $PWD '$FILE' doesn't exist or you don't have permissions"
        if [ ! -f "Makefile" ]; then continue; fi
        printf "\n"
        echo "==================-$FILE-============================================================="
        echo "-------------------make-------------------"
        make clean >/dev/null
        make | grep -v 'gcc -Wall -std=c99 -pedantic -lm -o'
        if [ ! -f "$FILE-test" ]; then 
            cd ..; 
            continue; 
        else
            echo "Make ok"
        fi
        echo "--------------valgrind-check--------------"
        valgrind -q ./"$FILE"-test 2>&1 | grep -v 'error calling PR_SET_PTRACER, vgdb might block' | grep '==[0-9][0-9][0-9][0-9]==' || echo "Malloc and memory acces ok"
        valgrind ./"$FILE"-test 2>&1 | grep 'All heap blocks were freed -- no leaks are possible' >/dev/null && echo "Memory free ok" || valgrind ./"$FILE"-test 2>&1 | awk '/HEAP/,/suppressed: .+ blocks$/'
        echo "--------------output-compare--------------"
        ./"$FILE"-test >"$FILE"-my.output
        diff -s "$FILE"*.output | grep -v "identical" || echo "Output ok"
        cd ..
        printf "\n"
        if [ "$FILE" = "$1" ]; then exit; fi
        continue
    fi

    if [ -d "$1" ]; then
        echo "Directory '$1' is not listed"
    fi
    exit
done