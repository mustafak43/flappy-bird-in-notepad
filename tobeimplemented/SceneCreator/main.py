# scene creator
from os import system, listdir # for system("cls") and listdir
from json import dumps # dict to json

def setupScreen(screen, screen_res):
    for i in range(screen_res[1]):
        screen_row = []
        for j in range(screen_res[0]):
            screen_row.append(" ")
        screen.append(screen_row)

def clearScreen(screen, screen_res):
    for i in range(screen_res[1]):
        for j in range(screen_res[0]):
            screen[i][j] = " "

def addObjectToScreen(screen, obj_nm, obj_pos):
    approximate_pos = obj_pos.split(",")
    x, y = int(int(approximate_pos[1]) / 10), int(int(approximate_pos[0]) / 10)
    screen[x][y] = obj_nm

def removeObjectFromScreen(screen, obj_pos):
    approximate_pos = obj_pos.split(",")
    x, y = int(int(approximate_pos[1]) / 10), int(int(approximate_pos[0]) / 10)
    screen[x][y] = " "

def displayScreen(screen, screen_res):
    for i in range(screen_res[1]):
        row = ""
        for j in range(screen_res[0]):
            row += screen[i][j]
        print(row)

# width, heigth
SCREEN_RES = (80, 40)
SCREEN_PREVIEW = []

sceneDir = "../.scenes/"
prefabsDir = "../.sprites/"
sceneExtension = ".scn"

quit = False

object_prefabs = listdir(prefabsDir)

objects_to_save = {}

setupScreen(SCREEN_PREVIEW, SCREEN_RES)

while not quit:
    system("cls")
    
    print("Scene Creator 9000")
    print("------------------")
    print("Said Scene Preview:")
    print("--------------------------------------------------------------------------------")
    displayScreen(SCREEN_PREVIEW, SCREEN_RES)
    print("--------------------------------------------------------------------------------")
    
    obj_data = {}
    
    # list all the possible prefabs to be chosen
    for i in range(len(object_prefabs)):
        print(f"[{i}] {object_prefabs[i]}")
    
    # make the user choose one
    obj_name = object_prefabs[int(input("Object index          : "))]
    obj_pos = input("Object position as x,y: ")
    obj_data["object_pos"] = obj_pos
    
    op = input(f"\nObject {obj_name} at {obj_pos}\nAdd (a) or Discard (d) the object? ")
    
    # add it to the saved list if user wants to
    if op == 'a':
        objects_to_save[obj_name] = obj_data
        addObjectToScreen(SCREEN_PREVIEW, obj_name[0], obj_pos)
    
    print("\nObjects in the scene:")
    for i in range(len(objects_to_save)):
        print(list(objects_to_save.keys())[i])
    print("---------------------")
    
    op = input("""
Continue editing    (c)
Pop the last object (p)
Save scene          (s)
Exit edit mode      (e)? """)
    
    # execute operations accordingly
    if op == "p":
        last_object = list(objects_to_save.keys())[-1]
        removeObjectFromScreen(SCREEN_PREVIEW, objects_to_save.pop(last_object)["object_pos"])
        print(f"Object {last_object} has been removed from the scene!")
    
    elif op == 's':
        scene_name = input("Scene name: ")
        f = open(sceneDir + scene_name + sceneExtension, "w")
        f.write(dumps(objects_to_save))
        f.close()
        clearScreen(SCREEN_PREVIEW, SCREEN_RES)
    elif op == 'e':
        print("Quitting...")
        quit = True
    