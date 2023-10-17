# flappy-bird-in-notepad
## System Requirements

- **Compiler**: Requires C++17 or above.
- **Operating System**: Works without any code modification on Windows 11.

**Note for Windows 10 and Below:**
If you are using Windows 10 or an earlier version, you may need to modify the code responsible for obtaining the handle for Notepad and its edit box.

## Demo

https://youtu.be/DCfDiIQDK9o

## Preparation

Before running the code, make sure to:

1. **Set the Font Family**: Set the font family to "MS Gothic."
2. **Set the Font Size**: Set the font size to "3."
3. **Resize the Window**: Resize the window to fit 150x150 characters in Notepad.

### Visual Enhancements (To Be Implemented)
The "tobeimplemented" folder contains files and resources that can be implemented to enhance the visual appeal of the project. You can explore and integrate these resources into the codebase as desired.

### Graphics and Images
The ".sprites" folder contains raw image files in the PNG format. The code located in the "imagetoprefab" folder, written in Python, can be used to convert these images into ".objt" format. The resulting ".objt" files are then located in the ".prefabs" folder. ".objt" files are just text files containing the ASCII representation of images which are (depending on what it is) less than 150x150 (as this is our canvas size).

Feel free to leverage these assets to improve the visual elements of your project.

## Additional Information

For a better understanding of the code and its functionality, we recommend reading the comments, especially in the `main.cpp` file.

Enjoy using the project! If you have any questions or encounter issues, feel free to reach out.
