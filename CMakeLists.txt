cmake_minimum_required(VERSION 3.3)

project(blink1-control)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -DDEBUG")
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

##################
# BLINK1-CONTROL #
##################
set(EXECUTABLE_NAME "blink1-control")
add_executable(${EXECUTABLE_NAME} ${SOURCE_DIR}/main.cpp)

###########
# DOXYGEN #
###########
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN YES)
find_package(Doxygen)
if (DOXYGEN_FOUND)
    doxygen_add_docs(
        doxygen
        ${SOURCE_DIR}
        COMMENT "Generate documentation"
    )
endif(DOXYGEN_FOUND)

###########
# PUGIXML #
###########
add_subdirectory(pugixml)
target_link_libraries(${EXECUTABLE_NAME} pugixml)

###############
# BLINK1-TOOL #
###############
set(BLINK1_DIR ${CMAKE_CURRENT_SOURCE_DIR}/blink1-tool)
add_definitions(-DUSE_HIDAPI)
include_directories(${BLINK1_DIR})
include_directories(${BLINK1_DIR}/hidapi/hidapi)
add_custom_command(OUTPUT ${BLINK1_DIR}/libblink1.so
                   COMMAND make lib
                   WORKING_DIRECTORY ${BLINK1_DIR})
