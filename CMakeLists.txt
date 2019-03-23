cmake_minimum_required(VERSION 3.10)

project(blink1-control)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

##################
# BLINK1-CONTROL #
##################
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG -g")
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})
set(EXECUTABLE_NAME "blink1-control")

set(SOURCES
    ${SOURCE_DIR}/Blink1Device.cpp
    ${SOURCE_DIR}/PatternLine.cpp
    ${SOURCE_DIR}/PatternLineN.cpp
    ${SOURCE_DIR}/PlayState.cpp
    ${SOURCE_DIR}/RGB.cpp
    ${SOURCE_DIR}/RGBN.cpp
)
add_executable(${EXECUTABLE_NAME} ${SOURCE_DIR}/main.cpp ${SOURCES})

set(WARNINGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation")
set_target_properties(${EXECUTABLE_NAME} PROPERTIES COMPILE_FLAGS "${WARNINGS}")

set(CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 17)

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
add_subdirectory(${LIB_DIR}/pugixml)
target_link_libraries(${EXECUTABLE_NAME} pugixml)

###############
# BLINK1-TOOL #
###############
set(BLINK1_DIR ${LIB_DIR}/blink1-tool)
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

###########
# Testing #
###########
add_subdirectory(${LIB_DIR}/googletest)
include(GoogleTest)
set(TEST_EXECUTABLE_NAME ${EXECUTABLE_NAME}_test)
set(TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

set(TEST_SOURCES
    ${TEST_SOURCE_DIR}/Blink1Device_BadInit_test.cpp
    ${TEST_SOURCE_DIR}/Blink1Device_GoodInit_test.cpp
    ${TEST_SOURCE_DIR}/Blink1Device_GoodInitBadFunction_test.cpp
    ${TEST_SOURCE_DIR}/FakeBlink1Lib.cpp
    ${TEST_SOURCE_DIR}/PatternLineN_test.cpp
    ${TEST_SOURCE_DIR}/PatternLine_test.cpp
    ${TEST_SOURCE_DIR}/PlayState_test.cpp
    ${TEST_SOURCE_DIR}/RGBN_test.cpp
    ${TEST_SOURCE_DIR}/RGB_test.cpp
)

enable_testing()
add_executable(${TEST_EXECUTABLE_NAME} ${TEST_SOURCES} ${SOURCES})
target_link_libraries(${TEST_EXECUTABLE_NAME} gtest gtest_main gmock gmock_main pugixml)
gtest_discover_tests(${TEST_EXECUTABLE_NAME})
set_property(TARGET ${TEST_EXECUTABLE_NAME} PROPERTY CXX_STANDARD 17)

# Coverage
if (CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    set(COVERAGE_GCOVR_EXCLUDES '${LIB_DIR}' '${TEST_SOURCE_DIR}' '${SOURCE_DIR}/main.cpp')
    SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME coverage
                                         EXECUTABLE ${TEST_EXECUTABLE_NAME}
                                         DEPENDENCIES ${TEST_EXECUTABLE_NAME})
endif (CMAKE_BUILD_TYPE MATCHES Debug)
