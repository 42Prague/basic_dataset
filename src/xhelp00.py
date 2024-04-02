import cv2
import numpy as np
import os

def real_time_template_matching(image_path, template_path):
    if not os.path.exists(image_path):
        print(f"Image not found: {image_path}")
        return 2  # Indicate the image does not exist

    original_img = cv2.imread(image_path) #Load target image 
    template = cv2.imread(template_path, 0) #Load template image in grayscale mode
    img_gray = cv2.cvtColor(original_img, cv2.COLOR_BGR2GRAY) #Convert target image into grayscale
    w, h = template.shape[::-1] #Extract dimensions 

    # Perform template matching over the entire image
    res = cv2.matchTemplate(img_gray, template, cv2.TM_CCOEFF_NORMED)
    threshold = 0.8

    window_title = f"Real-time Scanning - {os.path.basename(image_path)}"
    cv2.namedWindow(window_title, cv2.WINDOW_NORMAL)

    # Optional: Visualize the scanning process by showing a red rectangle every few steps
    step_size = 20  # Larger step size for quicker scanning
    visualization_frequency = 50  # Update the visualization every 50 steps to maintain performance
    steps = 0

    for x in range(0, img_gray.shape[1] - w, step_size):
        for y in range(0, img_gray.shape[0] - h, step_size):
            steps += 1
            if steps % visualization_frequency == 0:
                temp_img = original_img.copy()
                cv2.rectangle(temp_img, (x, y), (x + w, y + h), (0, 0, 255), 1)
                cv2.imshow(window_title, temp_img)
                if cv2.waitKey(1) == 27:  # ESC key pressed
                    cv2.destroyAllWindows()
                    return False  # User chose to exit

    # Highlight all matches found above the threshold
    locations = np.where(res >= threshold)
    for pt in zip(*locations[::-1]):  # Switch x and y coordinates
        cv2.rectangle(original_img, pt, (pt[0] + w, pt[1] + h), (0, 255, 0), 2)

    cv2.imshow(window_title, original_img)
    if cv2.waitKey(0) == 27:  # ESC key to exit
        cv2.destroyAllWindows()
        return False  # User chose to exit
    cv2.destroyAllWindows()

    for loc in zip(*locations[::-1]):
        print(f"Match found at coordinates: {loc} {os.path.basename(image_path)}")
    
    return True  # Successful operation

def process_images_in_sequence(folder_path, template_path, start=0, end=100):
    for i in range(start, end + 1):
        image_path = os.path.join(folder_path, f"emoji_{i}.jpg")
        if not real_time_template_matching(image_path, template_path):
            print("Exiting...")
            break  # User chose to exit

# Example usage
folder_path = '../data/dataset'
template_path = '../data/dataset/emoji.jpg'
process_images_in_sequence(folder_path, template_path)
