#include <buy_order_finder.hpp>
#include <order_counter.hpp>
#include <parse.hpp>
#include <sell_order_finder.hpp>

#include <cstdlib>
#include <cstring>
#include <iostream>

namespace
{
    std::vector<Record> readRecords(int argc, char* argv[])
    {
        if (argc != 2)
        {
            throw std::runtime_error("Wrong number of arguments");
        }

        return recordsFromFile(argv[1]);
    }

    void testOrderCounter(const std::vector<Record>& records)
    {
        OrderCounter counter;
        for (const auto& record : records)
        {
            counter.add(record);
        }

        const auto orders = counter.orderCounts();

        std::cout << "Order counts output:\n";
        for (const auto& [symbol, numberOfOrders] : orders)
        {
            std::cout << symbolToString(symbol) << " -> " << numberOfOrders << "\n";
        }
        std::cout << std::endl;
    }

    void testBuyOrderFinder(const std::vector<Record>& records)
    {
        BuyOrderFinder finder;
        for (const auto& record : records)
        {
            finder.add(record);
        }

        const std::string_view symbol = "DVAM1";
        const auto orders = finder.biggestBuyOrders(symbolFromString(symbol));

        std::cout << "Biggest buy orders output (\"" << symbol << "\"):\n";
        for (const auto& order : orders)
        {
            std::cout << timestampToString(order.ts) << ";"
                      << symbolToString(order.symbol) << ";"
                      << order.orderID << ";" << order.volume << ";" << order.price << "\n";
        }
        std::cout << std::endl;
    }

    void testBestSellAtTimeFinder(const std::vector<Record>& records)
    {
        SellOrderFinder finder;
        for (const auto& record : records)
        {
            finder.add(record);
        }

        const std::string_view symbol = "DVAM1";
        const std::string_view ts = "15:30:00";
        const auto bestSell = finder.bestSellAtTime(symbolFromString(symbol), timestampFromString(ts));

        std::cout << "Best sell at Time (\"" << symbol << "\", \"" << ts << "\"):\n";
        if (!bestSell)
        {
            std::cout << "NONE\n";
        }
        else
        {
            std::cout << priceToDouble(bestSell->price) << ";" << bestSell->volume << "\n";
        }
        std::cout << std::endl;
    }
}

int main(int argc, char* argv[])
{
    try
    {
        const auto records = readRecords(argc, argv);
        std::cout << "Number of records: " << records.size() << "\n\n";

        testOrderCounter(records);
        testBuyOrderFinder(records);
        testBestSellAtTimeFinder(records);

        return EXIT_SUCCESS;
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << "\n"
                  << "Usage:\n"
                  << "\t" << argv[0] << " <path to data file>" << std::endl;
        return EXIT_FAILURE;
    }
}