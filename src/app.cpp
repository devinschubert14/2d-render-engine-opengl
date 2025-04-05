#include "app.hpp"

OpenGLApp::OpenGLApp(GLFWwindow* window){
    this->window = window;
    this->eye = glm::vec3(0.0f, 0.0f, 0.0f);
    this->zoomLevel = 1.0f;
    cameraUpdate = false;
    moveCamera(0.0f, 0.0f);
}

void OpenGLApp::moveCamera(float x, float y){
    this->eye.x += x;
    if(this->eye.x > 1.0f){
       this->eye.x = 1.0f;
    }
    if(this->eye.x < -1.0f){
       this->eye.x = -1.0f;
    }

    this->eye.y += y;
    if(this->eye.y > 1.0f){
       this->eye.y = 1.0f;
    }
    if(this->eye.y < -1.0f){
       this->eye.y = -1.0f;
    }
    this->cameraUpdate = true;
    updateCamera();
}

void OpenGLApp::zoomCamera(float zoom){
    this->orthoInfo.left *= zoom;
    this->orthoInfo.right *= zoom;
    this->orthoInfo.top *= zoom;
    this->orthoInfo.bottom *= zoom;
    this->cameraUpdate = true;
    updateCamera();
}

void OpenGLApp::updateCamera(){
    glm::mat4 projection = glm::ortho(orthoInfo.left, orthoInfo.right, orthoInfo.bottom, orthoInfo.top, orthoInfo.near, orthoInfo.far);
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(this->eye.x, this->eye.y, this->eye.z));
    this->cameraMatrix = projection * view;
}

glm::mat4 OpenGLApp::getCamera(){
    return this->cameraMatrix;
}

bool OpenGLApp::parseShaders(){
    /* Vertex Shader */
    //Read source code to string
    std::ifstream file("../shaders/vertex.glsl");
    if(!file.is_open()){
        file.open("shaders/vertex.glsl");
    }
    if(!file.is_open()){
        std::cerr << "Error: Unable to find vertex shader...\n";
        return false;
    }
    std::stringstream buffer;
    std::string bufferString;
    const char* shaderCode;
    buffer << file.rdbuf();
    bufferString = buffer.str();
    shaderCode = bufferString.c_str();
    file.close();

    //Compile source code
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &shaderCode, NULL);
    glCompileShader(vertexShader);
    buffer.str("");

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /* Fragment Shader */
    //Read source code to string
    file.open("../shaders/frag.glsl");
    if(!file.is_open()){
        file = std::ifstream("shaders/frag.glsl");
    }
    
    if(!file.is_open()){
        std::cerr << "Error: Unable to find vertex shader...\n";
        return false;
    }
    buffer << file.rdbuf();
    bufferString = buffer.str();
    shaderCode = bufferString.c_str();
    file.close();

    //Compile source code
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &shaderCode, NULL);
    glCompileShader(fragmentShader);
    buffer.str("");

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return false;
    }
    // link shaders
    this->shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return false;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return true;
}

unsigned int OpenGLApp::getShaderProgram(){
    return this->shaderProgram;
}

float OpenGLApp::getScaleFactor(){
    return this->scaleFactor;
}
void OpenGLApp::setScaleFactor(float scaleFactor){
    this->scaleFactor = scaleFactor; 
}
