#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

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
        unsigned int shaderProgram;
        float scaleFactor;
    public:
        OpenGLApp(GLFWwindow* window);
        bool parseShaders();
        void moveCamera(float x, float y);
        void zoomCamera(float zoom);
        void updateCamera();
        bool cameraUpdated();
        glm::mat4 getCamera();
        bool cameraUpdate;
        unsigned int getShaderProgram();
        float getScaleFactor();
        void setScaleFactor(float scaleFactor);
};


