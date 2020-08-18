#!/bin/bash

PROGRAMS=Programs.hpp

rm $PROGRAMS

echo -e "/* Generated by MakePrograms.sh */\n" > $PROGRAMS
echo -e "#pragma once\n" >> $PROGRAMS

find . -maxdepth 1 -type d -exec bash -c '
    process() {
        if [ $1 == "." ]; then exit; fi
        cd "$1"
        cd Scripts
        for i in $(ls *.sh); do  FILE=${i%.sh}.id; echo -n /tmp/${i%.sh}XXXXXX > $FILE  ; done
        reswrap -S -a -k -N -o ../Scripts.hpp *
        rm *.id
        echo -e "#include \"${1#./}/Program.hpp\"\n" >> ../../Programs.hpp
    }
    process "$@"
    ' find-bash {} \;