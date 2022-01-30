cmake_minimum_required(VERSION 3.0)

project(libipinfo VERSION 0.1.0 DESCRIPTION "C library for https://ipapi.co/")

set(SRCS
    ipinfo.c
    )

set(HDRS
    ipinfo.h
    )

add_library(ipinfo SHARED ${SRCS} ${HRDS})
set_target_properties(ipinfo PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(ipinfo PROPERTIES SOVERSION ${PROJECT_VERSION})

add_library(ipinfo_static STATIC ${SRCS} ${HRDS})
set_target_properties(ipinfo_static PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(ipinfo_static PROPERTIES SOVERSION ${PROJECT_VERSION})

include(GNUInstallDirs)

install(TARGETS ipinfo
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS ipinfo_static
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(FILES "ipinfo.h" DESTINATION "include/")

find_package(CURL REQUIRED)
find_package(JSON-C CONFIG REQUIRED)

if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
    target_link_libraries(ipinfo PRIVATE ${CURL_LIBRARY})
    target_link_libraries(ipinfo_static PRIVATE ${CURL_LIBRARY})
else()
    message(FATAL_ERROR "libcurl not found")
endif()
if(JSON-C_FOUND)
    target_link_libraries(ipinfo PRIVATE json-c::json-c)
    target_link_libraries(ipinfo_static PRIVATE json-c::json-c)
else()
    message(FATAL_ERROR "json-c not found")
endif()
