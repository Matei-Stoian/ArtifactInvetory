cmake_minimum_required(VERSION 3.31)
project(ArtifactInentory)

# Enable testing
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.9.0/mingw_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Test
        REQUIRED)

add_executable(ArtifactInentory main.cpp
        src/domain/Artifact.cpp
        include/domain/Artifact.h
        src/repository/BaseRepository.cpp
        include/repository/BaseRepository.h
        src/repository/CSVRepository.cpp
        include/repository/CSVRepository.h
        src/repository/JSONRepository.cpp
        include/repository/JSONRepository.h
        include/commands/Command.h
        src/commands/AddArtifactCommand.cpp
        include/commands/AddArtifactCommand.h
        src/commands/RemoveArtifactCommand.cpp
        include/commands/RemoveArtifactCommand.h
        src/commands/UpdateArtifactCommand.cpp
        include/commands/UpdateArtifactCommand.h
        src/filters/ArtifactFilter.cpp
        include/filters/ArtifactFilter.h
        src/filters/TypeFilter.cpp
        include/filters/TypeFilter.h
        src/filters/PeriodFilter.cpp
        include/filters/PeriodFilter.h
        src/filters/LocationFilter.cpp
        include/filters/LocationFilter.h
        src/filters/MaterialFilter.cpp
        include/filters/MaterialFilter.h
        src/filters/AndFilter.cpp
        include/filters/AndFilter.h
        src/filters/OrFilter.cpp
        include/filters/OrFilter.h
        src/controller/ArtifactController.cpp
        include/controller/ArtifactController.h
        src/ui/MainWindow.cpp
        include/ui/MainWindow.h
        src/ui/ArtifactDialog.cpp
        include/ui/ArtifactDialog.h
)

target_link_libraries(ArtifactInentory
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

# Add tests subdirectory
add_subdirectory(tests)

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
