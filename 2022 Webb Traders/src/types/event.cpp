#include "types/event.hpp"

// Note: no checks are done, all inputs are considered valid.

Event::Event(const std::string& str)
{
    // parse command
    size_t begin = 0;
    size_t end = str.find(DELIMITER, begin);
    command = parseCommand(std::string_view{&str[begin], end - begin});

    // parse order id
    begin = end + 1;
    end = command == Command::PULL ? str.size() : str.find(DELIMITER, begin);
    orderId = parseOrderId(std::string_view{&str[begin], end - begin});

    switch (command)
    {
        case Command::INSERT:
            // parse symbol
            begin = end + 1;
            end = str.find(DELIMITER, begin);
            symbol = std::string_view{&str[begin], end - begin};

            // parse side
            begin = end + 1;
            end = str.find(DELIMITER, begin);
            side = parseSide(std::string_view{&str[begin], end - begin});
            // [[fallthrough]]

        case Command::AMEND:
            // parse price
            begin = end + 1;
            end = str.find(DELIMITER, begin);
            price = parsePrice(std::string_view{&str[begin], end - begin});

            // parse volume
            begin = end + 1;
            volume = parseVolume(std::string_view{&str[begin], str.size() - begin});
            break;
    }
}
