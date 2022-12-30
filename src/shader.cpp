#include "shader.h"

Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
    std::string vertexShaderSource = parseShader(vertexShaderPath);
    std::string fragmentShaderSource = parseShader(fragmentShaderPath);

    // std::cout<<"vertexShaderSource: "<<vertexShaderSource<<std::endl;
    // std::cout<<"fragmentShaderSource: "<<fragmentShaderSource<<std::endl;

    unsigned int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    //创建着色器程序
    m_id = glCreateProgram();
    std::cout<<"shader id: "<<m_id<<std::endl;

    //将着色器附加到着色器程序上，并链接着色器程序
    glAttachShader(m_id, vertexShader);
    glAttachShader(m_id, fragmentShader);
    glLinkProgram(m_id);

    // 检查链接错误
    GLint success;
    GLchar infoLog[512];
    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if(!success) 
    {
        glGetProgramInfoLog(m_id, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED\n\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}



Shader::~Shader()
{
}


// 读取着色器文件
std::string Shader::parseShader(const std::string& filepath)
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

unsigned int Shader::compileShader(const std::string & source, const unsigned int type)
{
    unsigned int id = glCreateShader(type); //shader id
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

void Shader::use() const
{
    glUseProgram(m_id);
}




void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value); 
}
void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(m_id, name.c_str()), value); 
}
void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(m_id, name.c_str()), value); 
} 
void Shader::setVec4(const std::string &name, glm::vec4 value) const
{ 
    glUniform4f(glGetUniformLocation(m_id, name.c_str()), value.x, value.y, value.z, value.w); 
}