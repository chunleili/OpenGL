if(USE_GLEW)
    # glew的cmake文件有问题，所以这里直接使用编译好的静态库(注意Windows64-only)
    add_library(glew STATIC IMPORTED)#使用IMPORTED方式导入静态库
    set_target_properties(glew PROPERTIES IMPORTED_LOCATION ${EXTERN_DIR}/glew/lib/Release/x64/glew32s.lib)#给出静态库的路径
    target_include_directories(glew INTERFACE ${EXTERN_DIR}/glew/include)
    target_compile_definitions(glew INTERFACE GLEW_STATIC)#使用静态库，需要定义这个宏
endif()