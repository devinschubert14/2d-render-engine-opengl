#include <string>
#include <vector>
#include <cmath>
#include "shape.hpp"
#include "vector2d.hpp"

#define G_CONST 1e-2
#define SIM_SIZE 1000.0f
#define MIN_DISTANCE_THRESHOLD 20.0f

extern unsigned int shaderProgram;
extern float scaleFactor;

typedef struct {
    float x;
    float y;
}PlanetPos;

class Planet {
    public:
        std::string name;
        float mass;
        Vector2D position;
        Vector2D velocity;
        Planet(std::string name, float mass, Vector2D pos, RGB color);
        ~Planet();
        Vector2D calculateGravityForce(const Planet& other);
        Circle* circle;
};
