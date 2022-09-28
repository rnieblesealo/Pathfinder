import pygame
import bfs
import sys

TILE_SIZE = 16
DISPLAY_SIZE = (TILE_SIZE * bfs.SIZE[0], TILE_SIZE * bfs.SIZE[1])

pygame.init()
pygame.display.set_caption("BFS Visualizer")

display = pygame.display.set_mode(DISPLAY_SIZE)

class Tile:
    DEFAULT_COLOR = (255, 255, 255)
    EXPLORED_COLOR = (98, 168, 252)
    START_COLOR = (235, 64, 52)
    END_COLOR = (255, 200, 0)
    WALL_COLOR = (150, 150, 150)
    PATH_COLOR = (44, 242, 83)

    tracked = None
    surface = None
    color = None

    def __init__(self, tracked) -> None:
        self.tracked = tracked
        self.surface = pygame.Surface((TILE_SIZE, TILE_SIZE))

        self.set_color(self.DEFAULT_COLOR)

    def set_color(self, color):
        if (self.color != color):
            self.color = color
            self.surface.fill(self.color)

    def update(self):
        if self.tracked.wall:
            self.set_color(self.WALL_COLOR)
        elif self.tracked.special == 0:
            self.set_color(self.START_COLOR)
        elif self.tracked.special == 1:
            self.set_color(self.END_COLOR)
        elif self.tracked.path:
            self.set_color(self.PATH_COLOR)
        elif self.tracked.explored:
            self.set_color(self.EXPLORED_COLOR)

        display_pos = (self.tracked.position[0] * TILE_SIZE, self.tracked.position[1] * TILE_SIZE)
        display.blit(self.surface, display_pos)

tiles = []
for y in range(bfs.SIZE[1]):
    row = []
    for x in range(bfs.SIZE[0]):
        row.append(Tile(bfs.GRID[y][x]))
    tiles.append(row)

while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            sys.exit()

    for y in range(bfs.SIZE[1]):
        for x in range(bfs.SIZE[0]):
            tiles[y][x].update()

    for x in range(1, bfs.SIZE[0]):
        pygame.draw.line(
            display, 
            (25, 25, 25), 
            (TILE_SIZE * tiles[x][0].tracked.position[0], 0), 
            (TILE_SIZE * tiles[x][0].tracked.position[0], DISPLAY_SIZE[1]), 
            1
            )

    for y in range(1, bfs.SIZE[1]):
        pygame.draw.line(
            display, 
            (25, 25, 25), 
            (0, TILE_SIZE * tiles[y][0].tracked.position[0]), 
            (DISPLAY_SIZE[0], TILE_SIZE * tiles[y][0].tracked.position[0]), 
            1
            )

    pygame.display.update()