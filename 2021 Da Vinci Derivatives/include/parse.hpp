#pragma once

#include <types.hpp>

#include <istream>
#include <string>
#include <string_view>
#include <vector>


Timestamp timestampFromString(const std::string_view& tsString);

std::string timestampToString(Timestamp ts);


Symbol symbolFromString(const std::string_view& symbolString);

std::string symbolToString(Symbol symbol);


Price priceFromDouble(double price);

double priceToDouble(Price price);


Record recordFromString(const std::string_view& recordString);


std::vector<Record> recordsFromStream(std::istream& stream);

std::vector<Record> recordsFromFile(const std::string& filename);
