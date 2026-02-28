from io import BytesIO
import requests
from PIL import Image
import numpy as np
import sys

IMG_URL = "https://t4.ftcdn.net/jpg/01/14/49/51/360_F_114495172_sTd0Eu8Tv1BP3LBYzgsd5YlADuF31Scj.jpg"
IMG_NAME = "colors.ppm"
ROWS = 80
COLS = 25

len(sys.argv) > 1 and (IMG_URL := sys.argv[1],)
colors = np.array(Image.open(IMG_NAME))[0][::45].astype(np.int32)
arr = np.array(Image.open(BytesIO(requests.get(IMG_URL).content)))
arr = arr[::len(arr) // COLS, ::len(arr[0]) // ROWS][:COLS, :ROWS].reshape((ROWS * COLS, 3))
nearest = lambda color: int(np.argmin(np.sum((color - colors) ** 2, 1)))
open("src/image.rs", "w").write(f"pub const IMAGE: [u16; {ROWS * COLS}] = " + str([nearest(c) for c in arr]) + ";")
