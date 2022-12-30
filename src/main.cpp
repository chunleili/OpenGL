// // #define GLEW_STATIC //由于已经在cmake中定义了, 所以这里不需要再定义了
// #include <GL/glew.h> // GLEW中包含了OpenGL的头文件, 因此要放在glfw的前面

#include <glad/glad.h> // GLAD
#include <GLFW/glfw3.h>// GLFW
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#undef NDEBUG//先去掉NDEBUG宏让断言发挥作用
#include <cassert>//记住一定要在上一行的后面

#include "indexBuffer.h"
#include "vertexBuffer.h"
#include "shader.h"

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// 键盘输入回调函数
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}


int main()
{
    std::cout << "Current working directory: " << std::filesystem::current_path() << '\n'; 

    //初始化GLFW
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    //创建GLFW窗口对象window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    //设置当前线程的上下文为window
    glfwMakeContextCurrent(window);

    //设置帧缓冲刷新间隔（垂直同步）
    glfwSwapInterval(1);

    //注册键盘输入回调函数
    glfwSetKeyCallback(window, keyCallback);  

    //初始化GLAD(如果采用GLEW, 请使用下面注释掉的代码)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // //初始化GLEW
    // glewExperimental = GL_TRUE;
    // if (glewInit() != GLEW_OK)
    // {
    //     std::cout << "Failed to initialize GLEW" << std::endl;
    //     return -1;
    // }

    //设置视口
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    const std::string fragShaderPath = "res/shader/fragShader.glsl";
    const std::string vertexShaderPath = "res/shader/vertexShader.glsl";
    Shader shader(vertexShaderPath, fragShaderPath);
    // shader.use();

{

    //绘制三角形
    unsigned int numTriangles = 2;
    unsigned int numIndices = 6;
    // 顶点数据, 三角形
    float vertices[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f, 0.5f,
        -0.5f, 0.5f,
        };
    
    unsigned int indices[] = {
        0, 1, 2, // 第一个三角形
        2, 3, 0  // 第二个三角形
    };

    //VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    //VBO
    VertexBuffer vbo(vertices, sizeof(vertices));
    //IBO
    IndexBuffer ibo(indices, numIndices);

    float red = 0;
    float inc = 0.01f;

    //渲染循环
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        //清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        red += inc;
        if (red > 1.0f)
            inc*=-1;
        if (red < 0.0f)
            inc*=-1;

        shader.use();
        shader.setVec4("u_color", glm::vec4(red, 0.2f, 0.3f, 1.0f));
        // glBindVertexArray(VAO);
        // ibo.bind();
        // glDrawArrays(GL_TRIANGLES, 0, numTriangles*3);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }
}

    //释放资源
    glfwTerminate();
    return 0;
}