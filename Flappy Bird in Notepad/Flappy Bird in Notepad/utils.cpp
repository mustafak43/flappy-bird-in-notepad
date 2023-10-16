#include "utils.h"

// [ Min, Max )
float RandomFloat()
{
    return (float)(rand()) / (float)(RAND_MAX);
}
int RandomInt(int a, int b)
{
    if (a > b)
        return RandomInt(b, a);
    if (a == b)
        return a;
    return a + (rand() % (b - a));
}
float RandomFloat(int a, int b)
{
    if (a > b)
        return RandomFloat(b, a);
    if (a == b)
        return a;

    return (float)RandomInt(a, b) + RandomFloat();
}

// get notepad edit box from notepad handle
HWND GetNotepadEditBox(HWND hParent)
{
    // if neither one of code snippets below work
    // use spy++ to examine the window structure of notepad
    // then recode here

    // for windows 11
    HWND hwndTextBox, hwndRichEdit;

    while (!(hwndTextBox = FindWindowExA(hParent, NULL, "NotepadTextBox", NULL))) { Sleep(10); }
    while (!(hwndRichEdit = FindWindowExA(hwndTextBox, NULL, "RichEditD2DPT", NULL))) { Sleep(10); }

    return hwndRichEdit;

    // for windows 10
    /*
    HWND hwndEditBox;

    while (!(hwndEditBox = FindWindowExA(hParent, NULL, "Edit", NULL))) { Sleep(10); }

    return hwndEditBox;
    */
}

// shortened version of send message
void SetText(HWND hEditBox, LPARAM text)
{
    SendMessage(hEditBox, WM_SETTEXT, 0, text);
}

// returns a vector of objects at the specified path
std::vector<Object*> GetObjectsAt(const std::filesystem::path directory)
{
    std::vector<Object*> container;
    // Iterate over the contents of the directory
    for (const auto& entry : std::filesystem::directory_iterator(directory))
    {

        std::string file_name = entry.path().filename().string();

        // parse the name of the object for metadata - i.e. name, width, height, size.
        std::vector<std::string> words;
        std::string temp = "";
        for (int i = 0; i < file_name.length(); i++)
        {
            if (file_name[i] != '-')
            {
                temp += file_name[i];
            }
            else
            {
                words.push_back(temp);
                temp = "";
            }
        }
        words.push_back(temp);


        UINT width = (UINT)stoi(words[1]);
        UINT height = (UINT)stoi(words[2]);
        // Get the size of the file in bytes
        UINT size = (UINT)stoi(words[3]) + height;

        // cout << entry.path().string() << endl;
        std::ifstream file(entry.path().string());

        // Allocate memory for the wchar_t array
        wchar_t* file_content = new wchar_t[size];

        char* actual_content = new char[size];

        // Read the contents of the file into the wchar_t array
        file.read(actual_content, size);

        // Close the file
        file.close();

        size_t s;
        mbstowcs_s(&s, file_content, static_cast<size_t>(size) + 1, actual_content, static_cast<size_t>(size) - 1);

        // pass the copy of file_content to the object

        if (words[0] == "myBird")
        {
            Vector2D pos(SCREEN_X / 2 - width / 2, SCREEN_Y / 2 - height / 2);
            Object* obj = new Bird(Vector2D::Up(), width, height, words[0], file_content, pos, size);
            container.push_back(obj);
        }
        else
        {
            Object* obj = new Object(width, height, words[0], file_content, Vector2D(0, 0), size);
            container.push_back(obj);
        }
    }
    return container;
}

void RemoveObjectFrom(Object* obj, std::vector<Object*>* v)
{
    for (int i = 0; i < v->size(); i++)
    {
        if (v->at(i) == obj)
        {
            v->erase(v->begin() + i);
        }
    }
    // v->erase(std::remove(v->begin(), v->end(), obj), v->end());
}
void RemoveObjectFromAndDestroy(Object* obj, std::vector<Object*>* v)
{
    for (int i = 0; i < v->size(); i++)
    {
        if (v->at(i) == obj)
        {
            v->erase(v->begin() + i);
        }
    }
    // v->erase(std::remove(v->begin(), v->end(), obj), v->end());
    delete obj;
}

Screen::Screen(std::vector<Object*>* _objects)
{
    objects = _objects;
    body = new wchar_t[objects->at(0)->GetSize()];
}

void Screen::Render(HWND hWndEdit) // hWndEdit = edit control to be editted
{
    body = const_cast<wchar_t*>(objects->at(0)->GetBody());

    // for all objects except for background
    // render them on top of background
    for (int i = 1; i < objects->size(); i++)
    {
        bool obj_active = objects->at(i)->IsActive();
        if (!obj_active) continue;

        const wchar_t* obj_body = objects->at(i)->GetBody();
        int obj_width = objects->at(i)->GetWidth();
        int obj_height = objects->at(i)->GetHeight();
        int obj_x = objects->at(i)->GetPos().GetX();
        int obj_y = objects->at(i)->GetPos().GetY();
        int start_x = objects->at(i)->GetStartX();
        int start_y = objects->at(i)->GetStartY();

        // change the variable names later on
        int arbitraryValue1 = 0; // OFFSET Y
        int arbitraryValue2 = 0; // OFFSET X

        // edge cases
        // there's probably something wrong with vertical edge cases
        // but since the bird is killed when reaching the ceiling or the floor,
        // these might not need fixing
        if (obj_y < 0)
        {
            start_y = abs(obj_y);
            obj_height -= abs(obj_y);
            obj_y = 0;
        }
        else if (obj_y + obj_height > SCREEN_Y)
        {
            obj_height = SCREEN_Y - obj_y;
        }
        // two of em up there

        if (obj_x < 0)
        {
            start_x = abs(obj_x);
            arbitraryValue2 += obj_x;
            obj_x = 0;
        }
        else if (obj_x + obj_width > SCREEN_X)
        {
            arbitraryValue2 += (obj_x + obj_width - SCREEN_X) * -1;
        }

        // render on top of body
        for (int j = obj_y, y = start_y; j < obj_y + obj_height + arbitraryValue1; j++, y++)
        {
            for (int k = obj_x, x = start_x; k < obj_x + obj_width + arbitraryValue2; k++, x++)
            {
                if (obj_body[y * (obj_width + 1) + x] != ' ')
                {
                    body[j * (SCREEN_X + 1) + k] = obj_body[y * (obj_width + 1) + x];
                }
            }
        }

        // delete body because it is a copy
        delete obj_body;
    }

    SetText(hWndEdit, (LPARAM)body);

    // delete body because it is a copy
    delete body;
}

