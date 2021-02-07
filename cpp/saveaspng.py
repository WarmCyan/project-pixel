import sys
import json
import numpy as np
#import scipy.misc
import imageio

prefix = ""

rid = -1
print(sys.argv)
if len(sys.argv) > 1:
    rid = sys.argv[1]
    prefix = str(rid) + "_"


print("Loading image data...")

imgdatafile = open(f"render/cache/{prefix}imgdata.json")
imgdata = json.load(imgdatafile)
imgdatafile.close();

print("Image data loaded!")

imgarray = np.zeros((imgdata["height"], imgdata["width"], 4), dtype=np.uint8)

print("Copying into image structure...")
for y in range(0, imgdata["height"]):
    for x in range(0, imgdata["width"]):
        imgarray[y][x][0] = imgdata["pixels"][y][x][0]
        imgarray[y][x][1] = imgdata["pixels"][y][x][1]
        imgarray[y][x][2] = imgdata["pixels"][y][x][2]
        imgarray[y][x][3] = imgdata["pixels"][y][x][3]

print("Saving...")
savestring = f"./render/cache/{prefix}render.png"
#scipy.misc.imsave("render.png", imgarray)
imageio.imwrite(savestring, imgarray)
print("Done!")
