#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstring>
#include <chrono>
#include <cmath>
#include "shape.cpp"
#include "planet.cpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
float* drawTriangle(float x, float y, float width, float height);
int drawCircle(float x, float y, float radius, unsigned int accuracy, float* vertices, unsigned int* indices);

// settings
#define ACCURACY 64
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "uniform mat4 transform;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "uniform vec4 aColor;\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(aColor);\n"
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Planet Simulation", NULL, NULL);
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
    float scaleFactor = 1.0/SIM_SIZE; 
    Planet planet1 = Planet("sun", 40000, {0, 0});
    Planet planet2 = Planet("earth", 6500, {-1500.0,500.0f});
    Planet planet3 = Planet("moon", 700, {-1600.0f,725.0f});
    Planet planet4 = Planet("Neptune", 350, {0.0f,1000.0f});
    float radius1 = std::sqrt(planet1.mass) * scaleFactor;
    float radius2 = std::sqrt(planet2.mass) * scaleFactor;
    float radius3 = std::sqrt(planet3.mass) * scaleFactor;
    float radius4 = std::sqrt(planet4.mass) * scaleFactor;
    Circle circle1 = Circle(shaderProgram, {0.0f,0.0f}, hex2rgb(0x90EE90), radius1, 64);
    Circle circle2 = Circle(shaderProgram, {planet2.position.x * scaleFactor,planet2.position.y * scaleFactor}, hex2rgb(0xFFA500), radius2, 64);
    Circle circle3 = Circle(shaderProgram, {planet3.position.x * scaleFactor,planet3.position.y * scaleFactor}, hex2rgb(0xFFC0CB), radius3, 64);
    Circle circle4 = Circle(shaderProgram, {planet4.position.x * scaleFactor,planet4.position.y*scaleFactor}, hex2rgb(0xFF0000), radius4, 64);
    planet2.velocity = {1.0f, 0.6f};
    planet3.velocity = {1.8f, 0.9f};
    // planet4.velocity = {0.4f, 0.0f};
    std::vector<Planet> planets;
    std::vector<Circle*> circles;
    planets.push_back(planet1);
    planets.push_back(planet2);
    planets.push_back(planet3);
    // planets.push_back(planet4);
    circles.push_back(&circle1);
    circles.push_back(&circle2);
    circles.push_back(&circle3);
    // circles.push_back(&circle4);

    // render loop
    // -----------

    std::chrono::time_point<std::chrono::system_clock> accStart,accEnd, animationStart;
    accStart = std::chrono::system_clock::now();
    animationStart = std::chrono::system_clock::now();

    //circle1.move();
    while (!glfwWindowShouldClose(window))
    {
        accEnd = std::chrono::system_clock::now();
        if(std::chrono::duration<double>(accEnd-accStart).count() > 0.001){
            for(int i = 0; i < planets.size(); i++){
                Planet *p1 = &planets[i];
                for(int j = i+1; j < planets.size(); j++){
                    Planet *p2 = &planets[j];
                    Vector2D force1 = p1->calculateGravityForce(*p2);
                    Vector2D force2 = p2->calculateGravityForce(*p1);
                    Vector2D acceleration1 = force1 * (1/p1->mass); 
                    Vector2D acceleration2 = force2 * (1/p2->mass); 

                    p1->velocity = p1->velocity + acceleration1;
                    p2->velocity = p2->velocity + acceleration2;
                    planets[0].velocity = {0,0};
                    p1->position = p1->position + p1->velocity;
                    p2->position = p2->position + p2->velocity;

                    circles[i]->move(p1->velocity.x * scaleFactor, p1->velocity.y*scaleFactor);
                    circles[j]->move(p2->velocity.x * scaleFactor, p2->velocity.y*scaleFactor);
                }
            }
            accStart = std::chrono::system_clock::now();
        } 

        // input
        // -----
        processInput(window);
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        for(Circle* circle: circles){
            circle->render();
        }

        // for(Circle circ: circles){
        //     circ.render();
        // }
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

