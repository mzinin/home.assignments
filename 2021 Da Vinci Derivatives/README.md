# Da Vinci Derivatives Programming Challenge

Given a simple, text-based, data file with a captured order-by-order feed for a single day from some exchange, containing records of the following format (with newline-characters delimiting the records):
```
timestamp;symbol;order-id;operation;side;volume;price
```
For example:
```
14:17:21.877391;DVAM1;00000001;I;BUY;100;12.5
14:17:22.123523;DVAM1;00000002;I;SELL;37;13.5
14:17:22.343883;DVAM1;00000001;A;BUY;100;12.7
14:17:24.737292;DVAM1;00000003;I;SELL;37;13.3
14:17:24.893811;DVAM1;00000004;I;BUY;55;12.7
14:17:25.883711;DVAM1;00000002;C;SELL;37;13.5
```
Where `operation` can be one of:
* `I` = Insert / new order
* `C` = Cancel / delete order
* `A` = Amend / modify order

In this simple assignment an order stays active until it is cancelled.

The task is to implement the following functions:

1. `OrderCounts()`

   Counts the total number of orders per symbol.

2. `BiggestBuyOrders(symbol=”DVAM1”)`

   Finds the top 3 biggest BUY orders in terms of volume for a specific symbol.

3. `BestSellAtTime(symbol=”DVAM1”, timestamp=”15:30:00”)`

   Finds the best SELL price and related order volume for a specific symbol at a given time.


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
./build/test/unit/Release/unit_test.exe
```


## Run manual tests

On Linux:
```
./build/release/test/manual_test <path to data file>
```
On Windows:
```
./build/test/manual/Release/manual_test.exe <path to data file>
```