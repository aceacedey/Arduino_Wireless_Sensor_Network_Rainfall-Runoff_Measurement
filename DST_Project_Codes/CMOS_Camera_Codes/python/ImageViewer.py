import os
import io
from PIL import Image
from array import array

def readimage(path):
    count = os.stat(path).st_size / 2
    imsize = 320*240
    with open(path, "rb") as f:
        bytes = bytearray(f.read(imsize))
    bytes = readimage('data.bin')
    imsize = 320*240
    print(len(bytes))
    img = Image.new('RGB', (320, 240), "black")
    pixels = img.load()
    print(img.size[0])
    
    for i in range(img.size[0]):
        offset = i*240        
        for j in range(img.size[1]):
            k = bytes[offset + j]
            pixels[i,j] = (k, k, k)

    img.show()
