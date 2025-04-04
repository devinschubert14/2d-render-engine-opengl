#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ios>
#include <iostream>
#include <cstring>
#include <chrono>
#include <cmath>
#include "shape.hpp"
#include "planet.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
#define ACCURACY 64
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

//TODO: These should be actual shader files :P
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

typedef struct {
    float left = -1.0f;
    float right = 1.0f;
    float top = 1.0f;
    float bottom = -1.0f;
    float near = -1.0f;
    float far = 1.0f;
}OrthoMatrix;

class OpenGLApp{
    private:
        glm::vec3 eye;
        glm::mat4 cameraMatrix;
        float zoomLevel;
        OrthoMatrix orthoInfo;
        GLFWwindow* window;
    public:
        OpenGLApp(GLFWwindow* window);
        void moveCamera(float x, float y);
        void zoomCamera(float zoom);
        void updateCamera();
        bool cameraUpdated();
        glm::mat4 getCamera();
        bool cameraUpdate;
};

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

OpenGLApp app = OpenGLApp(nullptr);


//TODO: These should probably be added to OpenGLApp, they are needed by planet.hpp for shape.hpp
//Deciding how planet and circle interact is tough
unsigned int shaderProgram;
float scaleFactor;
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
int main()
{
    /* GLFW */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /* Window */
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Planet Simulation", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scrollCallback);

    /* GLAD */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    app = OpenGLApp(window);
    /* Shaders */
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
    shaderProgram = glCreateProgram();
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

    /* Scale Factor */
    scaleFactor = 1.0/SIM_SIZE; //Should be part of OpenGLApp probably

    /* Default settings */
    RGB backgroundColor = hex2rgb(0x000000);

    /* Planets */
    Planet planet1 = Planet("sun", 1000, {0, 0}, hex2rgb(0x90EE90));
    Planet planet2 = Planet("earth", 5.97, {-350.0,200.0f}, hex2rgb(0xFFA500));
    Planet planet3 = Planet("jupiter", 12, {650.0f,350.0f}, hex2rgb(0xFFC0CB));
    Planet planet4 = Planet("moon", 1, {-350.0f,210.0f}, hex2rgb(0xFF0000));
    planet2.velocity = {0.05f, 0.07f};
    planet3.velocity = {0.0f, -0.05f};
    planet4.velocity = {0.048f, 0.07f};
    std::vector<Planet> planets;
    std::vector<Circle*> circles;
    planets.push_back(planet1);
    planets.push_back(planet2);
    planets.push_back(planet3);
    planets.push_back(planet4);

    /* Frame timers */
    std::chrono::time_point<std::chrono::high_resolution_clock> frameEnd, animationStart;
    animationStart = std::chrono::high_resolution_clock::now();
    frameEnd = std::chrono::high_resolution_clock::now();


    /* Render loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Calculate frame time */
        double dt = std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - frameEnd).count();
        dt *= 10000000; //Animation step speed

        /* Apply forces */
        //This loop makes sure planets are compared once
        for(int i = 0; i < planets.size(); i++){
            //First planet
            Planet *p1 = &planets[i];
            for(int j = i+1; j < planets.size(); j++){
                //Second planet
                Planet *p2 = &planets[j];
                //Gather forces
                Vector2D force1 = p1->calculateGravityForce(*p2);
                Vector2D force2 = p2->calculateGravityForce(*p1);

                //Calculate acceleration
                //TODO: Calculate position and acceleration in planet class function
                Vector2D acceleration1 = force1 * (1/p1->mass); 
                Vector2D acceleration2 = force2 * (1/p2->mass); 

                //Calculate velocity
                p1->velocity = p1->velocity + acceleration1 * dt;
                p2->velocity = p2->velocity + acceleration2 * dt;
                planets[0].velocity = {0,0}; //Hard coded a 'sun' to not move for now, animation looks better

                //Calculate position
                p1->position = p1->position + p1->velocity * dt;
                p2->position = p2->position + p2->velocity * dt;

                //Move planets
                //Probably should just be a wrapper around circle->move i.e: planet->move will call circle->move
                planets[i].circle->move(p1->velocity.x * scaleFactor * dt, p1->velocity.y*scaleFactor * dt);
                planets[j].circle->move(p2->velocity.x * scaleFactor * dt, p2->velocity.y*scaleFactor * dt);
            }
        } 

        /* User Input */
        processInput(window);

        /* Render */

        //Clear screen
        glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //Add planets
        for(Planet planet: planets){
            //Probably should just be a wrapper around circle->render i.e: planet->render will call circle->render
            planet.circle->render(app.getCamera());
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

        //End of frame
        frameEnd = std::chrono::high_resolution_clock::now();
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
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        app.moveCamera(0.0f, -0.001f);
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        app.moveCamera(0.0f, 0.001f);
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        app.moveCamera(-0.001f, 0.0f);
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        app.moveCamera(0.001f, 0.0f);
    }
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset){
    float zoomSensitivity = 0.1f; // Increase sensitivity for faster zoom changes
    
    if (yoffset > 0) {
        app.zoomCamera(1.0 - zoomSensitivity);
    } else if (yoffset < 0) {
        app.zoomCamera(1.0 + zoomSensitivity);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

