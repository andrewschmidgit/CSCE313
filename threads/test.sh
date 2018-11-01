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

echo "1," >> output.txt
./client -t -n 10000 -w 1 >> output.txt
echo progress
echo "10," >> output.txt
./client -t -n 10000 -w 10 >> output.txt
echo "20," >> output.txt
./client -t -n 10000 -w 20 >> output.txt
echo "30," >> output.txt
./client -t -n 10000 -w 30 >> output.txt
echo "40," >> output.txt
./client -t -n 10000 -w 40 >> output.txt
echo "50," >> output.txt
./client -t -n 10000 -w 50 >> output.txt
echo "60," >> output.txt
./client -t -n 10000 -w 60 >> output.txt
echo "70," >> output.txt
./client -t -n 10000 -w 70 >> output.txt
echo progress
echo "80," >> output.txt
./client -t -n 10000 -w 80 >> output.txt
echo "90," >> output.txt
./client -t -n 10000 -w 90 >> output.txt
echo "100," >> output.txt
./client -t -n 10000 -w 100 >> output.txt
echo "110," >> output.txt
./client -t -n 10000 -w 110 >> output.txt
echo "120," >> output.txt
./client -t -n 10000 -w 120 >> output.txt
echo progress
echo "130," >> output.txt
./client -t -n 10000 -w 130 >> output.txt
echo "140," >> output.txt
./client -t -n 10000 -w 140 >> output.txt
echo "150," >> output.txt
./client -t -n 10000 -w 150 >> output.txt
echo "160," >> output.txt
./client -t -n 10000 -w 160 >> output.txt
echo "170," >> output.txt
./client -t -n 10000 -w 170 >> output.txt
echo progress
echo "180," >> output.txt
./client -t -n 10000 -w 180 >> output.txt
echo "190," >> output.txt
./client -t -n 10000 -w 190 >> output.txt
echo "200," >> output.txt
./client -t -n 10000 -w 200 >> output.txt

echo "Finished!"