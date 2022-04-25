# Paper-Rock-Scissors Game

Paper-Rock-Scissors is a game for two players. Each player simultaneously opens their hand to display a symbol:
* Fist equals rock
* Open hand equals paper
* Showing the index and middle finger equals scissors.

The winner is determined by the following schema:
* paper beats (wraps) rock
* rock beats (blunts) scissors
* scissors beats (cuts) paper.

This is a simple implementation of the game which allow a user to play agains a computer.

## Prerequisites

To compile this project one would need:
* C++ compiler with C++17 support
* CMake >= 3.5.0


## Compile library

On Linux:
```
./build.sh
```
On Windows:
```
cd ./build
cmake ..
cmake --build . --config Release
```


## Run unit tests

On Linux:
```
./test.sh
```
On Windows:
```
./build/test/Release/unit-tests.exe
```

## Play the game

On Linux run:
```
./run.sh
```
On Windows:
```
./build/release/paper-rock-scissors.exe
```

First user is asked for a number of rounds:
```
$ ./run.sh
How many rounds would you like to play? Enter a number between 0 and 255:
```

Until a valid number is entered, user will be asked this question again and again:
```
$ ./run.sh
How many rounds would you like to play? Enter a number between 0 and 255:
-4
Number of rounds is too small, try again.
How many rounds would you like to play? Enter a number between 0 and 255:
1000
Number of rounds is too big, try again.
How many rounds would you like to play? Enter a number between 0 and 255:
3
Starting a new game of 3 rounds
```

During each of the rounds user have to choose one of 3 available throws - paper, rock or scissors. In the case of invalid input, used is asked to make a throw again:
```
Round 1:
Choose your throw (P[APER], R[OCK], S[CISSORS]):
t
Invalid choice, try again.
Choose your throw (P[APER], R[OCK], S[CISSORS]):
p
Human's throw: PAPER
Computer's throw: ROCK
Human won round 1
```

Once proper throw is made, outcome of the round is printed, including the throw made by the computer opponent:
```
Round 2:
Choose your throw (P[APER], R[OCK], S[CISSORS]):
r
Human's throw: ROCK
Computer's throw: SCISSORS
Human won round 2
```

Once the last round is finished, the whole is concluded and the statistics is printed out:
```
Game is finished, 3 rounds were played:
Human won 1 round(s)
Computer won 0 round(s)
2 round(s) ended in a draw
Human is the winner!
```
