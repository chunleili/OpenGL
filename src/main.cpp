#include <iostream>
// GLEW中包含了OpenGL的头文件, 因此要放在glfw的前面
// #define GLEW_STATIC //由于已经在cmake中定义了, 所以这里不需要再定义了
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

// Window dimensions
const GLuint WIDTH = 1000, HEIGHT = 800;

// 键盘输入回调函数
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // 当用户按下ESC键,我们设置window窗口的WindowShouldClose属性为true
    // 关闭应用程序
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main()
{
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

    //注册键盘输入回调函数
    glfwSetKeyCallback(window, key_callback);  

    //初始化GLEW, 告知其使用modern mode
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    //设置视口
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    //渲染循环
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        //清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
    }

    //释放资源
    glfwTerminate();
    return 0;
}