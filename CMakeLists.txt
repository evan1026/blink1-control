cmake_minimum_required(VERSION 3.10)

project(blink1-control)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG_PRINTF)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

##################
# BLINK1-CONTROL #
##################
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAG_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG -g")
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})
set(EXECUTABLE_NAME "blink1-control")

set(SOURCES
    ${SOURCE_DIR}/config/ConfigParser.cpp
    ${SOURCE_DIR}/config/PatternCommand.cpp
)

set(EXECUTABLE_SOURCES
    ${SOURCES}
    ${SOURCE_DIR}/main.cpp
)

add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCES})

set(WARNINGS "-Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion")
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    # Set specific warnings if they ever exist
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(WARNINGS "${WARNINGS} -Wmisleading-indentation")
endif()
set_target_properties(${EXECUTABLE_NAME} PROPERTIES COMPILE_FLAGS "${WARNINGS}")

set(CXX_STANDARD_REQUIRED ON)
set_property(TARGET ${EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)

##############
# BLINK1-LIB #
##############
add_definitions(-DUSE_HIDAPI)
add_subdirectory(${LIB_DIR}/blink1-lib)
include_directories(${BLINK1_LIB_INCLUDES})

if (USE_BLINK1_TESTING_LIBRARY)
    add_definitions(-DUSE_BLINK1_TESTING_LIBRARY)
    target_link_libraries(${EXECUTABLE_NAME} ${BLINK1_LIB_TESTING_LIBRARIES})
else(USE_BLINK1_TESTING_LIBRARY)
    target_link_libraries(${EXECUTABLE_NAME} ${BLINK1_LIB_LIBRARIES})
endif(USE_BLINK1_TESTING_LIBRARY)

###########
# DOXYGEN #
###########
set(DOXYGEN_GENERATE_HTML YES)
set(DOXYGEN_GENERATE_MAN YES)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_HTML_EXTRA_STYLESHEET "${LIB_DIR}/doxygen-awesome-css/doxygen-awesome.css")
find_package(Doxygen)
if (DOXYGEN_FOUND)
    doxygen_add_docs(
        doxygen
        ${INCLUDE_DIR}
        COMMENT "Generate documentation"
    )
endif(DOXYGEN_FOUND)

########
# JSON #
########
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${LIB_DIR}/json)
target_link_libraries(${EXECUTABLE_NAME} nlohmann_json::nlohmann_json)

###########
# Testing #
###########
add_subdirectory(${LIB_DIR}/googletest)
include(GoogleTest)
set(TEST_EXECUTABLE_NAME ${EXECUTABLE_NAME}_test)
set(TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

set(TEST_SOURCES
    ${TEST_SOURCE_DIR}/config/ConfigParser_test.cpp
    ${TEST_SOURCE_DIR}/config/PatternCommand_test.cpp
)

enable_testing()
add_executable(${TEST_EXECUTABLE_NAME} ${TEST_SOURCES} ${SOURCES})
target_link_libraries(${TEST_EXECUTABLE_NAME} ${BLINK1_LIB_TESTING_LIBRARIES} gtest gtest_main gmock gmock_main nlohmann_json::nlohmann_json)
gtest_discover_tests(${TEST_EXECUTABLE_NAME})
set_property(TARGET ${TEST_EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)

# Coverage
if (CMAKE_BUILD_TYPE MATCHES Debug)
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules")
    include(CodeCoverage)

    set(COVERAGE_EXCLUDES "/usr/*" "${LIB_DIR}/*" "${TEST_SOURCE_DIR}/*" "${SOURCE_DIR}/main.cpp")
    APPEND_COVERAGE_COMPILER_FLAGS()

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(GCOV_PATH "${CMAKE_CURRENT_SOURCE_DIR}/scripts/llvm-cov.sh")
        SETUP_TARGET_FOR_COVERAGE_LCOV(NAME coverage
                                       EXECUTABLE ${TEST_EXECUTABLE_NAME}
                                       DEPENDENCIES ${TEST_EXECUTABLE_NAME})
    else()
        SETUP_TARGET_FOR_COVERAGE_GCOVR_HTML(NAME coverage
                                             EXECUTABLE ${TEST_EXECUTABLE_NAME}
                                             DEPENDENCIES ${TEST_EXECUTABLE_NAME})
    endif()

    add_definitions(-DDEBUG_PRINTF)

    set_target_properties(
        ${EXECUTABLE_NAME}
        PROPERTIES
            CXX_CLANG_TIDY "clang-tidy"
    )

endif (CMAKE_BUILD_TYPE MATCHES Debug)

################
# OPTIMIZATION #
################
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT error)
if (ipo_supported)
    message(STATUS "IPO/LTO enabled")
    set_property(TARGET ${EXECUTABLE_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
    set_property(TARGET ${TEST_EXECUTABLE_NAME} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(STATUS "IPO/LTO not supported")
endif()

