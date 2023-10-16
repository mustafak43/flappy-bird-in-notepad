#pragma once

#include <string>
#include "vector2d.h"


class Object
{
private:
    // start x and y defines from where to begin drawing
    // in other words, top left corner coordinates
    unsigned int start_x;
    unsigned int start_y;
    // toggle to render the object or not
    bool active;

protected:
    const wchar_t* body;
    unsigned int width, height;
    unsigned int size;
    std::string name;
    Vector2D position;

public:
    Object();
    Object(unsigned int w, unsigned int h, std::string nm, wchar_t* b, Vector2D p, unsigned int s);

    unsigned int GetStartX() const;
    unsigned int GetStartY() const;

    unsigned int GetWidth() const;
    unsigned int GetHeight() const;

    unsigned int GetSize() const;

    std::string GetName() const;

    Vector2D GetPos() const;

    void SetPos(Vector2D p);

    bool IsActive() const;
    void SetActive(bool b);

    // returns the copy of the body
    // is used when drawing onto the screen (aka notepad)
    wchar_t* GetBody() const;
};