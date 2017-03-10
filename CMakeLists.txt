cmake_minimum_required(VERSION 2.8)
SET(projectname Vulkanic)
set(VULKAN_DIR $ENV{VULKAN_SDK} CACHE FILEPATH "The Vulkan sdk dir")
set(GLFW_DIR $ENV{GLFW_DIR} CACHE FILEPATH "The GLFW3 dir")
include_directories(${VULKAN_DIR}/Include)
include_directories(${VULKAN_DIR}/Third-Party/Include)
include_directories(${GLFW_DIR}/include)
include_directories(include)
link_directories(${VULKAN_DIR}/Bin)
link_directories(${GLFW_DIR}/lib)

FILE(GLOB SRCFILES 
src/*.h
src/*.cpp
src/*.hpp
src/Engine/*.cpp
src/Engine/*.hpp
src/Engine/*.h
src/Engine/utils/*.cpp
src/Engine/utils/*.hpp
src/Engine/utils/*.h
include/*.hpp
include/*.h
include/*.cpp
)

add_executable(Vulkanic ${SRCFILES})

target_link_libraries(Vulkanic vulkan-1 glfw3)