#!/bin/bash
########################################
#
# Test script to check the generated .h 
# file by ploting all the letters
#
# Usage: ./test.sh Path/to/file
#
########################################

pid=$$
dir=/tmp/$pid
mkdir $dir
trap 'unset IFS;rm -r "$dir"' EXIT

# We parse the .h file separating the letter by a line withe the character 'a'
cat $1|grep '}'|sed 's/\ \ }./a/'|sed 's/..}.//'|sed 's/{//'|sed 's/,//g' |grep -v '}'>$dir/todas
cd $dir

# Change the BASH separator
IFS=a

for i in $(cat todas)
   do echo $i | gnuplot -e "set terminal png; \
      set style data linespoints; \
      set xrange [-1:73]; \
      set yrange [-1:73]; \
      plot '-'; \
      pause -1" > foo.png
      
      echo "Nueva letra"
      echo "$i"
      display foo.png 
   done
