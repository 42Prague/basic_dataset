import matplotlib as plt
import cv2 as cv
import numpy as np
import csv
import re

def main():
    files = []

    # Read from the csv file to create a list of files, 
    # since not all numbers are there and the indexes don't correspond to the file names
    with open('labels.csv', newline='') as csvfile:
        csv_reader = csv.reader(csvfile, delimiter=" ")
        for row in csv_reader:
            match = re.search(';.+jpg', row[0])
            if match != None:
                files.append(match.group(0)[1:])

    # Load the emoji file that I'm finding match to
    emoji = cv.imread('emoji.png')
    if emoji is None:
        print('Is broken')
    height = 600
    width = 800

    # Checking for the type of input
    choice = input("Choose index of input image or 'all' for the whole dataset: ")
    if choice == 'all':
        findAll(files, height, width, emoji)
    elif not choice.isdigit():
        print("Incorrect input.")
    elif int(choice) >= 0 and int(choice) < 100:
        findOne(files[int(choice)], height, width, emoji, int(choice))
    else:
        print("Incorrect input")

#Function to go through the whole dataset and show all results
def findAll(files, height, width, emoji):
    k = 0
    check = -1, -1
    for file in files: # Loop through all images
        img = cv.imread('./dataset/' + file)
        clone = img.copy() # Create deep copy for purposes of displaying rectangle
        for i in range (0, height // 2):
            i2 = i + height // 2 - 50   # Sets the second square axis
            if i2 > 550:                # And checks so it doesn't run out of bounds
                i2 = 550
            for j in range (0, width // 2):
                j2 = j + width // 2 - 50 # Again setting the second square axis
                if j2 > 750:
                    j2 = 750
                check = checkSquares(img, emoji, i, j, i2, j2) # Checks if any of the squares contain emoji, if yes, return coordinates
                if check[0] != -1 and check[1] != -1:
                    print("%d,%s,['happy'],[%d],[%d]" % (k, file, check[0], check[1]))
                    k += 1
                    break
            if check[0] != -1 and check[1] != -1:
                check = -1, -1
                break

def findOne(file, height, width, emoji, k):
        img = cv.imread('./dataset/' + file)
        clone = img.copy() # Create deep copy for purposes of displaying rectangle
        for i in range (0, height // 2):
            i2 = i + height // 2 - 50   # Sets the second square axis
            if i2 > 550:                # And checks so it doesn't run out of bounds
                i2 = 550
            for j in range (0, width // 2):
                j2 = j + width // 2 - 50 # Again setting the second square axis
                if j2 > 750:
                    j2 = 750
                check = checkSquares(img, emoji, i, j, i2, j2) # Checks if any of the squares contain emoji, if yes, return coordinates
                if check[0] != -1 and check[1] != -1:
                    print("%d,%s,['happy'],[%d],[%d]" % (k, file, check[0], check[1]))
                    cv.rectangle(img, (check[0], check[1]), (check[0] + 50, check[1] + 50), color=(0,255,0))  # Uncomment this to show found image
                    cv.imshow('Found', img)
                    cv.waitKey(0)
                    exit()
                if i % 4 == 0 and j % 4 == 0: # show the moving squares, except it's a bit slow, so it doesn't show on every pixel refresh
                    cv.rectangle(img, (j, i), (j + 50, i + 50), color=(0,0,255))
                    cv.rectangle(img, (j2, i), (j2 + 50, i + 50), color=(0,0,255))
                    cv.rectangle(img, (j, i2), (j + 50, i2 + 50), color=(0,0,255))
                    cv.rectangle(img, (j2, i2), (j2 + 50, i2 + 50), color=(0,0,255))
                    cv.imshow('test', img)
                    key = cv.waitKey(1)
                    if key == 27: #if ESC is pressed, exit loop
                        cv.destroyAllWindows()
                        exit()
                    img = clone.copy()

#Compares the current area of interest with the emoji
def checkForEmoji(img, emoji, X, Y):
    roi = img[Y:Y+50, X:X+50]
    diff = cv.subtract(roi, emoji)
    diff = diff.astype(np.uint8)
    if np.count_nonzero(diff) < 2700:
        return X, Y
    return -1, -1

# Checking of all the different squares
def checkSquares(img, emoji, i, j, i2, j2):
    check = checkForEmoji(img, emoji, j, i)
    if check[0] != -1 and check[1] != -1:
        return check
    check = checkForEmoji(img, emoji, j2, i)
    if check[0] != -1 and check[1] != -1:
        return check
    check = checkForEmoji(img, emoji, j, i2) 
    if check[0] != -1 and check[1] != -1:
        return check
    check = checkForEmoji(img, emoji, j2, i2)
    return check 

main()