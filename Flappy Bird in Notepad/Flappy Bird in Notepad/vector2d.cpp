#include "vector2d.h"

Vector2D Vector2D::operator+(const Vector2D& p)
{
    Vector2D temp(x + p.x, y + p.y);
    return temp;
}
Vector2D Vector2D::operator-(const Vector2D& p)
{
    Vector2D temp(x - p.x, y - p.y);
    return temp;
}
Vector2D& Vector2D::operator=(const Vector2D& p)
{
    x = p.x;
    y = p.y;

    return *this;
}

Vector2D::Vector2D()
{
    x = 0;
    y = 0;
}
Vector2D::Vector2D(float _x, float _y) : x(_x), y(_y) {}

float Vector2D::GetX() const { return x; }
float Vector2D::GetY() const { return y; }

void Vector2D::Set(Vector2D p) { *this = p; }
void Vector2D::Add(Vector2D v) { *this = *this + v; }
void Vector2D::Sub(Vector2D v) { *this = *this - v; }
void Vector2D::Mult(int s)     { *this = Vector2D(x * s, y * s); }

Vector2D Vector2D::Up()    { return Vector2D(0, -1); }
Vector2D Vector2D::Down()  { return Vector2D(0, 1); }
Vector2D Vector2D::Right() { return Vector2D(1, 0); }
Vector2D Vector2D::Left()  { return Vector2D(-1, 0); }
Vector2D Vector2D::Zero()  { return Vector2D(0, 0); }