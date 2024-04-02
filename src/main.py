import cv2 as cv
import numpy as np
# from matplotlib import pyplot as plt

# Source images
source_img = 'emoji_test_many_copies.jpg'
emoji = 'emoji_template.jpg'

# Threshold of matching 
threshold = 0.9

img_gray = cv.imread(source_img, cv.IMREAD_GRAYSCALE)
assert img_gray is not None, "file could not be read, check with os.path.exists()"
template = cv.imread(emoji, cv.IMREAD_GRAYSCALE)
assert template is not None, "file could not be read, check with os.path.exists()"

# Comparing image with emoji
res = cv.matchTemplate(img_gray,template, cv.TM_CCOEFF_NORMED)
# Finding the possitions of all emojis and save it to loc list
loc = np.where( res >= threshold)
# Printing positions of all emojis
for pt in zip(*loc[::-1]):
 print("Coordinates of emoji: X:", (pt[0]), ", Y:", (pt[1]))

# Drawing rectangulars to new "res" image
img_rgb = cv.imread(source_img)
assert img_rgb is not None, "file could not be read, check with os.path.exists()"
w, h = template.shape[::-1]
for pt in zip(*loc[::-1]):
 cv.rectangle(img_rgb, pt, (pt[0] + w, pt[1] + h), (0, 128, 0), 2)
# Save to new res.png
cv.imwrite('output_found_emojis.png',img_rgb)
