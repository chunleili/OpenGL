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
# # # glad
# add_library(glad INTERFACE)
# target_include_directories(glad INTERFACE ${EXTERN_DIR}/glad/include)
# target_sources(glad INTERFACE ${EXTERN_DIR}/glad/src/glad.c)
# # eigen
# add_library(Eigen INTERFACE)
# target_include_directories(Eigen INTERFACE ${EXTERN_DIR}/eigen)
# # imgui
# add_library(imgui INTERFACE)
# target_include_directories(imgui INTERFACE ${EXTERN_DIR}/imgui)
# file(GLOB IMGUI_FILES ${EXTERN_DIR}/imgui/*.h ${EXTERN_DIR}/imgui/*.cpp ${EXTERN_DIR}/imgui/backends/*.h ${EXTERN_DIR}/imgui/backends/*.cpp)
# target_sources(imgui INTERFACE ${IMGUI_FILES})

# glm
add_subdirectory(extern/glm)
# opengl
# windows SDK中有opengl的库
find_package(OpenGL REQUIRED)
# glfw
add_subdirectory(extern/glfw)
# glew
# glew的cmake文件有问题，所以这里直接使用编译好的静态库
add_library(glew STATIC IMPORTED)#使用IMPORTED方式导入静态库
set_target_properties(glew PROPERTIES IMPORTED_LOCATION ${EXTERN_DIR}/glew/lib/Release/x64/glew32s.lib)#给出静态库的路径
target_include_directories(glew INTERFACE ${EXTERN_DIR}/glew/include)
target_compile_definitions(glew INTERFACE GLEW_STATIC)#使用静态库，需要定义这个宏

target_link_libraries(main PRIVATE
    OpenGL::GL
    glew
    glfw
    glm
    # assimp
    # glad
    # Eigen
)



# 假如使用了vcpkg，可以直接使用find_package来找到库
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
