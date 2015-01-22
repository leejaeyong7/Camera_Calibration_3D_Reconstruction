cmake_minimum_required(VERSION 2.8)
# Project Name
PROJECT(FLTK_CC)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
#########################################################
# FIND GLUT
#########################################################
find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIRS})
link_directories(${GLUT_LIBRARY_DIRS})
add_definitions(${GLUT_DEFINITIONS})
if(NOT GLUT_FOUND)
    message(ERROR " GLUT not found!")
endif(NOT GLUT_FOUND)

#########################################################
# FIND OPENGL
#########################################################
find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})
if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

#########################################################
# FIND FLTK
#########################################################

find_package(FLTK REQUIRED)
include_directories(${FLTK_INCLUDE_DIRS})
link_directories(${FLTK_LIBRARY_DIRS})
add_definitions(${FLTK_DEFINITIONS})

#########################################################
# FIND OPENCV
#########################################################

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_CFLAGS_OTHER})


#########################################################
# FIND C++11
#########################################################

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif()

#########################################################
# FIND OPENNI
#########################################################
#include_directories( ${OPENNI_INCLUDE_DIR})
#link_directories(${OPENNI_LIB_DIR} ${OPENNI_PRIME_SENSOR_MODULE_BIN_DIR})


#########################################################
# Include Files
#########################################################
add_executable(Camera_Calibrator main.cpp camera_window.cpp  main_window.cpp configure_window.cpp 3D_reconstruction.cpp)

########################################################
# Linking & stuff
#########################################################

  # create the program “camera_calibrator”
  target_link_libraries(Camera_Calibrator ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} ${FLTK_LIBRARIES} ${OpenCV_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT} ${OPENNI_LIBRARIES}  -ldl)

