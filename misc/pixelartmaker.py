import numpy as np

def generate_pixel_vertices(input_file):
    # Read the input file
    with open(input_file, 'r') as f:
        grid = [list(map(int, line.strip().split())) for line in f]
    
    # Convert to numpy array for easier processing
    grid = np.array(grid)
    
    # Lists to store vertices and colors
    vertices = []
    colors = []
    
    # Grid dimensions
    rows, cols = grid.shape
    
    # Pixel size and spacing
    pixel_width = 1 / cols
    pixel_height = 1 / rows
    
    for r in range(rows):
        for c in range(cols):
            # Only process if the cell is marked as 1
            if grid[r, c] == 1:
                # Calculate pixel coordinates
                x = (c / cols) * 2 - 1  # Map to [-1, 1] range
                y = 1 - ((r / rows) * 2)  # Invert y-axis, map to [1, -1] range
                
                # Vertex coordinates for the quad
                quad_vertices = [
                    x, y,
                    x + (2/cols), y,
                    x + (2/cols), y - (2/rows),
                    x, y - (2/rows)
                ]
                
                # Color (white with slight variation for depth)
                base_color = 1.0
                color_variation = 0.1
                color_depth = 1 - (r / rows) * color_variation
                
                quad_colors = [
                    base_color * color_depth, base_color * color_depth, base_color * color_depth,
                    base_color * color_depth, base_color * color_depth, base_color * color_depth,
                    base_color * color_depth, base_color * color_depth, base_color * color_depth,
                    base_color * color_depth, base_color * color_depth, base_color * color_depth
                ]
                
                # Extend the main lists
                vertices.extend(quad_vertices)
                colors.extend(quad_colors)
    
    return vertices, colors

def write_output_file(vertices, colors, output_file):
    with open(output_file, 'w') as f:
        # Combine vertices and colors
        for i in range(0, len(vertices), 2):
            # Calculate the correct color index
            color_index = (i // 2) * 3
            f.write(f"{vertices[i]} {vertices[i+1]} {colors[color_index]} {colors[color_index+1]} {colors[color_index+2]}\n")

def main():
    # Get input file path from user
    input_file = input("Enter the path to the input pixel art grid file: ").strip()
    
    # Get output file path from user
    output_file = input("Enter the path for the output vertex file: ").strip()
    
    try:
        # Generate vertices and colors
        vertices, colors = generate_pixel_vertices(input_file)
        
        # Write to output file
        write_output_file(vertices, colors, output_file)
        
        print(f"Vertex file successfully generated at {output_file}")
        print(f"Total vertices: {len(vertices) // 2}")
    
    except FileNotFoundError:
        print("Error: Input file not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

if __name__ == "__main__":
    main()