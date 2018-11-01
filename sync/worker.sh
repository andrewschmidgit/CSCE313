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

echo "1,500," >> worker.txt
./client -t -n 10000 -w 1 -b 500 >> worker.txt
echo "10,500," >> worker.txt
./client -t -n 10000 -w 10 -b 500 >> worker.txt
echo "20,500," >> worker.txt
./client -t -n 10000 -w 20 -b 500 >> worker.txt
echo "30,500," >> worker.txt
./client -t -n 10000 -w 30 -b 500 >> worker.txt
echo "40,500," >> worker.txt
./client -t -n 10000 -w 40 -b 500 >> worker.txt
echo "50,500," >> worker.txt
./client -t -n 10000 -w 50 -b 500 >> worker.txt
echo "60,500," >> worker.txt
./client -t -n 10000 -w 60 -b 500 >> worker.txt
echo "70,500," >> worker.txt
./client -t -n 10000 -w 70 -b 500 >> worker.txt
echo "80,500," >> worker.txt
./client -t -n 10000 -w 80 -b 500 >> worker.txt
echo "90,500," >> worker.txt
./client -t -n 10000 -w 90 -b 500 >> worker.txt
echo "100,500," >> worker.txt
./client -t -n 10000 -w 100 -b 500 >> worker.txt
echo "110,500," >> worker.txt
./client -t -n 10000 -w 110 -b 500 >> worker.txt
echo "120,500," >> worker.txt
./client -t -n 10000 -w 120 -b 500 >> worker.txt
echo "130,500," >> worker.txt
./client -t -n 10000 -w 130 -b 500 >> worker.txt
echo "140,500," >> worker.txt
./client -t -n 10000 -w 140 -b 500 >> worker.txt
echo "150,500," >> worker.txt
./client -t -n 10000 -w 150 -b 500 >> worker.txt
echo "160,500," >> worker.txt
./client -t -n 10000 -w 160 -b 500 >> worker.txt
echo "170,500," >> worker.txt
./client -t -n 10000 -w 170 -b 500 >> worker.txt
echo "180,500," >> worker.txt
./client -t -n 10000 -w 180 -b 500 >> worker.txt
echo "190,500," >> worker.txt
./client -t -n 10000 -w 190 -b 500 >> worker.txt
echo "200,500," >> worker.txt
./client -t -n 10000 -w 200 -b 500 >> worker.txt

echo "Finished!"