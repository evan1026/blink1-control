function(install_boost_library_recursively library_name boost_version)
    CPMAddPackage(
        NAME boost-${library_name}
        VERSION ${boost_version}
        GIT_TAG boost-${boost_version}
        GITHUB_REPOSITORY "boostorg/${library_name}"
        EXCLUDE_FROM_ALL yes
    )

    set(PROCESSED_BOOST_LIBRARIES "${PROCESSED_BOOST_LIBRARIES};${library_name}" CACHE INTERNAL "")

    set_target_properties(boost_${library_name} PROPERTIES INTERFACE_SYSTEM_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:boost_${library_name},INTERFACE_INCLUDE_DIRECTORIES>)
    get_target_property(libs Boost::${library_name} INTERFACE_LINK_LIBRARIES)

    while (libs)

        list(GET libs 0 first_library)
        string(REPLACE "::" ";" library_name_parts ${first_library})
        list(GET library_name_parts 1 child_library_name)

        install_boost_library_recursively(${child_library_name} ${boost_version})

        list(REMOVE_AT libs 0)

    endwhile()
endfunction()



function(install_cpm CPM_DOWNLOAD_VERSION)
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
endfunction()
