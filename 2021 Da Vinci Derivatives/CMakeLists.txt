CMAKE_MINIMUM_REQUIRED (VERSION 3.5.0 FATAL_ERROR)

PROJECT (orders VERSION 0.1.0 LANGUAGES CXX)

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

INCLUDE (BuildFlags)
INCLUDE (OutputDirs)

SET (HEADERS
    "include/buy_order_finder.hpp"
    "include/order_counter.hpp"
    "include/parse.hpp"
    "include/sell_order_finder.hpp"
    "include/types.hpp"
)

SET (SOURCES
    "src/buy_order_finder.cpp"
    "src/order_counter.cpp"
    "src/parse.cpp"
    "src/sell_order_finder.cpp"
)

ADD_LIBRARY (${PROJECT_NAME} STATIC
    ${HEADERS}
    ${SOURCES}
)

TARGET_INCLUDE_DIRECTORIES (${PROJECT_NAME}
    PUBLIC "include/"
)

ADD_SUBDIRECTORY (test)
