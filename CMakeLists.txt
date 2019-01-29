cmake_minimum_required(VERSION 3.3)

project(blink1-control)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Werror -DDEBUG")
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

##################
# BLINK1-CONTROL #
##################
set(EXECUTABLE_NAME "blink1-control")
add_executable(${EXECUTABLE_NAME} ${SOURCE_DIR}/main.cpp
                                  ${SOURCE_DIR}/Blink1Device.cpp)

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

###########
# MAYBE.H #
###########
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Maybe.h)

###############
# BLINK1-TOOL #
###############
set(BLINK1_DIR ${CMAKE_CURRENT_SOURCE_DIR}/blink1-tool)
set(LIB_BLINK1_LOC ${BLINK1_DIR}/libblink1.so)

add_definitions(-DUSE_HIDAPI)
include_directories(${BLINK1_DIR})
include_directories(${BLINK1_DIR}/hidapi/hidapi)

add_custom_command(OUTPUT ${LIB_BLINK1_LOC}
                   COMMAND $(MAKE) lib
                   WORKING_DIRECTORY ${BLINK1_DIR})
add_custom_target(libblink1_target DEPENDS ${LIB_BLINK1_LOC})
add_library(libblink1 STATIC IMPORTED GLOBAL)
set_target_properties(libblink1 PROPERTIES IMPORTED_LOCATION ${LIB_BLINK1_LOC})
add_dependencies(libblink1 libblink1_target)
target_link_libraries(${EXECUTABLE_NAME} libblink1)
