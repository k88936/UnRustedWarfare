import cv2
import numpy as np

def draw_grid_with_indices(image_path, output_path):
    # Load the image
    image = cv2.imread(image_path)
    if image is None:
        print("Error: Could not load image.")
        return

    # Get image dimensions
    height, width, _ = image.shape

    # Define the size of the grid boxes
    box_size = 16

    # Create a copy of the image to draw on
    output_image = image.copy()

    # Set font for text
    font = cv2.FONT_HERSHEY_SIMPLEX
    font_scale = 0.25
    color = (255, 0, 0)  # Blue color in BGR
    thickness = 1

    # Draw the grid and indices
    index = 0
    for y in range(0, height, box_size):
        for x in range(0, width, box_size):
            # Draw vertical and horizontal lines
            cv2.line(output_image, (x, 0), (x, height), (0, 255, 0), 1)
            cv2.line(output_image, (0, y), (width, y), (0, 255, 0), 1)

            # Calculate the position for the text
            text_position = (x + 2, y + 12)

            # Put the index text on the image
            cv2.putText(output_image, str(index), text_position, font, font_scale, color, thickness)
            index += 1

    # Draw the last vertical and horizontal lines
    cv2.line(output_image, (width - 1, 0), (width - 1, height), (0, 255, 0), 1)
    cv2.line(output_image, (0, height - 1), (width, height - 1), (0, 255, 0), 1)

    # Save the output image
    cv2.imwrite(output_path, output_image)

# Example usage
draw_grid_with_indices('input.png', 'output.png')