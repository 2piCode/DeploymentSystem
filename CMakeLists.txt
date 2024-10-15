cmake_minimum_required(VERSION 3.16)

project(DeldeliveryApp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH C:/msys64/mingw64/qt6-static)

# set(CMAKE_CXX_FLAGS_INIT-DQT_QML_DEBUG)
set(CMAKE_CXX_FLAGS_INIT "-DQT_QML_DEBUG")

set(LIBSSH2DIR CACHE STRING "LIBSSH2 static library directory")

set(LIBSSH2DIR C:/Projects/QML/DeploymentSystes/libssh2)

if(NOT LIBSSH2DIR)
    message(FATAL_ERROR "Please, specify LIBSSH2 directory via -DLIBSSH2DIR=<dir>")
endif()

message(STATUS "LIBSSH2 dir is ${LIBSSH2DIR}, change via -DLIBSSH2DIR=<dir>")

find_package(Qt6 6.7.2 REQUIRED COMPONENTS Quick LinguistTools)

set(HEADERS include/libssh2_manager.h
            include/ssh_connection.h
            include/station.h
            include/const.h)

set (SOURCES src/main.cpp
             src/ssh_connection.cpp
             src/station.cpp)

set (SOURCE_QML qml/Main.qml
                qml/StationItem.qml
                qml/ConnectionSettingsUI.qml
                qml/CustomToolBarButton.qml
                qml/FileSelection.qml
                qml/SettingsDialog.qml)

# Add translation files
set(TS_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/translations/translations_en.ts"
    "${CMAKE_CURRENT_SOURCE_DIR}/translations/translations_ru.ts"
)

# Set output directory for .qm files
set(QM_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/translations")
file(MAKE_DIRECTORY ${QM_OUTPUT_DIR})

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
find_program(Qt6_LUPDATE_EXECUTABLE lupdate HINTS "${CMAKE_PREFIX_PATH}/bin")
find_program(Qt6_LRELEASE_EXECUTABLE lrelease HINTS "${CMAKE_PREFIX_PATH}/bin")

# Update TS files using lupdate
add_custom_target(update_ts
    COMMAND Qt6::lupdate ${SOURCE_QML} -ts ${TS_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${SOURCE_QML}
    COMMENT "Updating translation (.ts) files"
)

# Generate QM files from TS files
set(QM_FILES)
foreach(TS_FILE ${TS_FILES})
    get_filename_component(TS_NAME ${TS_FILE} NAME)
    get_filename_component(TS_BASENAME ${TS_FILE} NAME_WE)
    set(QM_FILE "${QM_OUTPUT_DIR}/${TS_BASENAME}.qm")
    list(APPEND QM_FILES ${QM_FILE})

    add_custom_command(
        OUTPUT ${QM_FILE}
        COMMAND Qt6::lrelease ${TS_FILE} -qm ${QM_FILE}
        DEPENDS ${TS_FILE}
        COMMENT "Generating ${QM_FILE}"
    )
endforeach()


add_custom_target(translations ALL DEPENDS ${QM_FILES})
add_dependencies(translations update_ts)

qt_standard_project_setup()

qt_add_executable(deldeliveryApp
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

qt_add_qml_module(deldeliveryApp
    URI DeldeliveryApp
    VERSION 1.0
    QML_FILES
        ${SOURCE_QML}
        utils.js
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(deldeliveryApp PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appDeldeliveryApp
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
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
    "${LIBSSH2DIR}/include"
    "{CMAKE_CURRENT_BINARY_DIR}"/images
    "${CMAKE_SOURCE_DIR}/include"

)

include(GNUInstallDirs)
install(TARGETS deldeliveryApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Ensure the executable depends on the translations
add_dependencies(deldeliveryApp translations)
