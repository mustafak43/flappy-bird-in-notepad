/*
* requires c++17 or above
* works without any code modification in windows 11
* if you use window 10 or below you may need to modify the code responsible for getting handle for notepad and its edit box
* set the font family to MS Gothic and font size to 3 &
* resize the window to fit 150x150 characters in notepad before running the code
* 
* it might be useful to read comments at least in main.cpp
*/

#include <iostream>
#include <chrono>
#include "object.h"
#include "bird.h"
#include "utils.h"
#include "pipe.h"

#define BACKGROUND_INDEX 0
#define BIRD_INDEX 1
#define SEC_IN_MS 1000

const int FPS = 60;
int delay = SEC_IN_MS / FPS;

// variable for pipe generation
std::chrono::steady_clock::time_point nextInvocation;

class GeneticAlgorithm
{
    std::vector<Object*>* agents;
    int popSize;

    int generation;
public:
    GeneticAlgorithm(int _popSize, std::vector<Object*> objts, std::vector<Object*>* h)
    {
        agents = new std::vector<Object*>();
        popSize = _popSize;

        generation = 1;

        for (int i = 0; i < popSize; i++)
        {
            Object* bird = new Bird(*(objts[BIRD_INDEX]), Vector2D(0, -3));
            agents->push_back(bird);

            AddToHierarchy(h, bird);
        }
    }

    Object* GetABird()
    {
        if (agents->size() > 0)
            return agents->at(0);
        return NULL;
    }

    // returns the generation count
    int GetGeneration() const { return generation; }

    // returns the score of the best scorer
    int GetBestScore()
    {
        int bestScore = 0;
        for (int i = 0; i < agents->size(); i++)
        {
            if (static_cast<Bird*>(agents->at(i))->GetScore() > bestScore)
                bestScore = static_cast<Bird*>(agents->at(i))->GetScore();
        }
        return bestScore;
    }

    // h = hierarchy, p = pipes ,objts = OBJECTS
    // next population is actually created randomly
    // so its just a matter of luck to get an agent that knows to play
    // fix this function
    void NextPopulation(std::vector<Object*>* h, std::vector<Object*>* p, const std::vector<Object*>& objts)
    {
        // if all the birds are dead
        if (Bird::GetTotalDeadBirds() == popSize)
        {
            generation++;

            // remove every element in the hierarchy except background
            h->erase(h->begin() + 1, h->end());

            // clear the agents
            for (int i = 0; i < agents->size(); i++)
            {
                delete agents->at(i);
            }
            agents->clear();

            // add new agents
            for (int i = 0; i < popSize; i++)
            {
                Object* bird = new Bird(*(objts[BIRD_INDEX]), Vector2D(0, -3));

                agents->push_back(bird);
                AddToHierarchy(h, bird);

                static_cast<Bird*>(bird)->MutateBird();
            }

            // destroy pipes
            for (int i = 0; i < p->size(); i++)
            {
                delete p->at(i);
            }
            p->clear();

            // reset pipe generator timer
            nextInvocation = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(3250); // 3250 = 3.25f delay time

            Bird::totalDeadBirds = 0;
        }
    }

    void AddToHierarchy(std::vector<Object*>* h, Object* o)
    {
        h->push_back(o);
    }
};

class GameController
{
    std::vector<Object*>* pipes;
    int gap = 70;
    float pipeDelayTime = 3.25f; // in seconds

public:
    GameController(const std::vector<Object*>& objts, std::vector<Object*>* h)
    {
        pipes = new std::vector<Object*>();

        PipeTB* pipe = new PipeTB(gap, objts);
        pipes->push_back(pipe->top);
        pipes->push_back(pipe->bottom);

        AddToHierarchy(h, pipe->top, pipe->bottom);

        nextInvocation = std::chrono::high_resolution_clock::now();
    }

    // return closest pipe's index to the bird
    int GetClosestPipe(Object* bird)
    {
        if (pipes->size() % 2 == 0 && bird != NULL)
        {
            // increasing by 2, because pipes are stored as pairs (top & bottom)
            for (int i = 0; i < pipes->size(); i += 2)
            {
                // if pipes rightmost x is greater than birds x
                if (pipes->at(i)->GetPos().GetX() + PIPE_WIDTH > bird->GetPos().GetX())
                {
                    return i;
                }
            }

            return -1;
        }
        return -1;
    }

    void CreatePipe(const std::vector<Object*>& objts, std::vector<Object*>* h)
    {
        // if x seconds has passed create another pipe
        auto now = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(now - nextInvocation).count();

        if (elapsed >= pipeDelayTime)
        {
            PipeTB* pipe = new PipeTB(gap, objts);
            pipes->push_back(pipe->top);
            pipes->push_back(pipe->bottom);

            AddToHierarchy(h, pipe->top, pipe->bottom);

            nextInvocation = now;
        }

        // std::cout << "gc.pipes->size(): " << pipes->size() << std::endl;
    }

    // return pipes collection
    std::vector<Object*>* GetPipes() const { return pipes; }

    void AddToHierarchy(std::vector<Object*>* h, Object* pT, Object* pB)
    {
        h->push_back(pT);
        h->push_back(pB);
    }
};

