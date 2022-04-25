#include <parse.hpp>

#include <array>
#include <charconv>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <string>
#include <system_error>

namespace
{
    constexpr Timestamp US_PER_TEN_HOURS = 36000000000;
    constexpr Timestamp US_PER_HOUR = 3600000000;
    constexpr Timestamp US_PER_TEN_MINUTES = 600000000;
    constexpr Timestamp US_PER_MINUTE = 60000000;
    constexpr Timestamp US_PER_TEN_SECONDS = 10000000;
    constexpr Timestamp US_PER_SECOND = 1000000;
    constexpr Timestamp US_PER_HUNDRED_MS = 100000;
    constexpr Timestamp US_PER_TEN_MS = 10000;
    constexpr Timestamp US_PER_MS = 1000;
    constexpr Timestamp US_PER_HUNDRED_US = 100;
    constexpr Timestamp US_PER_TEN_US = 10;

    // Timestamps are expected in te form "15:30:21.123456"
    // up to 15 symbols, up to 3 symbols are just delimiters
    constexpr std::array<Timestamp, 15> TIMESTAMP_SYMBOL_VALUE = {
        US_PER_TEN_HOURS, US_PER_HOUR, 0,
        US_PER_TEN_MINUTES, US_PER_MINUTE, 0,
        US_PER_TEN_SECONDS, US_PER_SECOND, 0,
        US_PER_HUNDRED_MS, US_PER_TEN_MS, US_PER_MS,
        US_PER_HUNDRED_US, US_PER_TEN_US, 1
    };

    constexpr size_t SYMBOL_OFFSET = 16;

    constexpr char DELIMITER = ';';
    constexpr char INSERT_OPERATION = 'I';
    constexpr char CANCEL_OPERATION = 'C';
    constexpr char AMEND_OPERATION = 'A';

    constexpr char BUY_OPERATION = 'B';
    constexpr size_t BUY_OPERATION_SIZE = 3;
    constexpr char SELL_OPERATION = 'S';
    constexpr size_t SELL_OPERATION_SIZE = 4;

    // All prices are considired with one tenth precision.
    constexpr int PRICE_COEFFICIENT = 10;

    inline size_t findNextOffet(const std::string_view& str, size_t currentOffset)
    {
        auto result = str.find(DELIMITER, currentOffset);
        if (result == std::string::npos)
        {
            throw std::runtime_error("Malformed data");
        }
        return result + 1;
    }
}

Timestamp timestampFromString(const std::string_view& tsString)
{
    Timestamp result = 0;
    for (size_t i = 0; i < tsString.size(); ++i)
    {
        result += (tsString[i] - '0') * TIMESTAMP_SYMBOL_VALUE[i];
    }
    return result;
}

std::string timestampToString(Timestamp ts)
{
    std::string result = "00:00:00.000000";

    for (size_t i = 0; i < result.size(); ++i)
    {
        if (TIMESTAMP_SYMBOL_VALUE[i] != 0)
        {
            result[i] += static_cast<char>(ts / TIMESTAMP_SYMBOL_VALUE[i]);
            ts %= TIMESTAMP_SYMBOL_VALUE[i];
        }
    }

    return result;
}

Symbol symbolFromString(const std::string_view& symbolString)
{
    if (symbolString.size() > sizeof(Symbol))
    {
        throw std::runtime_error("Symbol " + std::string{symbolString.begin(), symbolString.end()} + " is to long");
    }

    Symbol result = 0;
    std::memcpy(&result, symbolString.data(), symbolString.size());

    return result;
}

std::string symbolToString(Symbol symbol)
{
    std::string result;
    for (; symbol != 0; symbol >>= 8)
    {
        result += static_cast<char>(symbol % 256);
    }
    return result;
}

Price priceFromDouble(double price)
{
    return static_cast<Price>(price * PRICE_COEFFICIENT);
}

double priceToDouble(Price price)
{
    return static_cast<double>(price) / PRICE_COEFFICIENT;
}

Record recordFromString(const std::string_view& recordString)
{
    Record result;

    // parse timestamp
    result.ts = timestampFromString({recordString.data(), SYMBOL_OFFSET - 1});

    // parse symbol
    const auto orderIDOffset = findNextOffet(recordString, SYMBOL_OFFSET);
    result.symbol = symbolFromString({recordString.data() + SYMBOL_OFFSET, orderIDOffset - SYMBOL_OFFSET - 1});

    // parse order id
    const auto operationOffset = findNextOffet(recordString, orderIDOffset);
    auto fromChars = std::from_chars(recordString.data() + orderIDOffset, recordString.data() + operationOffset - 1, result.orderID);
    if (fromChars.ec != std::errc())
    {
        throw std::runtime_error("Wrong order ID value: " + std::make_error_code(fromChars.ec).message());
    }

    // parse operation
    switch (recordString[operationOffset])
    {
        case INSERT_OPERATION:
            result.operation = Operation::INSERT;
            break;
        case CANCEL_OPERATION:
            result.operation = Operation::CANCEL;
            break;
        case AMEND_OPERATION:
            result.operation = Operation::AMEND;
            break;
        default:
            throw std::runtime_error(std::string{"Unsupported operation "} + recordString[operationOffset]);
    }

    // parse side
    const auto sideOffset = operationOffset + 2;
    size_t volumeOffset = 0;
    switch (recordString[sideOffset])
    {
        case BUY_OPERATION:
            result.side = Side::BUY;
            volumeOffset = sideOffset + BUY_OPERATION_SIZE + 1;
            break;
        case SELL_OPERATION:
            result.side = Side::SELL;
            volumeOffset = sideOffset + SELL_OPERATION_SIZE + 1;
            break;
        default:
            {
                const auto side = recordString.substr(sideOffset, recordString.find(DELIMITER, sideOffset) - sideOffset);
                throw std::runtime_error("Unsupported side " + std::string{side.begin(), side.end()});
            }
    }

    // parse volume
    const auto priceOffset = findNextOffet(recordString, volumeOffset);
    fromChars = std::from_chars(recordString.data() + volumeOffset, recordString.data() + priceOffset - 1, result.volume);
    if (fromChars.ec != std::errc())
    {
        throw std::runtime_error("Wrong volume value: " + std::make_error_code(fromChars.ec).message());
    }

    // parse price
    double price = 0.0;
    fromChars = std::from_chars(recordString.data() + priceOffset, recordString.data() + recordString.size(), price);
    if (fromChars.ec != std::errc())
    {
        throw std::runtime_error("Wrong price value: " + std::make_error_code(fromChars.ec).message());
    }
    result.price = priceFromDouble(price);

    return result;
}

std::vector<Record> recordsFromStream(std::istream& stream)
{
    std::vector<Record> result;
    for (std::string line; std::getline(stream, line);)
    {
        result.push_back(recordFromString(line));
    }
    return result;
}

std::vector<Record> recordsFromFile(const std::string& filename)
{
    std::ifstream input(filename);
    if (!input.good())
    {
        throw std::runtime_error("Failed to open file '" + filename + "' for reading.");
    }
    return recordsFromStream(input);
}

