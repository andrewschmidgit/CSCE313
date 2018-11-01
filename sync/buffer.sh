#!/bin/sh
#  test.sh
#  Created by Joshua Higginbotham on 11/8/15.

# This is only provided for your convenience.
# The tests used in this file may or may not
# match up with what is called for for the report,
# so be sure to read that part of the handout.
# But you're free to modify this script however
# you need to, it's not graded.

echo "Cleaning and compiling..."
make
echo "Running tests..."
echo "Worker threads,Time" >> output.txt

echo "200,1," >> buffer.txt
./client -t -n 10000 -w  -b 1 >> buffer.txt
echo "200,25," >> buffer.txt
./client -t -n 10000 -w  -b 25 >> buffer.txt
echo "200,50," >> buffer.txt
./client -t -n 10000 -w  -b 50 >> buffer.txt
echo "200,75," >> buffer.txt
./client -t -n 10000 -w  -b 75 >> buffer.txt
echo "200,100," >> buffer.txt
./client -t -n 10000 -w  -b 100 >> buffer.txt
echo "200,125," >> buffer.txt
./client -t -n 10000 -w  -b 125 >> buffer.txt
echo "200,150," >> buffer.txt
./client -t -n 10000 -w  -b 150 >> buffer.txt
echo "200,175," >> buffer.txt
./client -t -n 10000 -w  -b 175 >> buffer.txt
echo "200,200," >> buffer.txt
./client -t -n 10000 -w  -b 200 >> buffer.txt
echo "200,225," >> buffer.txt
./client -t -n 10000 -w  -b 225 >> buffer.txt
echo "200,250," >> buffer.txt
./client -t -n 10000 -w  -b 250 >> buffer.txt
echo "200,275," >> buffer.txt
./client -t -n 10000 -w  -b 275 >> buffer.txt
echo "200,300," >> buffer.txt
./client -t -n 10000 -w  -b 300 >> buffer.txt
echo "200,325," >> buffer.txt
./client -t -n 10000 -w  -b 325 >> buffer.txt
echo "200,350," >> buffer.txt
./client -t -n 10000 -w  -b 350 >> buffer.txt
echo "200,375," >> buffer.txt
./client -t -n 10000 -w  -b 375 >> buffer.txt
echo "200,400," >> buffer.txt
./client -t -n 10000 -w  -b 400 >> buffer.txt
echo "200,425," >> buffer.txt
./client -t -n 10000 -w  -b 425 >> buffer.txt
echo "200,450," >> buffer.txt
./client -t -n 10000 -w  -b 450 >> buffer.txt
echo "200,475," >> buffer.txt
./client -t -n 10000 -w  -b 475 >> buffer.txt
echo "200,500," >> buffer.txt
./client -t -n 10000 -w  -b 500 >> buffer.txt

echo "Finished!"