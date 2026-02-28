import numpy as np
from PIL import Image

IMG_NAME = "colors.ppm"

img = np.array(Image.open(IMG_NAME))
print(img[0][::45])
