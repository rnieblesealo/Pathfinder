from random import randint
from copy import deepcopy
from time import sleep

SIZE = (32, 32)
GRID = []

DIRECTIONS = ((0, -1), (0, 1), (-1, 0), (1, 0))

class Node:
	position = (0, 0)
	wall = False
	path = False
	explored = False
	exploredFrom = None
	special = -1 # -1 for regular, 0 for start, 1 for finish

	def __init__(self, position, wall = False) -> None:
		self.wall = wall
		self.position = position
		self.exploredFrom = None

def init_grid(random_walls = False):
	for y in range(SIZE[1]):
		row = []
		for x in range(SIZE[0]):
			row.append(Node((y, x), wall = random_walls and randint(0, 100) < 20)) # Make tile wall by 10% chance
		GRID.append(row)

def in_bounds(x, y):
	if (x < 0 or x >= SIZE[0] or y < 0 or y >= SIZE[1]):
		return False
	return True

def bfs(start, end, inplace = False):
	if not in_bounds(start[0], start[1]) or not in_bounds(start[0], start[1]):
		print("Endpoint not in bounds!")
		return None
	
	if start == end:
		print("No pathfinding necessary; start point is finish point")
		return None

	grid = GRID if inplace else deepcopy(GRID)
	
	# Mark as start and finish
	grid[start[0]][start[1]].special = 0
	grid[end[0]][end[1]].special = 1
	
	explored = [grid[start[0]][start[1]]] # Start has been explored
	while (grid[end[0]][end[1]] not in explored):
		found_new_neighbor = False
		for i in range(len(explored)):
			for j in range(len(DIRECTIONS)):
				x = explored[i].position[0] + DIRECTIONS[j][0]
				y = explored[i].position[1] + DIRECTIONS[j][1]
				if not in_bounds(x, y):
					continue
				neighbor = grid[x][y]
				if not neighbor.wall and not neighbor.explored and not neighbor in explored:
					if not found_new_neighbor:
						found_new_neighbor = True
					explored.append(neighbor)
					neighbor.explored = True
					neighbor.exploredFrom = explored[i];
		if not found_new_neighbor:
			print("No paths exist!")
			return None

	# Generate path
	path = [grid[end[0]][end[1]]]
	current = path[0]
	while (current != grid[start[0]][start[1]]):
		path.append(current)
		current = current.exploredFrom
	path.append(grid[start[0]][start[1]])
	path.reverse()
	for i in range(len(path)):
		path[i].path = True
	return path

def show_raw():
	print("--RAW GRID--")
	for y in range(SIZE[1]):
		for x in range(SIZE[0]):
			print(' . ' if not GRID[x][y].wall else ' # ', end='')	
		print('\n')

def show(path):
	print("--BFS OUTPUT--")
	for y in range(SIZE[1]):
		for x in range(SIZE[0]):
			if GRID[x][y].wall:
				print(' # ', end='')
			elif GRID[x][y] == path[0]:
				print(' $ ', end='')
			elif GRID[x][y] == path[len(path) - 1]:
				print(' % ', end='')
			else:	
				print(' . ' if not GRID[x][y] in path else ' * ', end='')
		print('\n')

# Main execution
init_grid(random_walls=True)

start = (randint(0, SIZE[0] - 1), randint(0, SIZE[1] - 1))
end = (randint(0, SIZE[0] - 1), randint(0, SIZE[1] - 1))

path = bfs(start, end, inplace=True)