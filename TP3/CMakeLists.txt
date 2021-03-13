cmake_minimum_required(VERSION 3.17)
project(CAL-TP_classes)

# Build the Google tests
set(GOOGLETEST_PATH lib/googletest-master)
add_subdirectory(${GOOGLETEST_PATH})

# Add the source files of each TP class
file(GLOB TP3_FILES CONFIGURE_DEPENDS
        "TP3/*.cpp"
        )

add_executable(TP3
        main.cpp
        ${TP3_FILES}
        )
target_link_libraries(TP3 gtest_main gmock_main)