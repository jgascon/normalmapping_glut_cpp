cmake_minimum_required(VERSION 3.6)
if(COMMAND cmake_policy)
  #cmake_policy(SET CMP0003 NEW)
  cmake_policy(SET CMP0072 NEW)
endif(COMMAND cmake_policy)


PROJECT (normalmap_glut)

#Podemos forzar compilacion en modo Debug o Release aqui
#SET(CMAKE_BUILD_TYPE Release)
#SET(CMAKE_BUILD_TYPE Debug)

#Verificamos que glut esta instalado en el sistema
find_package(OpenGL REQUIRED)
IF(OpenGL_FOUND)
    MESSAGE("OpenGL GLVND Found")
    INCLUDE_DIRECTORIES(${OpenGL_INCLUDE_DIRS})
ELSE(OpenGL_FOUND)
    MESSAGE(FATAL_ERROR "OpenGL GLVND not found!")
ENDIF(OpenGL_FOUND)

#find_package(GLU REQUIRED)


find_package(GLUT REQUIRED)
IF(GLUT_FOUND)
    MESSAGE("GLUT Found")
    INCLUDE_DIRECTORIES(${GLUT_INCLUDE_DIRS})
ELSE(GLUT_FOUND)
    MESSAGE(FATAL_ERROR "GLUT not found!")
ENDIF(GLUT_FOUND)


# PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
# PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
# INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
# TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})

Find_Package (SDL2 REQUIRED)
IF(SDL2_FOUND)
    MESSAGE("SDL2 Found")
    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
ELSE(SDL2_FOUND)
    MESSAGE(FATAL_ERROR "SDL2 not found!")
ENDIF(SDL2_FOUND)


Find_Package (SDL2_IMAGE REQUIRED)
IF(SDL2_IMAGE_FOUND)
    MESSAGE("SDL2_IMAGE Found, version ${SDL2_IMAGE_VERSION_STRING}")
    INCLUDE_DIRECTORIES(${SDL2_IMAGE_INCLUDE_DIRS})
ELSE(SDL2_IMAGE_FOUND)
    MESSAGE(FATAL_ERROR "SDL2_IMAGE not found!")
ENDIF(SDL2_IMAGE_FOUND)


# Find_Package (SDL_IMAGE REQUIRED)

#Añadimos directorios include de nuestro proyecto
include_directories ("${PROJECT_SOURCE_DIR}")
#include_directories ("/usr/lib")

#include_directories ("${PROJECT_SOURCE_DIR}/../otro_path/inc")
# include_directories(${OPENGL_INCLUDE_DIRS})

#Creamos una libreria LibRisaVoxel.a con los ficheros auxiliates
#Definimos los ficheros .cpp usados en la libreria en una lista llamada objList
SET (objList
    textfile.cpp
    normal_map.cpp
)


# if(WIN32)
#     set(COMMON_LIBS normal_map ${OPENGL_LIBRARIES} optimized ${SDL_LIBRARY} SDLmain glut debug glut_d ${GLUT_LIBRARIES})
# elseif (UNIX)
#     set(COMMON_LIBS normal_map ${OPENGL_LIBRARIES} ${SDL_LIBRARY} ${GLUT_LIBRARIES})
#     #set(COMMON_LIBS normal_map ${OPENGL_LIBRARIES} glut ${SDL_LIBRARY} SDLmain  GL rt dl Xxf86vm X11 ${GLUT_LIBRARIES})
# else()
#     set(COMMON_LIBS normal_map)
# endif()


#Listado de ejecutables a crear con este CMakeList
ADD_EXECUTABLE(${PROJECT_NAME} ${EXECUTABLE_FLAG} ${objList})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} SDL2_image ${GLUT_LIBRARIES} ${OPENGL_LIBRARIES})

ADD_CUSTOM_TARGET(debug ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Debug ${PROJECT_SOURCE_DIR})
ADD_CUSTOM_TARGET(release ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE:STRING=Release ${PROJECT_SOURCE_DIR})
