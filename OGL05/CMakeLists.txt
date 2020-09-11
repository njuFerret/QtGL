cmake_minimum_required(VERSION 3.5)

set (PROJECT_NAME OGL05)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(${PROJECT_NAME} SHARED
    main.cpp
    mainwidget.cpp
    mainwidget.h
    mainwidget.ui
  )
else()
  file(GLOB SRC_LIST  *.h *.cpp *.ui *.qrc)
  add_executable(${PROJECT_NAME}  ${SRC_LIST})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)

# file(GLOB SRC_LIST  *.cpp *.ui *.qrc)
# foreach(file IN LISTS SRC_LIST)
#     message(STATUS ${file})
# endforeach(file)
