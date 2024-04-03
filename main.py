# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    main.py                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pibouill <pibouill@student.42prague.c      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/02 12:32:21 by pibouill          #+#    #+#              #
#    Updated: 2024/04/03 17:05:16 by pibouill         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import cv2 as cv
import sys
import numpy as np
from matplotlib import pyplot as plt

if len(sys.argv) != 2:
    print("Usage: python3 <file>.py <img>.jpg")
    sys.exit(1)

# Gets the jpg from cli argument
image_path = sys.argv[1]

# Reads the image. 2 flag for greyscale
img = cv.imread(image_path, 2)
# Reads the emoji template
emoji_template = cv.imread("template_emoji.jpg", 2)

method = cv.TM_CCOEFF_NORMED
# Apply the match finding method to the img
result = cv.matchTemplate(emoji_template, img, method)

# Slice the emoji template to get the width and height of the square
w, h = emoji_template.shape[:2]

min_val, max_val, min_loc, max_loc = cv.minMaxLoc(result)



top_left = max_loc
bottom_right = (top_left[0] + w, top_left[1] + h) 


rectangle_color = (0, 0, 255)
rectangle_thickness = 1

img = cv.cvtColor(img, cv.COLOR_GRAY2BGR)
cv.rectangle(img, top_left, bottom_right, rectangle_color, rectangle_thickness)


cv.imshow('output', img)
cv.waitKey(0)

print("Found emoji at :",max_loc)
