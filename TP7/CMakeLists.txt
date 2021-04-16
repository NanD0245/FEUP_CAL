cmake_minimum_required(VERSION 3.17)
project(CAL-TP_classes)

# Build the Google tests
set(GOOGLETEST_PATH lib/googletest-master)
add_subdirectory(${GOOGLETEST_PATH})

# Add the source files of each TP class
file(GLOB TP7_FILES CONFIGURE_DEPENDS
        "TP7/*.cpp"
        )

add_executable(TP7
        main.cpp
        ${TP7_FILES}
        )
target_link_libraries(TP7 gtest_main gmock_main)