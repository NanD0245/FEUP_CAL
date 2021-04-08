cmake_minimum_required(VERSION 3.17)
project(CAL-TP_classes)

# Build the Google tests
set(GOOGLETEST_PATH lib/googletest-master)
add_subdirectory(${GOOGLETEST_PATH})

# Add the source files of each TP class
file(GLOB TP5_FILES CONFIGURE_DEPENDS
        "TP5/*.cpp"
        )

add_executable(TP5
        main.cpp
        ${TP5_FILES}
        )
target_link_libraries(TP5 gtest_main gmock_main)