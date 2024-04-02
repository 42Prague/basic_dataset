# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    main.py                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pibouill <pibouill@student.42prague.c      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/02 12:32:21 by pibouill          #+#    #+#              #
#    Updated: 2024/04/02 18:19:35 by pibouill         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import cv2 as cv
import sys
import numpy as np
from matplotlib import pyplot as plt

if len(sys.argv) != 2:
    print("Usage: python3 <file>.py <img>.jpg")
    sys.exit(1)

image_path = sys.argv[1]

# Reads the image. 2 flag for greyscale
#img = cv.imread(image_path, 2)
#emoji_template = cv.imread("template_emoji.jpg", 2)
#w, h = emoji_template.shape[::-1]
#
#print(w , h)
#
#res = cv.matchTemplate(img, emoji_template, 5)
#max_loc = cv.minMaxLoc(res)
#
#top_left = max_loc
#bottom_right = (top_left[0] + w, top_left[1] + h)
#
#print(top_left)
#print(bottom_right)
#
#cv.rectangle(img,top_left, bottom_right, 255, 2)
#
#print(top_left)
#print(bottom_right)

#plt.subplot(121),plt.imshow(res,cmap = 'gray')
#plt.title('Matching Result'), plt.xticks([]), plt.yticks([])
#plt.subplot(122),plt.imshow(img,cmap = 'gray')
#plt.title('Detected Point'), plt.xticks([]), plt.yticks([])

#plt.show()

# Display the image
#cv2.imshow("Binary", bw_img)

