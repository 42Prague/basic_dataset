import cv2
import numpy as np
import os
import time


def mouse_event_handler(event, x, y, flags, param):
    global mouseX, mouseY
    if event == cv2.EVENT_MOUSEMOVE:
        mouseX, mouseY = x, y

def real_time_template_matching(image_path, template_path):
    #last_search_time = 0
    #search_delay = 0.001  # Seconds between searches
    global mouseX, mouseY
    mouseX = mouseY = 0
    match_found = False  # Flag to indicate if a match has been found
    match_position = (0, 0)  # To store the match position

    if not os.path.exists(image_path):
        print(f"Image not found: {image_path}")
        return False, None

    original_img = cv2.imread(image_path)  # Load target image
    template = cv2.imread(template_path, 0)  # Load template image in grayscale mode
    img_gray = cv2.cvtColor(original_img, cv2.COLOR_BGR2GRAY)  # Convert target image into grayscale
    w, h = template.shape[::-1]  # Extract dimensions

    window_title = f"Real-time Scanning - {os.path.basename(image_path)}"
    cv2.namedWindow(window_title, cv2.WINDOW_NORMAL)
    cv2.setMouseCallback(window_title, mouse_event_handler)

    search_region_size = 25  # Define the size of the search region around the cursor

    while True:
        temp_img = original_img.copy()
        x, y = mouseX, mouseY

        # Define the search region around the cursor, ensuring it stays within image bounds
        search_region_x = max(0, min(x - search_region_size // 2, img_gray.shape[1] - w))
        search_region_y = max(0, min(y - search_region_size // 2, img_gray.shape[0] - h))
        # Adjust search region end points considering the template size to prevent false positives near borders
        search_region_end_x = min(img_gray.shape[1] - w, search_region_x + search_region_size)
        search_region_end_y = min(img_gray.shape[0] - h, search_region_y + search_region_size)

        #current_time = time.time()
        if not match_found: #and current_time - last_search_time > search_delay:
            #last_search_time = current_time
            for search_x in range(search_region_x, search_region_end_x):
                for search_y in range(search_region_y, search_region_end_y):
                    scan_area = img_gray[search_y:search_y + h, search_x:search_x + w]
                    res = cv2.matchTemplate(scan_area, template, cv2.TM_CCOEFF_NORMED)
                    threshold = 0.8
                    if np.amax(res) >= threshold:
                        match_position = (search_x, search_y)
                        match_found = True
                        print(f"Match found at coordinates: ({search_x}, {search_y}) {os.path.basename(image_path)}")
                        break
                if match_found:
                    break

        if match_found:
            cv2.rectangle(temp_img, match_position, (match_position[0] + w, match_position[1] + h), (0, 255, 0), 2)

        # Drawing the red rectangle (optional) could be commented out to avoid confusion when a match is found
        cv2.rectangle(temp_img, (x, y), (x + w, y + h), (0, 0, 255), 2)
        cv2.imshow(window_title, temp_img)

        key = cv2.waitKey(1) & 0xFF
        if key == 27:  # ESC key pressed
            cv2.destroyAllWindows()
            return False, None
        elif key == ord('a'):  # 'a' key pressed for left navigation
            cv2.destroyAllWindows()
            return True, -1
        elif key == ord('d'):  # 'd' key pressed for right navigation
            cv2.destroyAllWindows()
            return True, 1


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
