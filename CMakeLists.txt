cmake_minimum_required(VERSION 3.16)

project(
  DeldeliveryApp
  VERSION 0.1
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt5 5.15.2 REQUIRED COMPONENTS Core Qml Quick LinguistTools
                                            QuickControls2 QuickTemplates2)

set(LIBSSH2DIR CACHE STRING "LIBSSH2 static library directory")

if(NOT LIBSSH2DIR)
  message(
    FATAL_ERROR "Please, specify LIBSSH2 directory via -DLIBSSH2DIR=<dir>")
endif()

message(STATUS "LIBSSH2 dir is ${LIBSSH2DIR}, change via -DLIBSSH2DIR=<dir>")

set(LIBPUGIXML CACHE STRING "LIBPUGIXML static library directory")

if(NOT LIBPUGIXML)
  message(
    FATAL_ERROR "Please, specify LIBPUGIXML directory via -DLIBPUGIXML=<dir>")
endif()

set(HEADERS
    include/libssh2_manager.h
    include/ssh_connection.h
    include/station.h
    include/config.h
    include/user_settings.h
    include/roles.h
    include/systems.h
    include/config_writer.h
    include/xml_config_writer.h
    include/station_builder.h
    include/language_controller.h
    include/utils.h)

set(SOURCES
    src/main.cpp
    src/ssh_connection.cpp
    src/station.cpp
    src/systems.cpp
    src/config.cpp
    src/user_settings.cpp
    src/xml_config_writer.cpp
    src/roles.cpp
    src/language_controller.cpp)

set(SOURCE_QML
    qml/Main.qml
    qml/StationItem.qml
    qml/ConnectionSettingsUI.qml
    qml/InstallationSettingsUI.qml
    qml/StationSettingsUI.qml
    qml/CustomToolBarButton.qml
    qml/FileSelection.qml
    qml/SettingsDialog.qml)

# Add translation files
set(TS_FILES "${CMAKE_CURRENT_SOURCE_DIR}/translations/translations_en.ts"
             "${CMAKE_CURRENT_SOURCE_DIR}/translations/translations_ru.ts")

# Set output directory for .qm files
set(QM_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/translations")
file(MAKE_DIRECTORY ${QM_OUTPUT_DIR})

set(RESOURCES resources.qrc)

add_custom_target(
  update_ts ALL
  DEPENDS ${SOURCE_QML}
  COMMAND ${Qt5_LUPDATE_EXECUTABLE} ${SOURCE_QML} -ts ${TS_FILES}
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMENT "Updating translation (.ts) files")

# Generate QM files from TS files
foreach(TS_FILE ${TS_FILES})
  get_filename_component(TS_NAME ${TS_FILE} NAME)
  get_filename_component(TS_BASENAME ${TS_FILE} NAME_WE)
  set(QM_FILE "${QM_OUTPUT_DIR}/${TS_BASENAME}.qm")
  list(APPEND QM_FILES ${QM_FILE})

  add_custom_command(
    OUTPUT ${QM_FILE}
    COMMAND ${Qt5_LRELEASE_EXECUTABLE} ${TS_FILE} -qm ${QM_FILE}
    DEPENDS ${TS_FILE}
    COMMENT "Generating ${QM_FILE}")
endforeach()

add_custom_target(translations ALL DEPENDS ${QM_FILES})
add_dependencies(translations update_ts)

set(PUGIXML_SOURCES ${LIBPUGIXML}/pugixml.hpp ${LIBPUGIXML}/pugiconfig.hpp
                    ${LIBPUGIXML}/pugixml.cpp)

include_directories(${CMAKE_SOURCE_DIR}/libs/libssh2/include)

add_executable(deldeliveryApp ${SOURCES} ${HEADERS} ${SOURCE_QML} ${RESOURCES}
                              ${PUGIXML_SOURCES})

if(APPLE)
  find_library(METAL_LIB Metal)
  target_link_libraries(deldeliveryApp PRIVATE ${METAL_LIB})
endif()

target_link_directories(deldeliveryApp PUBLIC
                        ${CMAKE_SOURCE_DIR}/libs/libssh2/lib)

target_link_libraries(
  deldeliveryApp
  PRIVATE Qt5::Core
  PRIVATE Qt5::Qml
  PRIVATE Qt5::Quick
  PRIVATE Qt5::QuickControls2
  PRIVATE Qt5::QuickTemplates2
  PRIVATE ssh2 stdc++fs)

target_include_directories(
  deldeliveryApp PUBLIC ${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/libs
                        ${LIBSSH2DIR}/include ${LIBPUGIXML})

include(GNUInstallDirs)
install(
  TARGETS deldeliveryApp
  BUNDLE DESTINATION .
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

add_dependencies(deldeliveryApp translations)
