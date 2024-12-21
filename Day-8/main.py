def read_positions(file_path):
    positions = []
    with open(file_path, 'r') as file:
        for line in file:
            pos = line.strip().split(',')
            positions.append((int(pos[0]), int(pos[1])))
    return positions

def create_grid(size):
    return [['.' for _ in range(size)] for _ in range(size)]

def place_positions_on_grid(grid, positions):
    for posx, posy in positions:
        if 0 <= posx < len(grid[0]) and 0 <= posy < len(grid):
            grid[posy][posx] = 'X'

def print_grid(grid):
    for row in grid:
        print(' '.join(row))

def main():
    file_path = './Day-8/t.txt'
    grid_size = 12
    positions = read_positions(file_path)
    grid = create_grid(grid_size)
    place_positions_on_grid(grid, positions)
    print_grid(grid)

if __name__ == "__main__":
    main()