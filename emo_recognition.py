import numpy as np
import cv2
import pandas as pd
import os

def compare_pixels(roi, emoji, threshold=10):
    if roi.shape != emoji.shape:
        raise ValueError("Image and roi must have the same shape for comparison.")

    # Iterate through each pixel in each channel
    for i in range(roi.shape[0]):
        for j in range(roi.shape[1]):
            for k in range(roi.shape[2]):
                if abs(int(roi[i, j, k]) - int(emoji[i, j, k])) > threshold:
                    return False  # Return False if any pixel difference exceeds threshold
    return True

def find_emoji(image, emoji):
    # Get dimensions of image and emoji
    img_height, img_width, img_channels= image.shape
    emoji_height, emoji_width, emoji_channels = emoji.shape
    # print("img_height, img_width:", img_height, img_width)
    # print(" emoji_height, emoji_width:", emoji_height, emoji_width)

    canvas = np.copy(image)

    # Iterate through image using sliding window
    for y in range(img_height - emoji_height + 1):
        for x in range(img_width - emoji_width + 1):
            # Extract region of interest (ROI) from image
            roi = image[y:y+emoji_height, x:x+emoji_width]

            # Visualisation every 25th pixel
            if y % 25 == 0 and x % 25 == 0:
                cv2.rectangle(canvas, (x, y), (x + emoji_width, y + emoji_height), (0, 0, 255), 1)
                cv2.imshow("VISUALISATION", canvas)
                cv2.waitKey(1)
                canvas = np.copy(image)

            if compare_pixels(roi, emoji, 15):
                cv2.rectangle(canvas, (x, y), (x + emoji_width, y + emoji_height), (0, 255, 0), 1)
                cv2.imshow("VISUALISATION", canvas)
                cv2.waitKey(1000)
                cv2.destroyAllWindows() 
                return (x, y)  # Return coordinates if found
			
    cv2.waitKey(1) 
    cv2.destroyAllWindows() 
    return None  # Return None if emoji not found


# Load and parse labels.csv file
labels_df = pd.read_csv('data/labels.csv', sep=';')
labels_df.columns = labels_df.columns.str.replace(';', '')

for index, row in labels_df.iterrows():
    image_path = os.path.join('data', 'dataset', row['file_name'])

    # cv2.imread returns a NumPy array representing the image.
    image = cv2.imread(image_path)
    if image is None:
        print("Image", image_path, "could not be read.")
        continue

    # Load emoji image
    emoji = cv2.imread('data/emoji_example.jpg')
    if emoji is None:
        print("Emoji could not be read.")
        continue

    # Find emoji in image
    coordinates = find_emoji(image, emoji)

    # Output coordinates if found
    if coordinates is not None:
        print("Emoji found in", image_path, "at coordinates:", coordinates)
    else:
        print("Emoji not found in", image_path)