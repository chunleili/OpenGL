#imgui 
add_library(imgui INTERFACE)
target_include_directories(imgui INTERFACE
 ${EXTERN_DIR}/imgui
 ${EXTERN_DIR}/imgui/backends
 )
file(GLOB IMGUI_FILES 
    ${EXTERN_DIR}/imgui/*.h 
    ${EXTERN_DIR}/imgui/*.cpp 
    ${EXTERN_DIR}/imgui/backends/imgui_impl_opengl3.*
    ${EXTERN_DIR}/imgui/backends/imgui_impl_glfw.*
    ${EXTERN_DIR}/imgui/backends/imgui_impl_opengl3_loader.h
)
target_sources(imgui INTERFACE ${IMGUI_FILES})

target_link_libraries(imgui INTERFACE glad glfw OpenGL::GL)

add_executable(imgui_example ${EXTERN_DIR}/imgui/examples/example_glfw_opengl3/main.cpp)
target_link_libraries(imgui_example PRIVATE imgui glad glfw)