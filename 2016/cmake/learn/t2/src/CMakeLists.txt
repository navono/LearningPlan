SET(SRC_LIST main.cpp)

#SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
ADD_EXECUTABLE(hello ${SRC_LIST})

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)