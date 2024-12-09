cmake_minimum_required(VERSION 3.16)

project(LDAPSetup  VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS "-static")

set(LIBPUGIXML CACHE STRING "LIBPUGIXML static library directory")

if(NOT LIBPUGIXML)
    message(FATAL_ERROR "Please, specify LIBPUGIXML directory via -DLIBPUGIXML=<dir>")
endif()

message(STATUS "LIBPUGIXML dir is ${LIBPUGIXML}, change via -DLIBPUGIXML=<dir>")

set(HEADERS include/ldap_setup.h)

set (SOURCES src/main.cpp
             src/ldap_setup.cpp
             ${LIBPUGIXML}/pugixml.cpp
)

add_executable(ldap_setup ${SOURCES} ${HEADERS})

target_include_directories(ldap_setup PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${LIBPUGIXML}
)

find_package(Threads REQUIRED)
target_link_libraries(ldap_setup stdc++fs)

if(SYSTEM STREQUAL "astra")
    message("Building for AstraLinux")
    target_compile_definitions(ldap_setup PUBLIC ASTRA)
elseif(SYSTEM STREQUAL "redos")
    message("Building for RedOS")
    target_compile_definitions(ldap_setup PUBLIC REDOS)
endif()
