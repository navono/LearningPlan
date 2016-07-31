SET(SRC main.cpp)
ADD_EXECUTABLE(main ${SRC})


#AUX_SOURCE_DIRECTORY(. SRC_LIST)
#作用是发现一个目录下所有的源代码文件并将列表存储在一个变量中
# ADD_EXECUTABLE(main ${SRC_LIST})
FOREACH(F ${SRC_LIST})
MESSAGE(STATUS ${F})
ENDFOREACH(F)

#添加动态库的头文件目录
#INCLUDE_DIRECTORIES("E:\\cmake\\tmp\\t3\\include\\hello")

FIND_PATH(myHeader NAME hello.h PATHS "E:\\cmake\\tmp\\t3\\include\\hello")
IF(myHeader)
message(STATUS "myHeader found")
INCLUDE_DIRECTORIES(${myHeader})
ENDIF(myHeader)


#理论上PATHS可以通过环境变量export CMAKE_INCLUDE_PATH="E:\\cmake\\tmp\\t3\\lib"来设置
#CMAKE_LIBRARY_PATH可以用在FIND_LIBRARY中
#实际测试不行
#FIND_LIBRARY(HELLO_PATH NAME hello)

#添加库路径。VS下貌似无效
#link_directories("E:\\cmake\\\tmp\\t3\\lib")

#FIND_LIBRARY貌似只适用于lib文件，而不是适用于dll文件
FIND_LIBRARY(HELLO_PATH NAMES hello PATHS "E:\\cmake\\tmp\\t3\\lib")

IF(HELLO_PATH)
message(STATUS "HELLO_PATH found ")
ENDIF(HELLO_PATH)

TARGET_LINK_LIBRARIES(main ${HELLO_PATH})


#以下均为绝对路径
#t4
MESSAGE(STATUS ${PROJECT_SOURCE_DIR})
#build
MESSAGE(STATUS ${PROJECT_BINARY_DIR})
#src
MESSAGE(STATUS ${CMAKE_CURRENT_SOURCE_DIR})
#为空
MESSAGE(STATUS ${CMAKE_CURRRENT_BINARY_DIR})
# src/CMakeLists.txt
MESSAGE(STATUS ${CMAKE_CURRENT_LIST_FILE})

#$ENV{HOME}无效
MESSAGE(STATUS “HOME dir: $ENV{HOME}”)



SET(BUILD_SHARED_LIBS ON)
FIND_LIBRARY(TEST_PATH NAME test PATHS "E:\\cmake\\tmp\\t3\\lib")
IF(NOT TEST_PATH)
message(STATUS "TEST_PATH not found ")
ENDIF(NOT TEST_PATH)
