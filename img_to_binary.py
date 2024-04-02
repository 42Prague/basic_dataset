# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    img_to_binary.py                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pibouill <pibouill@student.42prague.c      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/02 12:32:21 by pibouill          #+#    #+#              #
#    Updated: 2024/04/02 13:30:10 by pibouill         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

import cv2
import sys
import numpy as np

if len(sys.argv) != 2:
    print("Usage: python3 <file>.py <img>.jpg")
    sys.exit(1)

image_path = sys.argv[1]

# Reads the image. 2 flag for greyscale
img = cv2.imread(image_path, 2)

# Any pixel value greater than 127 is set to 0 and any pixel value less than 127 is set to 255
#ret, bw_img = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)

# the last argument is a flag to specify we want the object to appear black on white bg
#bw = cv2.threshold(img, 127, 255, cv2.THRESH_BINARY)

# Display the image
#cv2.imshow("Binary", bw_img)

ret, binary_img = cv2.threshold(img, 127, 1, cv2.THRESH_BINARY_INV)

# Generate a txt file representing the img where white is 0 and black is 1
with open("binary_map.txt", "w") as file:
    for row in binary_img:
        for pixel in row:
            file.write(str(pixel))
        file.write("\n")

# Waits for user to press a key to terminate
cv2.waitKey(0)

cv2.destroyAllWindows()
