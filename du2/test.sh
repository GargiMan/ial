#!/bin/sh
#---------------------------------INFO-AND-SETUP---------------------------------------------------------------

# Testing for IAL homeworks (Make, Memory check, Basic tests)
# by Gargi

# put in folder with homework directories which contain files for testing
# using make and valgrind, install them
# makefile should have clean
# if you changed (upgraded) basic tests , ignore output compare

# ./test.sh             => run all tests with all directories
# ./test.sh [name]      => run tests with inserted directory name
# ./test.sh -clean      => delete all generated files by this script (cxxx-my.output file and  run make clean)
# to redirect output from script to file use ./test.sh >file
# -s as first argument for skip output

#--------------------------------------SCRIPT------------------------------------------------------------------

SKIPTEST=0
if [ "$1" = "-s" ]; then
    shift
    SKIPTEST=1
fi

DIRS=$(ls -d */ | cut -f1 -d'/')    #directories in $PWD without slash (/)
DIRARG=$(echo $1 | cut -f1 -d'/')   #directory in arguments without slash (/)

for FILE in $DIRS; do
    if [ "$1" = "-clean" ]; then
        cd "$FILE" 2>/dev/null || echo "\e[1;91mDirectory '$FILE' doesn't exist or you don't have permissions\e[0;39m"
        if [ ! -f "Makefile" ]; then continue; fi
        printf "\n"
        echo "==================-$FILE-============================================================="
		
        echo "-------------------clean------------------"
        make clean >/dev/null
        rm -f "$FILE"-my.output 
        echo "Files deleted \e[1;92mok \e[0;39m"
		
        cd ..
        continue
    fi

    if [ -z "$1" ] || [ "$FILE" = "$DIRARG" ]; then
        cd "$FILE" 2>/dev/null || echo "\e[1;91mDirectory '$FILE' doesn't exist or you don't have permissions\e[0;39m"
        if [ ! -f "Makefile" ]; then continue; fi
        printf "\n"
        echo "==================-$FILE-============================================================="
		
        echo "-------------------make-------------------"

        make clean >/dev/null
        make >/dev/null
        ERRCODE=$?

        if [ "$ERRCODE" -eq 0 ]; then
            echo "Executable file: \e[1;92mok \e[0;39m"
        else
            printf "\n"
            echo "Executable file: \e[1;91mnot ok \e[0;39m"
            cd ..
            continue
        fi
		
        echo "-----------------valgrind-----------------"

        valgrind -q ./"$FILE"-test 2>&1 | grep -v 'error calling PR_SET_PTRACER, vgdb might block' | 
			grep '==[0-9][0-9][0-9][0-9]==' || echo "Malloc and memory access: \e[1;92mok \e[0;39m"
        valgrind ./"$FILE"-test 2>&1 | grep 'All heap blocks were freed -- no leaks are possible' >/dev/null &&
			echo "Memory free: \e[1;92mok \e[0;39m" || valgrind ./"$FILE"-test 2>&1 | awk '/HEAP/,/suppressed: .+ blocks$/'


        echo "-------------------test-------------------"

        if [ "$SKIPTEST" -eq 1 ]; then
            echo "Output files: \e[1;93mskipped \e[0;39m"
        else 
            # create comparable file and compare for exit code
            ./"$FILE"-test >"$FILE"-my.output
		    diff "$FILE"*.output >/dev/null 2>&1
        
            # exit code 0 - correct , X - print output
            if [ "$?" -eq 0 ]; then
                echo "Output files: \e[1;92mok \e[0;39m"
            else 
                diff -y "$FILE"*.output
            fi
        fi
        cd ..

        # exit if only 1 part should be tested
        if [ "$FILE" = "$1" ]; then break; fi
        continue
    fi

    # invalid option
    echo "$1" | grep -v '^[-]' >/dev/null
    if [ $? -ne 0 ]; then 
        echo "\e[1;91mOption '$1' is not correct for this script, use --help option for info\e[0;39m" 
        exit
    fi

    echo "$DIRS" | grep "$DIRARG" >/dev/null;
    if [ $? -ne 0 ]; then echo "\e[1;91mDirectory '$DIRARG' does not exist\e[0;39m"; exit; fi

done

printf "\n"