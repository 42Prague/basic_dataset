import cv2
import numpy as np
import os

def mouse_event_handler(event, x, y, flags, param):
    global mouseX, mouseY
    if event == cv2.EVENT_MOUSEMOVE:
        mouseX, mouseY = x, y

def real_time_template_matching(image_path, template_path):
    global mouseX, mouseY
    mouseX = mouseY = 0
    match_found = False  # Flag to indicate if a match has been found
    match_position = (0, 0)  # To store the match position

    if not os.path.exists(image_path):
        print(f"Image not found: {image_path}")
        return -42

    original_img = cv2.imread(image_path) # Load target image 
    template = cv2.imread(template_path, 0) # Load template image in grayscale mode
    img_gray = cv2.cvtColor(original_img, cv2.COLOR_BGR2GRAY) # Convert target image into grayscale
    w, h = template.shape[::-1] # Extract dimensions 

    window_title = f"Real-time Scanning - {os.path.basename(image_path)}"
    cv2.namedWindow(window_title, cv2.WINDOW_NORMAL)
    cv2.setMouseCallback(window_title, mouse_event_handler)

    while True:
        temp_img = original_img.copy()
        x, y = mouseX, mouseY

        x = max(0, min(x, img_gray.shape[1] - w))
        y = max(0, min(y, img_gray.shape[0] - h))

        if not match_found:
            scan_area = img_gray[y:y+h, x:x+w]
            res = cv2.matchTemplate(scan_area, template, cv2.TM_CCOEFF_NORMED)
            threshold = 0.8
            if np.amax(res) >= threshold:
                match_position = (x, y)
                match_found = True
                print(f"Match found at coordinates: ({x}, {y}) {os.path.basename(image_path)}")

        if match_found:
            cv2.rectangle(temp_img, match_position, (match_position[0] + w, match_position[1] + h), (0, 255, 0), 2)

        cv2.rectangle(temp_img, (x, y), (x + w, y + h), (0, 0, 255), 2)
        cv2.imshow(window_title, temp_img)

        key = cv2.waitKey(1) & 0xFF  # Ensure we get a 8-bit mask for consistency
        if key == 27:  # ESC key pressed
            cv2.destroyAllWindows()
            return False, None  # User chose to exit
        elif key == ord('a'):  # 'a' key pressed for left navigation
            cv2.destroyAllWindows()
            return True, -1  # Indicate to move to the previous image
        elif key == ord('d'):  # 'd' key pressed for right navigation
            cv2.destroyAllWindows()
            return True, 1  # Indicate to move to the next image

def process_images_in_sequence(folder_path, template_path, start=0, end=100):
    current_image_index = start
    while start <= current_image_index <= end:
        image_path = os.path.join(folder_path, f"emoji_{current_image_index}.jpg")
        result = real_time_template_matching(image_path, template_path)
        if isinstance(result, tuple):  # Check if the result is a tuple
            proceed, step = result
            if not proceed:
                print("Exiting...")
                break  # User chose to exit or end of the sequence
            current_image_index += step
        else: # Case of missing picture
            print(f"Unexpected result from template matching: {result}")
            current_image_index += step  
        
# Example usage
folder_path = '../data/dataset'
template_path = '../data/dataset/emoji.jpg'
process_images_in_sequence(folder_path, template_path)
