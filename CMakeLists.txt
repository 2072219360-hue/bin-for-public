cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0091 NEW)

project(Dev701QtNSPlatformBase LANGUAGES CXX)

if(NOT WIN32)
    message(FATAL_ERROR "The conservative CMake prototype currently supports Windows only.")
endif()
if(NOT MSVC)
    message(FATAL_ERROR "The current binary dependency contract requires MSVC.")
endif()
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR "Only x64 is supported; configure with -A x64.")
endif()
if(MSVC_TOOLSET_VERSION AND NOT MSVC_TOOLSET_VERSION EQUAL 141)
    message(FATAL_ERROR "The baseline requires MSVC v141, found v${MSVC_TOOLSET_VERSION}.")
endif()

# The first migration milestone deliberately supports only the one known-good
# legacy configuration. Release is enabled after all third-party libraries can
# be rebuilt with a documented x64 Release ABI.
if(CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_CONFIGURATION_TYPES Debug CACHE STRING "Supported configurations" FORCE)
elseif(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(FATAL_ERROR "Configure this prototype with CMAKE_BUILD_TYPE=Debug.")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS
    "${PROJECT_SOURCE_DIR}/ui"
    "${PROJECT_SOURCE_DIR}/src/postprocessQWT3D"
    "${PROJECT_SOURCE_DIR}/src/testQGLViewer")

find_package(Qt5 5.14.1 REQUIRED COMPONENTS
    Core Gui Widgets Network OpenGL Xml Sql Charts)
find_package(VTK 8.2 REQUIRED)
find_package(OpenGL REQUIRED)

# VTK 8.2 still exports VTK_USE_FILE in common source builds.
if(VTK_USE_FILE)
    include("${VTK_USE_FILE}")
endif()

include(cmake/LegacySources.cmake)
include(cmake/LegacyDependencies.cmake)

add_executable(Dev701QtNSPlatformBase WIN32
    ${DEV701_APP_SOURCES}
    ${DEV701_LOG4QT_SOURCES}
    ${DEV701_MOC_HEADERS}
    ${DEV701_QT_RESOURCES}
    ${DEV701_UI_FORMS})

target_compile_definitions(Dev701QtNSPlatformBase PRIVATE
    _WINDOWS
    QT_LARGEFILE_SUPPORT
    ROCKMAN
    QT_DLL
    QWT_DLL
    QWT3D_DLL
    QT_THREAD_SUPPORT)

# Do not force a source encoding yet: the legacy tree mixes GBK and UTF-8 BOM
# files. Encoding normalization is deliberately outside this migration.
target_compile_options(Dev701QtNSPlatformBase PRIVATE /W3 /EHsc)

target_include_directories(Dev701QtNSPlatformBase PRIVATE
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/src"
    "${PROJECT_SOURCE_DIR}/src/log4qt"
    "${PROJECT_SOURCE_DIR}/src/qwt/include"
    "${PROJECT_SOURCE_DIR}/src/qwtplot3d/include"
    "${PROJECT_SOURCE_DIR}/src/QGLViewer/include"
    "${PROJECT_SOURCE_DIR}/src/testqwtplot3d/mesh2"
    "${PROJECT_SOURCE_DIR}/Qssh-master/src")

target_link_libraries(Dev701QtNSPlatformBase PRIVATE
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::OpenGL
    Qt5::Xml Qt5::Sql Qt5::Charts
    ${VTK_LIBRARIES}
    OpenGL::GL OpenGL::GLU
    legacy_qwt legacy_qwtplot3d legacy_qglviewer legacy_qssh
    legacy_jsoncpp legacy_jsoncpp_static
    gdi32 user32)

set_target_properties(Dev701QtNSPlatformBase PROPERTIES
    MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_SOURCE_DIR}/out/runtime/Debug")

# Only copy source-controlled, architecture-neutral runtime data. Dependency
# DLL deployment remains explicit until verified x64 binaries are available.
add_custom_command(TARGET Dev701QtNSPlatformBase POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${PROJECT_SOURCE_DIR}/huanglog4qt.conf"
        "$<TARGET_FILE_DIR:Dev701QtNSPlatformBase>/huanglog4qt.conf"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/images"
        "$<TARGET_FILE_DIR:Dev701QtNSPlatformBase>/images"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/sound"
        "$<TARGET_FILE_DIR:Dev701QtNSPlatformBase>/sound"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/qss"
        "$<TARGET_FILE_DIR:Dev701QtNSPlatformBase>/qss"
    COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${PROJECT_SOURCE_DIR}/lang"
        "$<TARGET_FILE_DIR:Dev701QtNSPlatformBase>/lang"
    COMMAND "${CMAKE_COMMAND}" -E copy_if_different
        "${PROJECT_SOURCE_DIR}/qt_zh_CN.qm"
        "$<TARGET_FILE_DIR:Dev701QtNSPlatformBase>/qt_zh_CN.qm"
    VERBATIM)
