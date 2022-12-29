// // #define GLEW_STATIC //由于已经在cmake中定义了, 所以这里不需要再定义了
// #include <GL/glew.h> // GLEW中包含了OpenGL的头文件, 因此要放在glfw的前面

#include <glad/glad.h> // GLAD
#include <GLFW/glfw3.h>// GLFW

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>

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


unsigned int drawTriangle()
{
    //顶点数据, 三角形
    GLfloat vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f};

    //创建顶点缓冲对象
    GLuint VBO;
    glGenBuffers(1, &VBO);   
    //绑定顶点缓冲对象
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    //将顶点数据复制到顶点缓冲对象中
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    // 1. 绑定VAO
    glBindVertexArray(VAO);
    // 2. 把顶点数组复制到缓冲中供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 3. 设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return VAO;
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

    //绘制三角形
    unsigned int VAO = drawTriangle();

    //渲染循环
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        //清屏
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
    }

    //释放资源
    glfwTerminate();
    return 0;
}