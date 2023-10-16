from imgop import Image # img operations
from os import listdir # get files in dir
#from pywinauto import application

#app = application.Application()
#app.start("notepad")

pathToSprites = "../.sprites/"
prefabsDir = "../.prefabs/"
prefabExtension = ".objt"
spritesToConvert = listdir(pathToSprites)

# create prefabs from images (sprites)
prefabs = []
for sprite in spritesToConvert:
    obj = Image(pathToSprites + sprite)
    # prefab = (object as string, object name, object size as tuple, object len as int)
    prefab = (obj.image_ascii, sprite, (obj.image_size[0], obj.image_size[1]), obj.image_len)
    prefabs.append(prefab)

# save prefabs to file
for prefab in prefabs:
    # fileName = fileName-sizeX-sizeY.png
    fileName = prefab[1].split(".")[0] + f"-{prefab[2][0]}-{prefab[2][1]}-{prefab[3]}" + prefabExtension
    f = open(prefabsDir + fileName, "w")
    f.write(prefab[0])
    f.close()

print("executed succesfully")
# app.notepad.edit.set_edit_text(prefabs[0][0])
