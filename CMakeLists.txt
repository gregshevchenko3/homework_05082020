cmake_minimum_required(VERSION 3.5)

project(homework_05082020 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)


add_executable(homework_05082020
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    shapepropersdialog.cpp
    shapepropersdialog.h
    shapepropersdialog.ui
    shapepropmodel.cpp
    shapepropmodel.h
    graphicsview.h
    graphicsview.cpp
    gview.cpp
    gview.h
)

target_link_libraries(homework_05082020 PRIVATE Qt5::Widgets)
