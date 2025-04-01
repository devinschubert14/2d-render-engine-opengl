#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstring>
#include <chrono>
#include <cmath>
#include "Shape.cpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
float* drawTriangle(float x, float y, float width, float height);
int drawCircle(float x, float y, float radius, unsigned int accuracy, float* vertices, unsigned int* indices);

// settings
#define PI 3.14159265358979323846
#define ACCURACY 64
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    std::vector<float> vertices = {
        0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left
   };

    std::vector<unsigned int> indices = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };  
    Square square1 = Square(0, {0.0f,0.0f}, {0.0f,0.0f,0.0f}, 1.0f);
    Shape shape1 = Shape(0, vertices, indices, {0.0f, 0.0f, 0.0f}, 2);
    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------

    std::chrono::time_point<std::chrono::system_clock> accStart,accEnd, animationStart;
    float dis = 0.0f;
    float vel = 0.0f;
    float acc = 0.0f;
    float momentum = 0.005f;
    int bounces = 0;
    accStart = std::chrono::system_clock::now();
    animationStart = std::chrono::system_clock::now();

    while (!glfwWindowShouldClose(window))
    {
        accEnd = std::chrono::system_clock::now();
        if(std::chrono::duration<double>(accEnd-accStart).count() > 0.1){
            accStart = std::chrono::system_clock::now();
            dis += vel;
            //if(dis < -0.75f){
            if(dis > 1.5f){
                vel = -vel;
                acc = -(acc - (0.1f * (bounces + 1)));
                bounces++;
            }
            //if(dis > 0.75f){
            //    vel += acc;
            //    acc += 0.1f;
            //}
            vel += acc;
            acc += 0.007f;
        }

        
    //   if(std::chrono::duration<double>(accEnd-animationStart).count() > 10){
        //    float dis = 0.0f;
        //    float vel = 0.0f;
        //    float acc = 0.0f;
        //    int bounces = 0;
        //    animationStart = std::chrono::system_clock::now();
        //}

        // input
        // -----
        processInput(window);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        //Draw circle 1
        square1.render();
        // shape1.render();
 
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
   }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    //glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

float* drawTriangle(float x, float y, float width, float height){
    if(x > 1.0 || x < -1.0 || y > 1.0 || y < -1.0 || height > 1.0 || height < -1.0){
        exit(1);
    }

    float v1[] = {x, y+(height/2), 0.0f};
    float v2[] = {x-(height/2), y-(height/2), 0.0f};
    float v3[] = {x+(height/2), y-(height/2), 0.0f};
   
    float* arr = (float *)malloc(9 * sizeof(float));
    memcpy(&arr[0], v1, sizeof(v1));
    memcpy(&arr[3], v2, sizeof(v2));
    memcpy(&arr[6], v3, sizeof(v3));
    return arr;
}

// int drawCircle(float x, float y, float radius, unsigned int accuracy, float* vertices, unsigned int* indices){ if(x > 1.0f || x < -1.0f || y > 1.0f || y < -1.0f || radius > 1.0f || radius < -1.0f){
//         return false;
//     }
//
//     float angleStep = 2 * PI / accuracy; 
//     vertices[0] = x;
//     vertices[1] = y;
//     vertices[2] = 0.0f;
//     for(int i = 0; i < accuracy; i++){
//         float angle =  i * angleStep;
//         vertices[(VERTEX_COORD_TOTAL*(i+1))] = x + radius * cos(angle);
//         vertices[(VERTEX_COORD_TOTAL*(i+1))+1] = y + radius * sin(angle);
//         vertices[(VERTEX_COORD_TOTAL*(i+1))+2] = 0.0f;
//     }
//     for(int i = 0; i < accuracy; i++){
//         indices[i*VERTEX_COORD_TOTAL] = 0;
//         indices[(i*VERTEX_COORD_TOTAL)+1] = i + 1;
//         //indices[(i*VERTEX_COORD_TOTAL)+2] = i + 2;
//         indices[(i * VERTEX_COORD_TOTAL) + 2] = (i + 1) % accuracy + 1;
//     }
//     return true; 
// }
