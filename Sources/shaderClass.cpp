#include "shaderClass.h"
#include<string>
#include <filesystem>

std::string get_file_contents(const char* filename)
{
    // Very clunky fix-- Issue: ifstream doesn't find file not in current path...
    std::string shadersDir = std::string(std::filesystem::current_path())+"/Resources/Shaders/";

    std::ifstream in(shadersDir+filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    throw(errno);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    /*const char* vertexSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

    const char* fragmentSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
    "}\n\0";*/


    // Create Vertex Shader Object and get reference
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach Vertex Shader soure to the Vertex Shader Object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Compile the Vertex Shader into machine code
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");

    // Create Fragment Shader Object and get reference
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach Fragment Shader source to the Fragment Shader Object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // Compile the Fragment Shader into machine code
    glCompileShader(fragmentShader);
    compileErrors(fragmentShader, "FRAGMENT");

    // Create Shader Program Object and get reference
    ID = glCreateProgram();
    // Attach the Vertex and Fragment Shaders to the Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Wrap-up/Link all the shaders together into the Shader Program
    glLinkProgram(ID);
    compileErrors(ID, "PROGRAM");

    // Delete (now useless) Vertex and Fragment Shader Objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate()
{
    glUseProgram(ID);
}
void Shader::Delete()
{
    glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
    GLint hasCompiled;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if (hasCompiled == GL_FALSE)
        {
            glGetShaderInfoLog(shader,1024,NULL,infoLog);
            std::cout << "SHADER_COMPILATION_ERROR for:"<< type << "\n" << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
        if(hasCompiled == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "SHADER_LINKING_ERROR for:"<< type << "\n" << std::endl;
        }
    }
}