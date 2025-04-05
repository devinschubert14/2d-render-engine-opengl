#include "planet.hpp"

Planet::Planet(std::string name, float mass, Vector2D pos, RGB color){
    this->name = name;
    this->mass = mass;
    this->position =  {pos.x, pos.y};
    this->velocity = {0.0f, 0.0f};

    float x = pos.x * app.getScaleFactor();
    float y = pos.y * app.getScaleFactor();

    float radius = std::sqrt(mass) * app.getScaleFactor();
    this->circle = new Circle(app.getShaderProgram(),{x,y}, color, radius, 64);
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
