cmake_minimum_required(VERSION 3.16)

project(gwi VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Quick Charts)

# Detect if building on a RPi
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm" OR CMAKE_SYSTEM_PROCESSOR MATCHES "aarch64")
    set(IS_RASPBERRY_PI TRUE)
    message(STATUS "Building for Raspberry Pi (${CMAKE_SYSTEM_PROCESSOR})")
else()
    set(IS_RASPBERRY_PI FALSE)
    message(STATUS "Building for development platform (${CMAKE_SYSTEM_PROCESSOR})")
endif()

# Only find WiringPi on an RPi
if(IS_RASPBERRY_PI)
    find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
    if(NOT WIRINGPI_LIBRARIES)
        message(FATAL_ERROR "WiringPi library not found on Raspberry Pi")
    endif()
    add_compile_definitions(HAVE_WIRINGPI)
endif()

# find_library(WIRINGPI_LIBRARIES NAMES wiringPi)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(appgwi
    src/main.cpp
    src/ButtonHandler.cpp
)

qt_add_qml_module(appgwi
    URI gwi
    VERSION 1.0
    QML_FILES
        Main.qml
        SOURCES include/ButtonHandler.hpp
        RESOURCES resources/example.yml
        SOURCES include/fkYAML.hpp
        RESOURCES resources/resources.qrc
        SOURCES src/StateManager.cpp
        SOURCES include/StateManager.hpp
        QML_FILES IntensitySlider.qml
        QML_FILES AmplificationPlot.qml
        SOURCES include/DataManager.hpp
        SOURCES src/DataManager.cpp
        SOURCES include/SliderHandler.hpp
        SOURCES src/SliderHandler.cpp
        QML_FILES RawData.qml
        SOURCES include/RawDataModel.hpp
        SOURCES src/RawDataModel.cpp
        SOURCES include/HardwareController.hpp
        SOURCES src/HardwareController.cpp
        RESOURCES resources/last_led_intensity.txt
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appgwi PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appgwi
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_include_directories(appgwi PRIVATE
    include
)

target_link_libraries(appgwi PUBLIC
    Qt6::Quick
    Qt6::Charts
)

if(IS_RASPBERRY_PI)
    target_link_libraries(appgwi PUBLIC ${WIRINGPI_LIBRARIES})
endif()

include(GNUInstallDirs)
install(TARGETS appgwi
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
