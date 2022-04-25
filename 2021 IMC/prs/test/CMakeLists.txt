SET (TARGET_NAME unit-tests)

SET (SOURCES
    "computer_player.cpp"
    "human_player.cpp"
    "main.cpp"
    "referee.cpp"
    "throw.cpp"
)

SET (SOURCES_UNDER_TEST
    "../src/computer_player.cpp"
    "../src/human_player.cpp"
    "../src/referee.cpp"
    "../src/throw.cpp"
)

ADD_EXECUTABLE (${TARGET_NAME}
    ${SOURCES}
    ${SOURCES_UNDER_TEST}
)

TARGET_INCLUDE_DIRECTORIES (${TARGET_NAME}
    PUBLIC "../3rdparty/"
    PUBLIC "../include/"
)

INSTALL (
    TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION ${TEST_OUTPUT_DIR}
)