CMAKE_MINIMUM_REQUIRED (VERSION 3.5.0 FATAL_ERROR)

PROJECT (paper-rock-scissors VERSION 0.1.0 LANGUAGES CXX)

SET (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

INCLUDE (BuildFlags)
INCLUDE (OutputDirs)

SET (HEADERS
    "include/computer_player.hpp"
    "include/human_player.hpp"
    "include/player.hpp"
    "include/referee.hpp"
    "include/throw.hpp"
    "include/types.hpp"
)

SET (SOURCES
    "src/computer_player.cpp"
    "src/human_player.cpp"
    "src/referee.cpp"
    "src/main.cpp"
    "src/throw.cpp"
)

ADD_EXECUTABLE (${PROJECT_NAME}
    ${HEADERS}
    ${SOURCES}
)

TARGET_INCLUDE_DIRECTORIES (${PROJECT_NAME}
    PUBLIC "include/"
)

ADD_SUBDIRECTORY (test)
