# image operations
import imageio.v3 as iio
import numpy as np

EMPTY_PIXEL = [255, 255, 255, 255]
# 70 levels of gray
GSCALE = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~i!lI;:,\"^`. "
# 10 levels of gray
#GSCALE = "@%#*+=-:.     "
GSCALE_LEN = len(GSCALE)

def remap(OldMin, OldMax, NewMin, NewMax, OldValue):
    OldRange = (OldMax - OldMin)
    if OldRange == 0:
        NewValue = NewMin
    else:
        NewRange = (NewMax - NewMin)  
        NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin
    return int(NewValue)

def pixelToAscii(density): # density = integer avg of rgb
    indice = remap(0, 255, GSCALE_LEN, 0, density)
    return GSCALE[(GSCALE_LEN-1 if indice >= GSCALE_LEN else indice)]

class Image:
    def __init__(self, img_path):
        self.image = iio.imread(img_path)
        self.image_size = (-1, -1)
        self.image_len = -1
        self.image_ascii = self.ImageToAscii()
        
    def ImageToAscii(self):
        img_ascii = ""
        for i in range(self.image.shape[0]):
            for j in range(self.image.shape[1]):
                asciiOfPixel = " "
                lum = self.image[i][j][0] * .3 + self.image[i][j][1] * .59 + self.image[i][j][2] * .11
                if not np.array_equal(self.image[i][j], EMPTY_PIXEL):
                    asciiOfPixel = pixelToAscii(lum)
                img_ascii += asciiOfPixel
            img_ascii += "\n"
        img_ascii = self.CropAsciiImage(img_ascii)
        return img_ascii
        
    def CropAsciiImage(self, img_ascii):
        lines = []
        line = ""
        
        for i in range(len(img_ascii)):
            if img_ascii[i] != '\n':
                line += img_ascii[i]
            else:
                line = line.rstrip(" ")
                lines.append(line)
                line = ""
        
        lines_leading_spaces_counts = []
        for i in range(len(lines)-1, -1, -1):
            shouldDelete = True
            for j in range(GSCALE_LEN - 1):
                if GSCALE[j] in lines[i]:
                    lines_leading_spaces_counts.append(len(lines[i]) - len(lines[i].lstrip(' ')))
                    shouldDelete = False
                    break
            if shouldDelete:
                del lines[i]
        
        min_spaces_to_be_removed = min(lines_leading_spaces_counts)
        for i in range(len(lines)):
            lines[i] = lines[i][min_spaces_to_be_removed:]
            
        max_width = 0
        for l in lines:
            if len(l) > max_width:
                max_width = len(l)
        
        self.image_size = (max_width, len(lines))
        
        for i in range(len(lines)):
            if len(lines[i]) < max_width:
                for j in range(max_width - len(lines[i])):
                    lines[i] += " "
        
        
        for l in lines:
            if len(l) < max_width:
                for i in range(max_width - len(l)):
                    l += ' '
        
        for l in lines:
            self.image_len += len(l)
        self.image_len += 1
        
        print("image_len: " + str(self.image_len))
        
        
        return "\n".join(lines)
