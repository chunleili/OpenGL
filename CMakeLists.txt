cmake_minimum_required(VERSION 3.23)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(OpenGL CXX)

# set default build type to release
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

# 设置输出目录为build/bin
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_DEBUG_POSTFIX "_d")

# compile the source
add_executable(main "")
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
target_include_directories(main PRIVATE ${SRC_DIR}/include)
file(GLOB_RECURSE SRC_FILES ${SRC_DIR}/*.cpp ${SRC_DIR}/*.h)
# set(SRC_FILES 
# ${SRC_DIR}/indexBuffer.cpp
# ${SRC_DIR}/indexBuffer.h
# ${SRC_DIR}/main.cpp
# ${SRC_DIR}/renderer.cpp
# ${SRC_DIR}/renderer.h
# ${SRC_DIR}/shader.cpp
# ${SRC_DIR}/shader.h
# ${SRC_DIR}/vertexBuffer.cpp
# ${SRC_DIR}/vertexBuffer.h
# )
target_sources(main PRIVATE ${SRC_FILES})


# 复制shader等到build/bin目录
# file(COPY ${PROJECT_SOURCE_DIR}/res DESTINATION ${PROJECT_BINARY_DIR}/bin/$<CONFIG>)
add_custom_command(
        TARGET main  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/res ${PROJECT_BINARY_DIR}/bin/$<CONFIG>/res
    )

# -------------------------------------------------------------------------- #
#                           set external libraries                           #
# -------------------------------------------------------------------------- #
set(EXTERN_DIR ${PROJECT_SOURCE_DIR}/extern)
# # assimp
# add_subdirectory(extern/assimp)

# eigen
add_library(Eigen INTERFACE)
target_include_directories(Eigen INTERFACE ${EXTERN_DIR}/eigen)
# glm
add_subdirectory(extern/glm)
# opengl
find_package(OpenGL REQUIRED) # windows SDK中有opengl的库
# glfw
add_subdirectory(extern/glfw)
# glad
add_library(glad INTERFACE)
target_include_directories(glad INTERFACE ${EXTERN_DIR}/glad/include)
target_sources(glad INTERFACE ${EXTERN_DIR}/glad/src/glad.c)

#glad和glew使用一个即可。这里使用glad, 因此不再使用glew
option(USE_GLEW "use glew(Not recommend, now we use glad instead)" OFF)
if(USE_GLEW)
    add_subdirectory(extern/glew)
    # 如果使用GLEW 并且还要link一下！
endif()

#imgui 
add_subdirectory(extern/imgui)

target_link_libraries(main PRIVATE
    OpenGL::GL
    glad
    glfw
    glm
    Eigen
    imgui
    # assimp
)



# # 假如使用了vcpkg，可以直接使用find_package来找到库
# find_package(Eigen3 CONFIG REQUIRED)
# target_link_libraries(main PRIVATE Eigen3::Eigen)

# find_package(imgui CONFIG REQUIRED)
# target_link_libraries(main PRIVATE imgui::imgui)

# find_package(assimp CONFIG REQUIRED)
# target_link_libraries(main PRIVATE assimp::assimp)

# find_package(glfw3 CONFIG REQUIRED)
# target_link_libraries(main PRIVATE glfw)

# find_package(glad CONFIG REQUIRED)
# target_link_libraries(main PRIVATE glad::glad)

# find_package(glm CONFIG REQUIRED)
# target_link_libraries(main PRIVATE glm::glm)
