SET (SUBPROJECT 0424_lucky_number)
PROJECT (${SUBPROJECT} C CXX)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)


SET (HEADERS

) 


SET (SOURCES
        0424_lucky_number.cpp
)

SET (INC_DIRS
   ${FULL_SOURCE_DIR}
)


SOURCE_GROUP ("Header Files" FILES ${HEADERS} )
SOURCE_GROUP ("Source Files" FILES ${SOURCES})

ADD_EXECUTABLE(${SUBPROJECT}  ${HEADERS} ${SOURCES}  )
TARGET_INCLUDE_DIRECTORIES(${SUBPROJECT} PUBLIC ${INC_DIRS})
TARGET_COMPILE_DEFINITIONS(${SUBPROJECT} PUBLIC ROOTDIR="${FULL_SOURCE_DIR}" TESTSAPPSRC="${${SUBPROJECT}_SOURCE_DIR}")



