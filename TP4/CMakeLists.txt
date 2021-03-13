cmake_minimum_required(VERSION 3.17)
project(CAL-TP_classes)

# Build the Google tests
set(GOOGLETEST_PATH lib/googletest-master)
add_subdirectory(${GOOGLETEST_PATH})

# Add the source files of each TP class
file(GLOB TP4_FILES CONFIGURE_DEPENDS
        "TP4/*.cpp"
        )

add_executable(TP4
        main.cpp
        ${TP4_FILES}
        )
target_link_libraries(TP4 gtest_main gmock_main)