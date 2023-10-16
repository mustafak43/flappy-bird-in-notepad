#include "object.h"

Object::Object()
{
    body = L"";

    width = 0;
    height = 0;
    name = "-";
    position = Vector2D::Zero();
    size = 0;

    start_x = 0;
    start_y = 0;
    
    active = false;
}

Object::Object(unsigned int w, unsigned int h, std::string nm, wchar_t* b, Vector2D p, unsigned int s) : body(b)
{
    width = w;
    height = h;
    name = nm;
    position = p;
    size = s;

    start_x = 0;
    start_y = 0;

    active = true;
}

unsigned int Object::GetStartX() const      { return start_x; }
unsigned int Object::GetStartY() const      { return start_y; }
unsigned int Object::GetWidth() const       { return width; }
unsigned int Object::GetHeight() const      { return height; }
unsigned int Object::GetSize() const        { return size; }
std::string Object::GetName() const { return name; }
Vector2D Object::GetPos() const     { return position; }
wchar_t* Object::GetBody() const
{
    wchar_t* cpy;
    cpy = (wchar_t*)malloc(sizeof(wchar_t) * size);
    if (cpy != nullptr) wmemcpy(cpy, body, size);
    else 1; // ERROR
    return cpy;
}

void Object::SetPos(Vector2D p) { position = p; }
void Object::SetActive(bool b)  { active = b; }

bool Object::IsActive() const { return active; }