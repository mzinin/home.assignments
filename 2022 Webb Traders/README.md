# Codewriting

In this challenge you are going to write a matching engine that manages multiple central limit order books.

To fulfill the challenge you will have to implement the interface as described in `main.hpp`.

A matching engine matches orders from market participants. These matches will result in trades. A trade occurs when someone wants to buy at an equal or higher price than someone else wants to sell.

When someone wants to buy, a market participant sends a buy order for a given symbol (e.g. AAPL or TSLA). A sent order contains an id, side, limit price and volume. The limit price indicates that in case of a buy order you are willing to buy at the given price or lower. In case of a sell order the limit price indicates that you are willing to sell at the given price or higher.

All orders are managed in a central limit order book (CLOB) which has 2 sides, the buy side and the sell side. If a new order is received by the matching engine, it first checks whether it can match with orders already in the order book on the opposite side. The order will be matched with the opposite side until either the volume of the new order is exhausted or until there are no orders on the opposite side with which the new order can match. The matching priority starts with price priority, the "best" price matches first. If there are multiple orders in the order book at the same price, these orders are matched with time priority; orders which were inserted earlier are matched first.

There are two operations that can be applied to an order once it is in the order book; "pull" and "amend". A pull removes the order from the order book. An amend changes the price and/or volume of the order. An amend causes the order to lose time priority in the order book, unless the only change to the order is that the volume is decreased. If the price of the order is amended, it needs to be re-evaluated for potential matches.

# Example

There is an order book with the following orders:

```
ID Side Volume @ Price
4  Buy  7      @ 120
3  Buy  23     @ 121 <- This has higher price priority than the above order since it offers a better price
2  Buy  12     @ 121 <- This has higher time priority than the above order since it was added earlier
1  Buy  1      @ 122
```

Next we insert a sell order with a volume of 16 and a price of 120. The order book now looks like:

```
ID Side Volume @ Price
4  Buy  7      @ 120
3  Buy  20     @ 121

Fills:
MatchedID Volume @ Price
1         1      @ 122 <- Match against order 1
2         12     @ 121 <- Match against order 2
3         3      @ 121 <- Match against order 3
```

Next we insert another sell order (ID = 5) with a volume of 24 and a price of 121. The order book now looks like:

```
ID Side Volume @ Price
4  Buy  7      @ 120
----------------------
5  Sell 4      @ 121 <- leftover volume after match

Fills:
MatchedID Volume @ Price
3         20     @ 121
```

## [execution time limit] 30 seconds