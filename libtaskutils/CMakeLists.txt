SET (SUBPROJECT taskutils)
PROJECT (${SUBPROJECT} C CXX)


SET(CMAKE_INCLUDE_CURRENT_DIR ON)


SET (HEADERS
        python_utils.h
        pyobject_wrapper.h
        python_exceptions.h)

SET (SOURCES
        python_utils.cpp
        pyobject_wrapper.cpp python_exceptions.cpp)


#SET(Python3_VERSIONS 3.11 3.10 3.9 3.8 3.7 3.6 3.5 3.4 3.3 3.2 3.1 3.0)
FIND_PACKAGE(Python3 COMPONENTS Development)
IF (NOT Python3_Development_FOUND)
    MESSAGE(FATAL_ERROR "Python libs not found")
ENDIF(NOT Python3_Development_FOUND)

SET (INC_DIRS
   ${FULL_SOURCE_DIR} ${Python3_INCLUDE_DIRS}
)


SOURCE_GROUP ("Header Files" FILES ${HEADERS} )
SOURCE_GROUP ("Source Files" FILES ${SOURCES})

ADD_LIBRARY(${SUBPROJECT}  ${HEADERS} ${SOURCES}  )
TARGET_INCLUDE_DIRECTORIES(${SUBPROJECT} PUBLIC ${INC_DIRS})
TARGET_COMPILE_DEFINITIONS(${SUBPROJECT} PUBLIC ROOTDIR="${FULL_SOURCE_DIR}")
TARGET_LINK_LIBRARIES(${SUBPROJECT} ${Python_LIBRARIES})



