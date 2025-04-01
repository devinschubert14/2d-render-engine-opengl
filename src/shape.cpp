#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

#define PI 3.14159265358979323846

typedef struct {
    float r;
    float g;
    float b;
}RGB;

RGB hex2rgb(int hexValue)
{
  RGB rgbColor;
  rgbColor.r = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
  rgbColor.g = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
  rgbColor.b = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte

  return rgbColor; 
}

typedef struct {
    float x;
    float y;
}Pos;

class Shape{
    public:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        RGB color;
        unsigned int shader;
        unsigned int vao, vbo, ebo;
        unsigned int numElements;
        unsigned int numComponents;
        Shape(unsigned int shader, std::vector<float> vertices, std::vector<unsigned int> indices, RGB color, int numElements);
        ~Shape();
        void createVAO(unsigned int shader);
        void createVBO();
        void createEBO();
        void render();
};

Shape::Shape(unsigned int shader, std::vector<float> vertices, std::vector<unsigned int> indices, RGB color, int numElements){
    this->shader = shader;
    this->vertices = vertices;
    this->createVBO();

    this->indices = indices;
    this->createEBO();

    this->color = color;

    this->numComponents = 3;
    this->numElements = numElements;

    this->createVAO(shader);
}

Shape::~Shape(){
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Shape::createVAO(unsigned int shader){
        glGenVertexArrays(1, &this->vao);
        glBindVertexArray(this->vao);
        glVertexAttribPointer(0, this->numComponents , GL_FLOAT, GL_FALSE, this->numComponents * sizeof(float), (void*)0);
        //TODO: Find shader attrib for vertex coords and change value, currently variable is hardcoded
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
}

void Shape::createVBO(){
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), this->vertices.data(), GL_STATIC_DRAW);
}

void Shape::createEBO(){
    glGenBuffers(1, &this->ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(unsigned int), this->indices.data(), GL_STATIC_DRAW);
}

void Shape::render(){
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);

    glUseProgram(this->shader);
    int colorLocation = glGetUniformLocation(this->shader, "aColor"); 
    glUniform4f(colorLocation, color.r, color.g, color.b, 1.0f);

    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

class Square : public Shape{
    public:
        Square(unsigned int shader, Pos pos, RGB color, float sideLength) : Shape(shader, calculateVertices(pos,sideLength), calculateIndices(), color, 2) {
        }

        std::vector<float> calculateVertices(Pos pos, float sideLength){
            return {
                    (pos.x + (sideLength/2)), (pos.y + (sideLength/2)), 0.0f, //Top Right
                    (pos.x + (sideLength/2)), (pos.y - (sideLength/2)),  0.0f, //Bottom Right 
                    (pos.x - (sideLength/2)), (pos.y - (sideLength/2)), 0.0f, //Bottom Left
                    (pos.x - (sideLength/2)), (pos.y + (sideLength/2)), 0.0f //Top left
            };
        }

        std::vector<unsigned int> calculateIndices(){
            return {0,1,3,1,2,3};
        }
};

class Triangle : public Shape{
    public:
        Triangle(unsigned int shader, Pos pos, RGB color, float sideLength) : Shape(shader, calculateVertices(pos,sideLength), calculateIndices(), color, 2) {
        }

        std::vector<float> calculateVertices(Pos pos, float sideLength){
            return {
                    (pos.x - (sideLength/2)), (pos.y - (sideLength/2)), 0.0f, //Bottom right
                    (pos.x + (sideLength/2)), (pos.y - (sideLength/2)), 0.0f, //Bottom left
                    (pos.x), (pos.y + (sideLength/2)), 0.0f  //Top 
            };
        }

        std::vector<unsigned int> calculateIndices(){
            return {0,2,1};
        }
};

class Circle : public Shape{
    public:
        Circle(unsigned int shader, Pos pos, RGB color, float radius, unsigned int numElements) : Shape(shader, calculateVertices(pos,radius,  numElements), calculateIndices(numElements), color, numElements) {
        }

        std::vector<float> calculateVertices(Pos pos, float radius, unsigned int numElements){
            std::vector<float> vertices;
            float angleStep = 2 * PI / numElements; 
            vertices.push_back(pos.x);
            vertices.push_back(pos.y);
            vertices.push_back(0.0f);
            for(int i = 0; i < numElements; i++){
                float angle =  i * angleStep;
                vertices.push_back(pos.x + radius * cos(angle));
                vertices.push_back(pos.y + radius * sin(angle));
                vertices.push_back(0.0f);
            }
           return vertices; 
        }

        std::vector<unsigned int> calculateIndices(unsigned int numElements){
            std::vector<unsigned int> indices;
            for(int i = 0; i < numElements; i++){
                indices.push_back(0);
                indices.push_back(i + 1);
                //indices[(i*VERTEX_COORD_TOTAL)+2] = i + 2;
                indices.push_back((i + 1) % numElements + 1);
            }
            return indices;
        }
};


