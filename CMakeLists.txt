cmake_minimum_required(VERSION 2.8)
SET(projectname Vulkanic)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/modules/")

#the next code should not be neccesary but I cant get it to work
set(VULKAN_DIR $ENV{VULKAN_SDK} CACHE PATH "The Vulkan sdk dir")
set(GLFW_DIR $ENV{GLFW_DIR} CACHE PATH "The GLFW3 dir")
include_directories(${VULKAN_DIR}/Include)
include_directories(${GLFW_DIR}/include)
link_directories(${VULKAN_DIR}/Bin)
link_directories(${GLFW_DIR}/lib)

find_package(Vulkan REQUIRED)
include_directories(${VULKAN_INCLUDE_DIRS})
message(STATUS ${VULKAN_INCLUDE_DIRS})
#find_package(boost)
#include_directories(${BOOST_INCLUDE_DIRS})
find_package(GLM REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})
#find_package(GLFW REQUIRED)
#include_directories(${GLFW_INCLUDE_DIRS})
include_directories(include)

FILE(GLOB SRCFILES 
src/*.h
src/*.cpp
src/*.hpp
src/Engine/*.cpp
src/Engine/*.hpp
src/Engine/*.h
src/Engine/Example/*.cpp
src/Engine/Example/*.hpp
src/Engine/Example/*.h
include/*.hpp
include/*.h
include/*.cpp
)

add_executable(Vulkanic ${SRCFILES})

target_link_libraries(Vulkanic vulkan-1 glfw3)