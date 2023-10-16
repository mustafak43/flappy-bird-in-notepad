#pragma once

#include <Windows.h>
#include <vector>
#include <filesystem>
#include <fstream>
#include "object.h"
#include "bird.h"

#define SCREEN_X 150 // screen width
#define SCREEN_Y 150 // screen height

// [ Min inclusive, Max exclusive )
float RandomFloat();
// [ Min inclusive, Max exclusive )
int RandomInt(int a, int b);
// [ Min inclusive, Max exclusive )
float RandomFloat(int a, int b);

// get notepad edit box from notepad handle
HWND GetNotepadEditBox(HWND hParent);

// shortened version of send message
void SetText(HWND hEditBox, LPARAM text);

// returns a vector of objects at the specified path
std::vector<Object*> GetObjectsAt(const std::filesystem::path directory);

void RemoveObjectFrom(Object* obj, std::vector<Object*>* v);
void RemoveObjectFromAndDestroy(Object* obj, std::vector<Object*>* v);

class Screen
{
    wchar_t* body;
    std::vector<Object*>* objects;

public:
    Screen(std::vector<Object*>* _objects);

    void Render(HWND hWndEdit);
};