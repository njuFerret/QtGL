cmake_minimum_required(VERSION 3.5)

set (PROJECT_NAME OGL06)

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

set(ASSIMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/Assimp/lib/cmake/assimp-5.0)

find_package(ASSIMP REQUIRED)
include_directories(${ASSIMP_INCLUDE_DIRS})
link_directories(${ASSIMP_LIBRARY_DIRS})
#message(${ASSIMP_DIR}
#message(${ASSIMP_INCLUDE_DIRS})
message(${ASSIMP_LIBRARIES})
#message(${ASSIMP_LIBRARY_DIRS})

find_package(Qt5 COMPONENTS Widgets REQUIRED )

file(GLOB SRC_LIST  *.h *.cpp *.ui *.qrc)
add_executable(${PROJECT_NAME}  ${SRC_LIST})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets ${ASSIMP_LIBRARIES} )
