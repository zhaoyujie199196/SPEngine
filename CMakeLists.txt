cmake_minimum_required(VERSION 3.8)
PROJECT (SPEngine)

##添加第三方库路径
set(THIRDPARTY_DIR ./3rdparty)
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/glfw/include)
INCLUDE_DIRECTORIES(${THIRDPARTY_DIR}/glad/include)
LINK_DIRECTORIES(${THIRDPARTY_DIR}/glfw/libs/win64_debug)

#指定执行文件
ADD_EXECUTABLE(SPEngine sources/main.cpp)
#指定第三方库
TARGET_LINK_LIBRARIES(SPEngine PUBLIC glfw3)
#添加glad源代码
FILE(GLOB GLAD_SOURCE_C "${THIRDPARTY_DIR}/glad/src/*.c")
TARGET_SOURCES(SPEngine PUBLIC ${GLAD_SOURCE_C})

##主工程代码
FILE(GLOB SRC_LIST_CPP "${PROJECT_SOURCE_DIR}/sources/*.cpp")
FILE(GLOB SRC_LIST_H "${PROJECT_SOURCE_DIR}/source/*.h")
TARGET_SOURCES(SPEngine PUBLIC ${SRC_LIST_CPP})
MESSAGE("source files ${SRC_LIST_CPP}")

##demos代码
FILE(GLOB DEMO_SRC_LIST_CPP ${PROJECT_SOURCE_DIR}/sources/demos/openGLDemos/*.cpp)
FILE(GLOB DEMO_SRC_LIST_H ${PROJECT_SOURCE_DIR}/sources/demos/openGLDemos/*.h)
MESSAGE("demo source files ${DEMO_SRC_LIST_CPP}")
TARGET_SOURCES(SPEngine PUBLIC ${DEMO_SRC_LIST_CPP})


message("thirdparty = ${THIRDPARTY_DIR}")