int main()
{
	srand(time(0));

	// std::cout << "everything works just fine " << RandomInt(1, 11) << "\n";

    // DATA DECLARATIONS &
    // ENVIRONMENT SETUP
    bool crashed = false;

    LPCWSTR appName = L"C:\\Windows\\system32\\notepad.exe";
    // check the name of the empty notepad instance in your language from task manager...
    // ...and change the 'nameOfInstance' variable accordingly...
    // ...if it doesn't work try using the name of the empty notepad title
    // for windows 10 = "Adsýz - Not Defteri"
    // for windows 11 = "Adsýz"
    LPCWSTR nameOfInstance = L"Not Defteri"; // title of the empty notepad
     
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    HWND hWndNotepad = NULL, hWndNotepadEdit = NULL;

    // OBJECTS TO CHOOSE FROM A.K.A OBJECTS POOL (or just prefabs)
    std::vector<Object*> OBJECTS;
    const std::filesystem::path objects_path = "../../.prefabs/";
    OBJECTS = GetObjectsAt(objects_path);
    std::cout << "OBJECTS.size(): " << OBJECTS.size() << std::endl;

    // objects that will be present in the scene
    std::vector<Object*>* hierarchy = new std::vector<Object*>();

    // add background - always at index 0 -
    hierarchy->push_back(OBJECTS[BACKGROUND_INDEX]);

    int population_size = 100;
    GeneticAlgorithm ga(population_size, OBJECTS, hierarchy);

    GameController gc(OBJECTS, hierarchy);

    Screen SCREEN(hierarchy);

    // if open notepad doesn't work terminate the app
    if (!CreateProcess(appName, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        std::cout << "Error running the notepad !\n";
        return 1;
    }
    else
    {
        // find notepad
        std::cout << "trying to find the instance\n";
        while (!(hWndNotepad = FindWindow(NULL, nameOfInstance))) { Sleep(10); }
        std::cout << "instance found\n";

        // if the code gets stuck here its because it will keep trying to get a handle for notepad edit box
        std::cout << "trying to find rich edit control of notepad window\n";
        // find its edit box
        hWndNotepadEdit = GetNotepadEditBox(hWndNotepad);
        std::cout << "rich edit control found\n";

        // ------------------------------

        // GAME LOOP WHERE THE MAGIC HAPPENS
        bool q_key_down = false;
        bool space_key_down = false;
        while (!crashed)
        {
            // GAME LOGIC

            // if q is pressed terminate the game
            if (GetAsyncKeyState('Q') & 0x8000)
            {
                if (!q_key_down)
                {
                    // break the loop
                    crashed = true;

                    q_key_down = true;
                }
            }
            else
            {
                q_key_down = false;
            }

            // below code should only be un-commented when one decides to play him/herself
            // note that you should also comment out some other parts of the code so that neuroevolution is deactivated
            //// if spacebar is pressed jump
            //if (GetAsyncKeyState(VK_SPACE) & 0x8000)
            //{
            //    if (!space_key_down)
            //    {
            //        // jump
            //        static_cast<Bird*>(hierarchy->at(1))->setVelocity(Vector2D(0, -3));

            //        space_key_down = true;
            //    }
            //}
            //else
            //{
            //    space_key_down = false;
            //}

            gc.CreatePipe(OBJECTS, hierarchy);
            ga.NextPopulation(hierarchy, gc.GetPipes(), OBJECTS);

            // std::cout << "dead bird counter: " << Bird::totalDeadBirds << std::endl;

            clock_t start = clock();

            // render the screen
            SCREEN.Render(hWndNotepadEdit);
            // to prevent the keyboard hits from registering to the notepad edit control
            SetForegroundWindow(GetDesktopWindow());
            clock_t end = clock();

            //// below was first used to check if it was even possible to render such large amounts of characters 
            //// more than 30 times in notepad
            //double elapsed_time = (end - start) / (double)CLOCKS_PER_SEC;
            //std::cout << "the algorithm took " << elapsed_time << " seconds to complete." << std::endl;

            // iterate over the objects in hierarchy, aka scene.
            for (const auto& obj : (*hierarchy))
            {
                if (obj != nullptr)
                {
                    // if the object in hierarchy is myBird
                    if (obj->GetName() == "myBird")
                    {
                        // if not dead
                        if (!static_cast<Bird*>(obj)->IsDead())
                        {
                            static_cast<Bird*>(obj)->Update(gc.GetPipes(), gc.GetClosestPipe(ga.GetABird()));
                        }
                    }
                    // if the object in hierarchy is pipe
                    else if (obj->GetName() == "pipe_top" || obj->GetName() == "pipe_bottom")
                    {
                        if (!static_cast<Pipe*>(obj)->IsDead())
                        {
                            static_cast<Pipe*>(obj)->Update();
                        }
                        else
                        {
                            // remove the object from the vector
                            RemoveObjectFrom(obj, gc.GetPipes());
                            RemoveObjectFromAndDestroy(obj, hierarchy);
                            // hierarchy->erase(std::remove(hierarchy->begin(), hierarchy->end(), obj), hierarchy->end());
                        }
                    }
                }
            }

            // used to debug
            // crashed = true;

            //// in windows 10, I could read the values in command prompt
            //// but in windows 11 I can't really read any of the values, they're rendered so fast
            //std::cout << "hierarchy->size(): " << hierarchy->size() << std::endl;
            //std::cout << "FPS: " << SEC_IN_MS / delay << std::endl;

            //std::cout << "Generation: " << ga.GetGeneration() << std::endl;
            //// score does not work as intended
            //std::cout << "Score: " << ga.GetBestScore() << std::endl;

            // when the algorithm took more than I anticipated
            // I tried subtracting the amount of time it takes to render 1 frame
            // so that it would compensate by starting to render the next frame earlier
            delay = (SEC_IN_MS / FPS);// -(elapsed_time * SEC_IN_MS);
            Sleep(delay);
        }
        // ------------------------------
    }


    int n;
    std::cin >> n;

    // TERMINATION
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

	return 0;
}