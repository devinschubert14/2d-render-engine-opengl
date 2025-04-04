#include <string>
#include <vector>
#include <cmath>

// #define G_CONST 6.67430e-11
#define G_CONST 1e-2
#define SIM_SIZE 1000.0f
#define MIN_DISTANCE_THRESHOLD 20.0f
struct Vector2D{
    float x;
    float y;

    Vector2D operator+(const Vector2D& other) const{
        return {x + other.x, y + other.y};
    }

    Vector2D operator-(const Vector2D& other) const{
        return {x - other.x, y - other.y};
    }

    Vector2D operator*(float scalar){
        return {x * scalar, y * scalar};
    }

    float dot(const Vector2D& other){
        return x* other.x + y * other.y;
    }

    float magnitude(){
        return sqrt(x * x + y * y);
    }

    Vector2D normalize(){
        float mag = magnitude();
        return {x/mag, y/mag};
    }
};

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
        Planet(std::string name, float mass, Vector2D pos);
        Vector2D calculateGravityForce(const Planet& other);
    };

std::vector<Planet> planets;

Planet::Planet(std::string name, float mass, Vector2D pos){
    this->name = name;
    this->mass = mass;
    this->position =  {pos.x, pos.y};
    this->velocity = {0.0f, 0.0f};
    planets.push_back(*this);
}

Vector2D Planet::calculateGravityForce(const Planet& other){
    float distance, forceMagnitude;

    //Distance
    Vector2D r = other.position - this->position; 
    distance = r.magnitude();
    
    //Force Magnitude
    if(distance < MIN_DISTANCE_THRESHOLD){
        return {0,0};
    }
    forceMagnitude = (G_CONST * (this->mass * other.mass))/(distance*distance);

    //Force vector
    Vector2D forceVector = r.normalize();
    return forceVector * forceMagnitude;
}
