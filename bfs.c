#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 16

typedef struct Node {
	struct Node* exploredFrom;
	int position[2];
	int explored;
} Node;

Node* new_Node(int x, int y){
	Node* n = (Node*)malloc(sizeof(Node));
	
	n->position[0] = x;
	n->position[1] = y;

	return n; 
}

int contains(Node*(grid)[GRID_SIZE], Node element){
        for (int y = 0; y < GRID_SIZE; ++y){
                for (int x = 0; x < GRID_SIZE; ++x){
	        	if (grid[y][x] == element)
				return 1;        
		}
        }
	return 0;
}

Node* bfs(Node(*grid)[GRID_SIZE], Node* start, Node* finish){
	Node** explored = (Node**)malloc(GRID_SIZE * GRID_SIZE * sizeof(Node*));
	while (){
		
	}
}

int main(){
	// Allocate & populate grid
	Node(*NODES)[16] = (Node(*)[16])malloc(GRID_SIZE * GRID_SIZE * sizeof(Node));
	for (int y = 0; y < GRID_SIZE; ++y){
		for (int x = 0; x < GRID_SIZE; ++x){
			NODES[y][x].position[0] = x;
			NODES[y][x].position[1] = y; 
			printf("(%d, %d) ", NODES[y][x].position[0], NODES[y][x].position[1]);		
		}
		printf("\n");
	}
	
	printf("%d", contains(NODES, NODES[0][0]);

	return 0;
}
