import cv2

def find_and_draw_emojis(image_path, emoji_path, threshold):
    # Read the image and emoji
    image = cv2.imread(image_path)
    emoji = cv2.imread(emoji_path, cv2.IMREAD_GRAYSCALE)

    # Get the dimensions of the image and emoji
    image_height, image_width, _ = image.shape
    emoji_height, emoji_width = emoji.shape

    # Create a grayscale version of the image
    gray_image = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

    # Match the template of the emoji in the image
    result = cv2.matchTemplate(gray_image, emoji, cv2.TM_CCOEFF_NORMED)

    # Get the coordinates of the matched emojis
    loc = cv2.findNonZero((result >= threshold).astype('uint8'))

    # Print coordinates of the found emojis
    if loc is not None:
        for pt in loc:
            x, y = pt.ravel()
            print("Emoji coordinates (x={}, y={})".format(x, y))
            cv2.circle(image, (x + emoji_width // 2, y + emoji_height // 2), 20, (0, 0, 255), 2)

    # Save the resultant image
    cv2.imwrite('output.jpg', image)

#
image_path = 'emoji_0.jpg'
emoji_path = 'temp_emoji.jpg'
threshold = 0.9  #threshold for matching
find_and_draw_emojis(image_path, emoji_path, threshold)
