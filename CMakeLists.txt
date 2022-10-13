cmake_minimum_required(VERSION 3.10)

project(blink1-control)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAG_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -DDEBUG -g")
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${INCLUDE_DIR})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_definitions(-DUSE_HIDAPI)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG_PRINTF)
endif (CMAKE_BUILD_TYPE MATCHES Debug)

set(CMAKE_EXPORT_COMPILE_COMMANDS yes)

#######
# CPM #
#######
set(CPM_DOWNLOAD_VERSION 0.36.0)

if(CPM_SOURCE_CACHE)
  set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
  set(CPM_DOWNLOAD_LOCATION "$ENV{CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
  set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
  message(STATUS "Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
  file(DOWNLOAD
       https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
       ${CPM_DOWNLOAD_LOCATION}
  )
endif()

include(${CPM_DOWNLOAD_LOCATION})

####################
# CPM DEPENDENCIES #
####################
set(JSON_VERSION         3.11.2  )
set(BLINK1_LIB_VERSION   1.1.0   )
set(CXXOPTS_VERSION      3.0.0   )
set(GOOGLE_TEST_VERSION  1.12.1  )
set(BOOST_VERSION        1.80.0  )
CPMAddPackage("gh:nlohmann/json@${JSON_VERSION}")
CPMAddPackage("gh:evan1026/blink1-lib@${BLINK1_LIB_VERSION}")
CPMAddPackage("gh:jarro2783/cxxopts@${CXXOPTS_VERSION}")
CPMAddPackage("gh:google/googletest@${GOOGLE_TEST_VERSION}#release-${GOOGLE_TEST_VERSION}")

set(GTEST_LIBRARIES gtest gtest_main gmock gmock_main)

CPMAddPackage("gh:boostorg/asio@${BOOST_VERSION}#boost-${BOOST_VERSION}")

get_target_property(asio_libs Boost::asio INTERFACE_LINK_LIBRARIES)
set(processed_libs "")
while (asio_libs)

    list(GET asio_libs 0 first_library)

    string(REPLACE "::" ";" library_name_parts ${first_library})
    list(GET library_name_parts 1 library_name)

    CPMAddPackage("gh:boostorg/${library_name}@${BOOST_VERSION}#boost-${BOOST_VERSION}")

    get_target_property(new_lib_libs ${first_library} INTERFACE_LINK_LIBRARIES)

    if (new_lib_libs)
        foreach (lib ${new_lib_libs})
            if (NOT ${lib} IN_LIST processed_libs AND NOT ${lib} IN_LIST asio_libs)
                list(APPEND asio_libs ${lib})
            endif()
        endforeach()
    endif()

    list(APPEND processed_libs ${first_library})
    list(REMOVE_AT asio_libs 0)

endwhile (asio_libs)

##################
# BLINK1-CONTROL #
##################
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
target_link_libraries(${EXECUTABLE_NAME} nlohmann_json Boost::asio cxxopts)

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

###########
# Testing #
###########
set(TEST_EXECUTABLE_NAME ${EXECUTABLE_NAME}_test)
set(TEST_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)

set(TEST_SOURCES
    ${TEST_SOURCE_DIR}/config/ConfigParser_test.cpp
    ${TEST_SOURCE_DIR}/config/PatternCommand_test.cpp
)

enable_testing()

add_executable(${TEST_EXECUTABLE_NAME} ${TEST_SOURCES} ${SOURCES})
target_link_libraries(${TEST_EXECUTABLE_NAME} ${GTEST_LIBRARIES} blink1-testing nlohmann_json)
set_property(TARGET ${TEST_EXECUTABLE_NAME} PROPERTY CXX_STANDARD 20)

include(GoogleTest)
gtest_discover_tests(${TEST_EXECUTABLE_NAME})

############
# COVERAGE #
############
if (CMAKE_BUILD_TYPE MATCHES Debug)
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

