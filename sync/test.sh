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
echo "Worker threads,Buffer size,Time" >> output.txt

echo "1,1" >> output.txt
	./client -t -n 10000 -w 1 -b 1 >> output.txt
echo "10,25" >> output.txt
	./client -t -n 10000 -w 10 -b 25 >> output.txt
echo "20,50" >> output.txt
	./client -t -n 10000 -w 20 -b 50 >> output.txt
echo "30,75" >> output.txt
	./client -t -n 10000 -w 30 -b 75 >> output.txt
echo "40,100" >> output.txt
	./client -t -n 10000 -w 40 -b 100 >> output.txt
echo "50,125" >> output.txt
	./client -t -n 10000 -w 50 -b 125 >> output.txt
echo "60,150" >> output.txt
	./client -t -n 10000 -w 60 -b 150 >> output.txt
echo "70,175" >> output.txt
	./client -t -n 10000 -w 70 -b 175 >> output.txt
echo "80,200" >> output.txt
	./client -t -n 10000 -w 80 -b 200 >> output.txt
echo "90,225" >> output.txt
	./client -t -n 10000 -w 90 -b 225 >> output.txt
echo "100,250" >> output.txt
	./client -t -n 10000 -w 100 -b 250 >> output.txt
echo "110,275" >> output.txt
	./client -t -n 10000 -w 110 -b 275 >> output.txt
echo "120,300" >> output.txt
	./client -t -n 10000 -w 120 -b 300 >> output.txt
echo "130,325" >> output.txt
	./client -t -n 10000 -w 130 -b 325 >> output.txt
echo "140,350" >> output.txt
	./client -t -n 10000 -w 140 -b 350 >> output.txt
echo "150,375" >> output.txt
	./client -t -n 10000 -w 150 -b 375 >> output.txt
echo "160,400" >> output.txt
	./client -t -n 10000 -w 160 -b 400 >> output.txt
echo "170,425" >> output.txt
	./client -t -n 10000 -w 170 -b 425 >> output.txt
echo "180,450" >> output.txt
	./client -t -n 10000 -w 180 -b 450 >> output.txt
echo "190,475" >> output.txt
	./client -t -n 10000 -w 190 -b 475 >> output.txt
echo "200,500" >> output.txt
	./client -t -n 10000 -w 200 -b 500 >> output.txt

echo "Finished!"