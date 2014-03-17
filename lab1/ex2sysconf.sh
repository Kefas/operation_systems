#!/bin/bash

echo "#include <stdio.h>"
echo "#include <unistd.h>"
echo "#include <limits.h>"

echo "int main(void) {"
echo "long r;"

for i in `cat sysconf.list.txt | grep _SC`
do
    echo '#ifdef '$i
    echo "r = sysconf($i);"
    echo "printf(\"The value of $i is %d\\n\", r);"
    echo "#else"
    echo "printf(\"The $i is undefined.\\n\");"
    echo '#endif'
done

echo "return 0;"
echo "}"