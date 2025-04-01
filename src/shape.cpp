#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

typedef struct {
    float R;
    float G;
    float B;
}RGB;

typedef struct {
    float x;
    float y;
}Pos;

class Shape{
    public:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        RGB color;
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
    //TODO: Change color
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
