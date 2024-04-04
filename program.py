import cv2
import numpy as np

def detect_emoji(image_path, template_path):
    # Load the image and the template
    img = cv2.imread(image_path)
    template = cv2.imread(template_path, 0)

    # Convert the images to grayscale
    img_gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Perform template matching
    res = cv2.matchTemplate(img_gray, template, cv2.TM_CCOEFF_NORMED)
    threshold = 0.8
    loc = np.where(res >= threshold)

    # Draw rectangles around the matched areas
    for pt in zip(*loc[::-1]):
        bottom_right = (pt[0] + template.shape[1], pt[1] + template.shape[0])
        cv2.rectangle(img, pt, bottom_right, (0, 255, 0), 2)

    # Output the coordinates of the detected emoji
    if loc[0].any():
        print("Coordinates of :)", loc)

    # Display the result
    cv2.imshow('Detected Emojis', img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

# Example usage
image_path = 'example_image.jpg'
template_path = 'emoji_template.jpg'
detect_emoji(image_path, template_path)
