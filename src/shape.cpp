#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

typedef struct {
    float R;
    float G;
    float B;
}RGB;

class Shape{
    private:
        std::vector<float> vertices;
        std::vector<unsigned int> indices;
        RGB color;
        unsigned int vao, vbo, ebo;
    public:
        Shape(unsigned int shader, std::vector<float> vertices, std::vector<unsigned int> indices, RGB color, int numElements);
        createVAO(unsigned int shader);
        createVBO();
        createEBO();
        render();
};

Shape::Shape(unsigned int shader, std::vector<float> vertices, std::vector<unsigned int> indices, RGB color, int numElements){
    this.vertices = vertices;
    this.createVBO();

    this.indices = indices;
    this.createEBO();

    this.color = color;

    this.num_components = 2;
    this.numElements = numElements;

    this.createVAO(shader);
}

Shape::createVAO(unsigned int shader){
        glGenVertexArrays(1, &this.vao);
        glBindVertexArray(this.vao);
        glVertexAttribPointer(0, VERTEX_COORD_TOTAL , GL_FLOAT, GL_FALSE, VERTEX_COORD_TOTAL * sizeof(float), (void*)0);
        glEnableVertexAttribArray();
        glBindVertexArray(0);
}

Shape::createVBO(){
    glGenBuffers(1, &this.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), this.vertices, GL_STATIC_DRAW);
}
