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
echo "5," >> output.txt
./client -t -n 10000 -w 5 >> output.txt
echo progress
echo "25," >> output.txt
./client -t -n 10000 -w 25 >> output.txt
echo "45," >> output.txt
./client -t -n 10000 -w 45 >> output.txt
echo "65," >> output.txt
./client -t -n 10000 -w 65 >> output.txt
echo "85," >> output.txt
./client -t -n 10000 -w 85 >> output.txt
echo "105," >> output.txt
./client -t -n 10000 -w 105 >> output.txt
echo "125," >> output.txt
./client -t -n 10000 -w 125 >> output.txt
echo "145," >> output.txt
./client -t -n 10000 -w 145 >> output.txt
echo "165," >> output.txt
./client -t -n 10000 -w 165 >> output.txt
echo "185," >> output.txt
./client -t -n 10000 -w 185 >> output.txt
echo "205," >> output.txt
./client -t -n 10000 -w 205 >> output.txt
echo progress
echo "225," >> output.txt
./client -t -n 10000 -w 225 >> output.txt
echo "245," >> output.txt
./client -t -n 10000 -w 245 >> output.txt
echo "265," >> output.txt
./client -t -n 10000 -w 265 >> output.txt
echo "285," >> output.txt
./client -t -n 10000 -w 285 >> output.txt
echo "305," >> output.txt
./client -t -n 10000 -w 305 >> output.txt
echo "325," >> output.txt
./client -t -n 10000 -w 325 >> output.txt
echo "345," >> output.txt
./client -t -n 10000 -w 345 >> output.txt
echo "365," >> output.txt
./client -t -n 10000 -w 365 >> output.txt
echo "385," >> output.txt
./client -t -n 10000 -w 385 >> output.txt
echo "405," >> output.txt
./client -t -n 10000 -w 405 >> output.txt
echo progress
echo "425," >> output.txt
./client -t -n 10000 -w 425 >> output.txt
echo "445," >> output.txt
./client -t -n 10000 -w 445 >> output.txt
echo "465," >> output.txt
./client -t -n 10000 -w 465 >> output.txt
echo "485," >> output.txt
./client -t -n 10000 -w 485 >> output.txt
echo "505," >> output.txt
./client -t -n 10000 -w 505 >> output.txt
echo "509," >> output.txt
./client -t -n 10000 -w 509 >> output.txt
echo "Finished!"
