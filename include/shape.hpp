#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "utils.hpp"

#define PI 3.14159265358979323846

class Shape{
    public:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        RGB color;
        unsigned int shader;
        unsigned int vao, vbo, ebo;
        unsigned int numElements;
        unsigned int numComponents;
        glm::mat4 trans;
        Shape(unsigned int shader, std::vector<float> vertices, std::vector<unsigned int> indices, RGB color, int numElements);
        ~Shape();
        void createVAO(unsigned int shader);
        void createVBO();
        void createEBO();
        void render(const glm::mat4& mvMatrix);
        void move(float x, float y);
        void reset();
};

class Square : public Shape{
    public:
        Square(unsigned int shader, Pos pos, RGB color, float sideLength);
    private:
        std::vector<float> calculateVertices(Pos pos, float sideLength);
        std::vector<unsigned int> calculateIndices();
};

class Triangle : public Shape{
    public:
        Triangle(unsigned int shader, Pos pos, RGB color, float sideLength);
    private:
        std::vector<float> calculateVertices(Pos pos, float sideLength);
        std::vector<unsigned int> calculateIndices();
};

class Circle : public Shape{
    public:
        Circle(unsigned int shader, Pos pos, RGB color, float radius, unsigned int numElements);
    private:
        std::vector<float> calculateVertices(Pos pos, float radius, unsigned int numElements);
        std::vector<unsigned int> calculateIndices(unsigned int numElements);
};
