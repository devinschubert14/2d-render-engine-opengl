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
        return std::sqrt(x * x + y * y);
    }

    Vector2D normalize(){
        float mag = magnitude();
        return {x/mag, y/mag};
    }
};


