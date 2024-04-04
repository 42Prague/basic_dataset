from PIL import Image
import csv
import os

def load_image(filepath):
    return Image.open(filepath)

def find_top(image):
    # Get the size of the image
    width, height = image.size
    
    # Iterate over each pixel in the image starting from the top-left corner
    for y in range(height):
        for x in range(width):
            # Get the RGB values of the pixel
            pixel = image.getpixel((x, y))
            
            # Check if the pixel is non-white
            if pixel != (255, 255, 255):
                return x, y  # Return the coordinates of the non-white pixel
    
    # If no non-white pixel is found, return None
    return None, None

dataset_directory = "../data/dataset"
csv_file_path = "../data/coordinates.csv"

# Create or open the CSV file in write mode
with open(csv_file_path, mode='w', newline='') as file:
    writer = csv.writer(file)
    writer.writerow(["Image", "X", "Y"])  # Write header row
    
    # Iterate over all files in the dataset directory
    for filename in os.listdir(dataset_directory):
        if filename.endswith(".jpg"):
            # Construct the full path to the image file
            image_path = os.path.join(dataset_directory, filename)
            
            # Load the image
            image = load_image(image_path)
            
            # Find the top-left pixel
            x , y = find_top(image)
            x = x + 1
            y = y + 2
            
            # Write the filename and coordinates to the CSV file
            writer.writerow([filename, x, y])
            print(f"Top left coordinates of {filename} written to {csv_file_path}")

print("CSV file generation complete.")
