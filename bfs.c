#include <stdio.h>
#include <stdlib.h>

#define SIZE_X 10
#define SIZE_Y 10

typedef struct Node{
	int* position;
	int wall;
	int explored;
	struct Node* exploredFrom;
} Node;

Node* new_Node(int x_pos, int y_pos, int wall){
	// Position and nodes go on heap
	Node* n = (Node*)malloc(sizeof(Node));
	
	n->position = (int*)malloc(2 * sizeof(int));
	n->position[0] = x_pos;
	n->position[1] = y_pos;
	
	n->wall = wall;
	n->explored = 0;
	n->exploredFrom = NULL;
	
	return n;
}


int main(){
	Node(*GRID)[SIZE_X] = (Node(*)[SIZE_X])malloc(SIZE_X * SIZE_Y * sizeof(Node));
	
	GRID[0][0] = new_Node(0, 0, 0);

	return 0;
}