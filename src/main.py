import cv2 as cv
import numpy as np
from matplotlib import pyplot as plt
 
# Load image and template
img_rgb = cv.imread('../data/dataset/emoji_0.jpg')
assert img_rgb is not None, "file could not be read, check with os.path.exists()"
img_gray = cv.cvtColor(img_rgb, cv.COLOR_BGR2GRAY)
template = cv.imread('emoji_template.jpg', cv.IMREAD_GRAYSCALE)
assert template is not None, "file could not be read, check with os.path.exists()"

# Get template dimensions
w, h = template.shape[::-1]
 
# Returns a map of comparison results
res = cv.matchTemplate(img_gray,template,cv.TM_CCOEFF_NORMED)

# Only consider such matches that are above the threshold
threshold = 0.85

# loc = np.where( res >= threshold )
loc = np.asarray(res >= threshold).nonzero() # preferable to using np.where without x,y according to docs

# Iterate through matches
for pt in zip(*loc[::-1]):
	cv.rectangle(img_rgb, pt, (pt[0] + w, pt[1] + h), (0,255,0), 2)
	print(f'Emoji found at: {pt}')
 