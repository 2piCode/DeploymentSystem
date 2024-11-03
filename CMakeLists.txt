cmake_minimum_required(VERSION 3.16)

project(DeldeliveryApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.4.2 REQUIRED COMPONENTS Quick)

set(LIBSSH2DIR CACHE STRING "LIBSSH2 static library directory")

if(NOT LIBSSH2DIR)
    message(FATAL_ERROR "Please, specify LIBSSH2 directory via -DLIBSSH2DIR=<dir>")
endif()

message(STATUS "LIBSSH2 dir is ${LIBSSH2DIR}, change via -DLIBSSH2DIR=<dir>")

set(LIBPUGIXML CACHE STRING "LIBPUGIXML static library directory")

if(NOT LIBPUGIXML)
    message(FATAL_ERROR "Please, specify LIBPUGIXML directory via -DLIBPUGIXML=<dir>")
endif()

message(STATUS "LIBPUGIXML dir is ${LIBPUGIXML}, change via -DLIBPUGIXML=<dir>")

set(HEADERS include/libssh2_manager.h
            include/ssh_connection.h
            include/station.h
            include/config.h
            include/user_settings.h
            include/roles.h
            include/systems.h
            include/config_writer.h
            include/xml_config_writer.h
            include/station_builder.h
)

set (SOURCES src/main.cpp
             src/ssh_connection.cpp
             src/station.cpp
             src/config.cpp
             src/user_settings.cpp
             src/xml_config_writer.cpp
             src/roles.cpp
)

set (SOURCE_QML qml/Main.qml
                qml/StationItem.qml)

set (IMAGES images/back.png
            images/connection.png
            images/duplicate.png
            images/edit.png
            images/go.png
            images/load.png
            images/minus.png
            images/next.png
            images/plus.png
            images/save.png
            images/settings.png
            images/upload.png)

set(RESOURCES
    resources.qrc
)

set (PUGIXML_SOURCES
    ${LIBPUGIXML}/pugixml.hpp
    ${LIBPUGIXML}/pugiconfig.hpp
    ${LIBPUGIXML}/pugixml.cpp)

qt_standard_project_setup()

qt_add_executable(deldeliveryApp
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
    ${PUGIXML_SOURCES}
)

qt_add_qml_module(deldeliveryApp
    URI DeldeliveryApp
    VERSION 1.0
    QML_FILES
        ${SOURCE_QML}
        utils.js
        QML_FILES
)

target_link_directories(deldeliveryApp PUBLIC
    "${LIBSSH2DIR}/lib/$<IF:$<CONFIG:Debug>,Debug,Release>"
)

target_link_libraries(deldeliveryApp
    PRIVATE Qt6::Quick
    PRIVATE ssh2
)

target_include_directories(deldeliveryApp
    PUBLIC
    ${LIBSSH2DIR}/include
    ${LIBPUGIXML}
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/libs

)

include(GNUInstallDirs)
install(TARGETS deldeliveryApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
