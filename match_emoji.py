import pandas as pd
import numpy as np
# import PIL
import cv2 as cv
import sys
import re

from glob import glob

import matplotlib.pyplot as plt
# from PIL import Image

fd = 'data/dataset/emoji_' + sys.argv[1] +'.jpg' #add command arg to file path

emoji_files = glob('data/dataset/*.jpg') #get all files in dataset folder
if (fd in emoji_files):
	img = cv.imread(fd, cv.IMREAD_GRAYSCALE) #read image as grayscale
else:
	print(f'file {fd} not found')
	sys.exit()


# create template from the image itself; x,y actually give similar result as max_loc
values = np.argwhere(img==0) #array where there are values to crop template
values = np.fliplr(values) #reverse columns
x, y, w, h = cv.boundingRect(values) #create rectangle around value
template = img[y:y+h, x:x+h] #crop image from boundingrectangle values

match = cv.matchTemplate(img, template, cv.TM_SQDIFF) #find template in image
match = cv.normalize(match, None, alpha=0, beta=255, norm_type=cv.NORM_MINMAX, dtype=cv.CV_8U) #normalize match to display as img
match = cv.resize(match, (img.shape[1], img.shape[0])) #resize match to have the same number of rows as img
min_val, max_val, min_loc, max_loc = cv.minMaxLoc(match) #get location of maximum match
print('Match location :', max_loc, '\nCrop location : ', (x,y))


both = cv.hconcat([img, match]) #concatenate image and match result horizontally
cv.imwrite('comparison.jpg', both)
cv.imshow(fd, both)
cv.waitKey(0)
cv.destroyAllWindows()
# wait_time = 1000
# while cv.getWindowProperty('just_a_window', cv.WND_PROP_VISIBLE) >= 1:
#     keyCode = cv.waitKey(wait_time)
#     if (keyCode & 0xFF) == ord("q"):
#         cv.destroyAllWindows()
#         break
