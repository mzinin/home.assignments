SET (TARGET_NAME unit_test)

SET (SOURCES
    "buy_order_finder.cpp"
    "main.cpp"
    "order_counter.cpp"
    "record.cpp"
    "sell_order_finder.cpp"
    "symbol.cpp"
    "timestamp.cpp"
)

ADD_EXECUTABLE (${TARGET_NAME}
    ${SOURCES}
)

TARGET_INCLUDE_DIRECTORIES (${TARGET_NAME}
    PUBLIC "../../3rdparty"
)

TARGET_LINK_LIBRARIES (${TARGET_NAME}
    orders
)

INSTALL (
    TARGETS ${TARGET_NAME}
    RUNTIME DESTINATION ${TEST_OUTPUT_DIR}
)
