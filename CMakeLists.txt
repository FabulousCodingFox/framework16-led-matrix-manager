cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
project(framework16-led-matrix-manager VERSION 1.0)

# Source files
file(GLOB_RECURSE SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HDRS ${PROJECT_SOURCE_DIR}/src/*.hpp)
message("src files:")
foreach(file ${SRCS})
    message(STATUS ${file})
endforeach()
message("header files:")
foreach(file ${HDRS})
    message(STATUS ${file})
endforeach()
add_executable(${PROJECT_NAME} ${SRCS})

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Find and link to libusb
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB REQUIRED libusb-1.0)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBUSB_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBUSB_LIBRARIES})

# Find and link to spdlog
find_package(spdlog REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)
add_definitions(-DSPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE)

# FFTW
pkg_check_modules(FFTW3 REQUIRED fftw3)
target_include_directories(${PROJECT_NAME} PRIVATE ${FFTW3_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${FFTW3_LIBRARIES})

# Alsa
pkg_check_modules(ALSA REQUIRED alsa)
target_include_directories(${PROJECT_NAME} PRIVATE ${ALSA_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${ALSA_LIBRARIES})

# Pipewire
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)
target_include_directories(${PROJECT_NAME} PRIVATE ${PIPEWIRE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PIPEWIRE_LIBRARIES})

# Find and link to Qt
find_package(Qt6 6.8.1 COMPONENTS Core Widgets LinguistTools REQUIRED)
qt_standard_project_setup()
qt_add_resources(resources_qrc resources/resources.qrc)
target_sources(${PROJECT_NAME} PRIVATE ${resources_qrc})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt::Core Qt::Widgets)
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
qt_generate_deploy_app_script(
    TARGET ${PROJECT_NAME}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
qt6_wrap_cpp(MOC_SRCS ${HDRS})
target_sources(${PROJECT_NAME} PRIVATE ${MOC_SRCS})
