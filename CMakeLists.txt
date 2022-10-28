cmake_minimum_required(VERSION 3.10)

project(blink1-control)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAG_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG -g")
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(CMAKE_SCRIPT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include_directories(${INCLUDE_DIR})

set(CMAKE_BUILD_TYPE Release CACHE STRING "")
option(USE_BLINK1_TESTING_LIBRARY "" off)
option(BUILD_DOCS "" off)
option(BUILD_TESTS "" off)
option(RUN_CLANG_TIDY "" off)

add_definitions(-DUSE_HIDAPI)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG_PRINTF)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS yes)

include(${CMAKE_SCRIPT_DIR}/cpm_utilities.cmake)

set(         CPM_VERSION  0.36.0  )
set(        JSON_VERSION  3.11.2  )
set(  BLINK1_LIB_VERSION  1.1.0   )
set(     CXXOPTS_VERSION  3.0.0   )
set( GOOGLE_TEST_VERSION  1.12.1  )
set(       BOOST_VERSION  1.80.0  )

#######
# CPM #
#######
install_cpm(${CPM_VERSION})

CPMAddPackage("gh:nlohmann/json@${JSON_VERSION}")
CPMAddPackage("gh:evan1026/blink1-lib@${BLINK1_LIB_VERSION}")
CPMAddPackage("gh:jarro2783/cxxopts@${CXXOPTS_VERSION}")
CPMAddPackage("gh:google/googletest@${GOOGLE_TEST_VERSION}#release-${GOOGLE_TEST_VERSION}")
CPMAddPackage("gh:boostorg/boost@${BOOST_VERSION}#boost-${BOOST_VERSION}")

set(GTEST_LIBRARIES gtest gtest_main gmock gmock_main)

##################
# BLINK1-CONTROL #
##################
set(EXECUTABLE_NAME "blink1-control")

set(SOURCES
    ${SOURCE_DIR}/config/ConfigParser.cpp
    ${SOURCE_DIR}/config/PatternCommand.cpp
    ${SOURCE_DIR}/network/NetworkManager.cpp
)

set(EXECUTABLE_SOURCES
    ${SOURCES}
    ${SOURCE_DIR}/main.cpp
)

add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCES})
target_link_libraries(${EXECUTABLE_NAME} nlohmann_json Boost::asio Boost::thread cxxopts)

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
if (USE_BLINK1_TESTING_LIBRARY)
    add_definitions(-DUSE_BLINK1_TESTING_LIBRARY)
    target_link_libraries(${EXECUTABLE_NAME} blink1-testing)
else(USE_BLINK1_TESTING_LIBRARY)
    target_link_libraries(${EXECUTABLE_NAME} blink1)
endif(USE_BLINK1_TESTING_LIBRARY)

###########
# DOXYGEN #
###########
if (BUILD_DOCS)
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
endif(BUILD_DOCS)

###########
# Testing #
###########
if (BUILD_TESTS)
    set(TEST_EXECUTABLE_NAME ${EXECUTABLE_NAME}_test)
    set(TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

    set(TEST_SOURCES
        ${TEST_SOURCE_DIR}/config/ConfigParser_test.cpp
        ${TEST_SOURCE_DIR}/config/PatternCommand_test.cpp
    )

    enable_testing()

    add_executable(${TEST_EXECUTABLE_NAME} ${TEST_SOURCES} ${SOURCES})
    target_link_libraries(${TEST_EXECUTABLE_NAME} ${GTEST_LIBRARIES} blink1-testing nlohmann_json Boost::asio Boost::thread)
    set_property(TARGET ${TEST_EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)

    include(GoogleTest)
    gtest_discover_tests(${TEST_EXECUTABLE_NAME})

    ############
    # COVERAGE #
    ############
    list(APPEND CMAKE_MODULE_PATH "${LIB_DIR}/cmake-modules")
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

endif (BUILD_TESTS)

##############
# CLANG-TIDY #
##############
if (RUN_CLANG_TIDY)
    set_target_properties(
        ${EXECUTABLE_NAME}
        PROPERTIES
            CXX_CLANG_TIDY "clang-tidy"
    )
endif (RUN_CLANG_TIDY)

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

