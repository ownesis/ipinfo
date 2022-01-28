cmake_minimum_required(VERSION 3.0)

project(libipinfo VERSION 0.1.0 DESCRIPTION "C library for https://ipapi.co")

set(SRCS
    ipinfo.c
    )

set(HDRS
    ipinfo.h
    )

add_library(ipinfo_shared SHARED ${SRCS} ${HRDS})
set_target_properties(ipinfo_shared PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(ipinfo_shared PROPERTIES SOVERSION ${PROJECT_VERSION})

add_library(ipinfo_static STATIC ${SRCS} ${HRDS})
set_target_properties(ipinfo_static PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(ipinfo_static PROPERTIES SOVERSION ${PROJECT_VERSION})


include(GNUInstallDirs)

install(TARGETS ipinfo_shared
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS ipinfo_static
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})


find_package(CURL)
find_package(JSON-C)

if(CURL_FOUND)
  # Une fois la dépendance trouvée, nous l'incluons au projet
  target_include_directories(ipinfo_shared PUBLIC ${CURL_INCLUDE_DIR})
  target_link_libraries(ipinfo_shared ${CURL_LIBRARY})

  target_include_directories(ipinfo_static PUBLIC ${CURL_INCLUDE_DIR})
  target_link_libraries(ipinfo_static ${CURL_LIBRARY})
else()
  # Sinon, nous affichons un message
  message(FATAL_ERROR "libcurl not found")
endif()

if(JSON-C_FOUND)
  # Une fois la dépendance trouvée, nous l'incluons au projet
  target_include_directories(ipinfo_shared PUBLIC ${JSON-C_INCLUDE_DIR})
  target_link_libraries(ipinfo_shared ${JSON-C_LIBRARY})

  target_include_directories(ipinfo_static PUBLIC ${JSON-C_INCLUDE_DIR})
  target_link_libraries(ipinfo_static ${JSON-C_LIBRARY})
else()
  # Sinon, nous affichons un message
  message(FATAL_ERROR "libjson-c not found")
endif()
