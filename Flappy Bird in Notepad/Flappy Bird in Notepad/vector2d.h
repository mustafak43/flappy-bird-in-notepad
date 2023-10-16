#pragma once

class Vector2D
{
private:
    float x;
    float y;

    Vector2D operator+(const Vector2D& p);
    Vector2D operator-(const Vector2D& p);

public:
    Vector2D();
    Vector2D(float _x, float _y);

    void Set(Vector2D p);
    float GetX() const;
    float GetY() const;

    void Add(Vector2D v);
    void Sub(Vector2D v);
    void Mult(int s);

    Vector2D& operator=(const Vector2D& p);

    static Vector2D Up();
    static Vector2D Down();
    static Vector2D Right();
    static Vector2D Left();
    static Vector2D Zero();
};