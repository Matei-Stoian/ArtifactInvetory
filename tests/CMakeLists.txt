cmake_minimum_required(VERSION 3.31)

# Enable testing
enable_testing()

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt packages
find_package(Qt6 COMPONENTS
    Core
    Gui
    Widgets
    Test
    REQUIRED)

# Define source files used in both main application and tests
set(COMMON_SOURCES
    ${CMAKE_SOURCE_DIR}/src/domain/Artifact.cpp
    ${CMAKE_SOURCE_DIR}/src/repository/BaseRepository.cpp
    ${CMAKE_SOURCE_DIR}/src/repository/CSVRepository.cpp
    ${CMAKE_SOURCE_DIR}/src/repository/JSONRepository.cpp
    ${CMAKE_SOURCE_DIR}/src/filters/ArtifactFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/filters/TypeFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/filters/PeriodFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/filters/LocationFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/filters/MaterialFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/filters/AndFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/filters/OrFilter.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/AddArtifactCommand.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/RemoveArtifactCommand.cpp
    ${CMAKE_SOURCE_DIR}/src/commands/UpdateArtifactCommand.cpp
    ${CMAKE_SOURCE_DIR}/src/controller/ArtifactController.cpp
)

# Create test executable
add_executable(artifact_tests
    main_test.cpp
    TestArtifact.cpp TestArtifact.h
    TestRepository.cpp TestRepository.h
    TestFilters.cpp TestFilters.h
    TestController.cpp TestController.h
    ${COMMON_SOURCES}
)

set_source_files_properties(
    TestArtifact.cpp
    TestRepository.cpp
    TestFilters.cpp
    TestController.cpp
    PROPERTIES SKIP_AUTOMOC TRUE
)

target_include_directories(artifact_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(artifact_tests PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Test
)

# Add tests to CTest
add_test(NAME ArtifactTests COMMAND artifact_tests)

# Add post-build commands to copy Qt DLLs for tests
if (WIN32)
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

    # Copy platform plugin
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET artifact_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:artifact_tests>/plugins/platforms/")
        add_custom_command(TARGET artifact_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:artifact_tests>/plugins/platforms/")
    endif ()

    # Copy Qt DLLs
    foreach(QT_LIB Core Gui Widgets Test)
        add_custom_command(TARGET artifact_tests POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:artifact_tests>")
    endforeach(QT_LIB)
endif()

