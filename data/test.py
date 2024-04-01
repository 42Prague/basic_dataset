import matplotlib as plt
import cv2 as cv
import numpy as np
import time

def checkForEmoji(img, emoji, X, Y):
    roi = img[Y:Y+50, X:X+50]
    diff = cv.subtract(roi, emoji)
    # diff = diff.astype(np.uint8)
    # print(np.count_nonzero(diff))
    if np.count_nonzero(diff) < 3100:
        # print(diff)
        # cv.imshow('a', roi)
        # cv.waitKey(0)
        return True
    return False

img = cv.imread('./dataset/emoji_10.jpg')
if img is None:
    print('Is broken')

emoji = cv.imread('emoji.jpg')
if emoji is None:
    print('Is broken')

clone = img.copy()

height, width, depth = img.shape

for k in range(0, 100):
    path = "./dataset/emoji_" + str(k) + ".jpg"
    img = cv.imread(path)
    for i in range (205, height - 50):
        for j in range (0, width - 50):
            check = checkForEmoji(img, emoji, j, i)
            if check == True:
                print("Emoji ", end="")
                print(k, end=": ")
                print(j, end=" ")
                print(i)
                break
        if check == True:
            check = False
            break
                # cv.rectangle(img, (j, i), (j + 50, i + 50), color=(0,255,0))
                # cv.imshow('test', img)
                # cv.waitKey(0)
                # exit()
            # cv.rectangle(img, (j, i), (j + 50, i + 50), color=(0,0,255))
            # cv.imshow('test', img)
            # key = cv.waitKey(1)
            # if key == 27:#if ESC is pressed, exit loop
            #     cv.destroyAllWindows()
            #     exit()
            # img = clone.copy()
            # print(img[i][j])

# roi = img[284:334,437:487]
# cv.imshow('test', roi)

# cv.waitKey(0)