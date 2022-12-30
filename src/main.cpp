// // #define GLEW_STATIC //由于已经在cmake中定义了, 所以这里不需要再定义了
// #include <GL/glew.h> // GLEW中包含了OpenGL的头文件, 因此要放在glfw的前面

#include <glad/glad.h> // GLAD
#include <GLFW/glfw3.h>// GLFW

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

#undef NDEBUG//先去掉NDEBUG宏让断言发挥作用
#include <cassert>//记住一定要在上一行的后面

#include "indexBuffer.h"
#include "vertexBuffer.h"

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

// 读取着色器文件
static std::string parseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    std::string line;
    std::stringstream ss;

    while(getline(stream, line))
    {
        ss << line << std::endl;
    }

    return ss.str();
}

static unsigned int compileShader(const std::string & source, const unsigned int type)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    std::cout << "compileShader id: " << id << std::endl;

    // 检查编译错误
    int success;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int createShader()
{
    std::string  fragmentShaderSource  = parseShader("res/shader/fragShader.glsl");
    std::string  vertexShaderSource = parseShader("res/shader/vertexShader.glsl");
    unsigned int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    //创建着色器程序
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    std::cout<<"shaderProgram: "<<shaderProgram<<std::endl;

    //将着色器附加到着色器程序上，并链接着色器程序
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) 
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED\n\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
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

    GLuint shaderProgram =  createShader();
    glUseProgram(shaderProgram);

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

    
    int location = glGetUniformLocation(shaderProgram, "u_color");
    assert(location != -1);
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

        glUniform4f(location, red, 0.2f, 0.3f, 1.0f);

        // glDrawArrays(GL_TRIANGLES, 0, numTriangles*3);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);

        glfwSwapBuffers(window);
    }
}

    //释放资源
    glfwTerminate();
    return 0;
}