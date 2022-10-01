#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Terminal colors :D
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define SIZE_X 10
#define SIZE_Y 10
#define MAX_PATH_SIZE 128

typedef struct Node{
	int* position;
	int wall;
	int explored;
	int inPath;
	int isSpecial; // -1 for regular, 0 for start, 1 for end
	struct Node* exploredFrom;
} Node;

typedef struct NodeListItem{
	Node* node;
	struct NodeListItem* next;
} NodeListItem;

void Append(NodeListItem** head, Node* node){
	NodeListItem* new_entry = (NodeListItem*)malloc(sizeof(NodeListItem));

	new_entry->node = node;
	new_entry->next = *head;

	*head = new_entry;
}

void Insert(NodeListItem** head, Node* node, int index){
	NodeListItem* new_entry = (NodeListItem*)malloc(sizeof(Node));
	NodeListItem* current = *head;

	new_entry->node = node;
	new_entry->next = NULL;

	if (index == 0){
		new_entry->next = *head;
		*head = new_entry;
		return;
	}

	for (int i = 0; i < index--; ++i){
		current = current->next;
	}

	new_entry->next = current->next;
	current->next = new_entry;
}

void Delete(NodeListItem** head, int index){
	NodeListItem* del;
	NodeListItem* current = *head;

	if (index == 0){
		del = *head;
		*head = (*head)->next;
		free(del);
	}

	for (int i = 0; i < index--; ++i){
		current = current->next;
	}

	del = current->next;
	current->next = del->next;
	free(del);
}

int Count(NodeListItem** head){
	// Return amount of entries in list
	NodeListItem* current = *head;
	
	int count = 0;
	while (current != NULL){
		current = current->next;
		count++;
	}
	
	return count;
}

Node* Index(NodeListItem** head, int index){
	// Return list item at given index
	NodeListItem* current = *head;
	
	int currentIndex = Count(head) - 1;
	while (current != NULL){
		if (currentIndex == index){
			return current->node;
		}
		current = current->next;
		currentIndex--;
	}
	
	return NULL;
}

int Contains(NodeListItem** head, Node* node){
	// Return 1 if node exists in list, 0 if it doesn't
	NodeListItem* current = *head;
	
	while (current != NULL){
		if (current->node == node){
			return 1;
		}
		current = current->next;
	}
	
	return 0;
}

Node* new_Node(int x_pos, int y_pos, int wall){
	// Position and nodes go on heap
	Node* n = (Node*)malloc(sizeof(Node));
	
	n->position = (int*)malloc(2 * sizeof(int));
	n->position[0] = x_pos;
	n->position[1] = y_pos;
	
	n->wall = wall;
	n->explored = 0;
	n->inPath = 0;
	n->isSpecial = -1;

	n->exploredFrom = NULL;
	
	return n;
}

Node*** InitializeGrid(){
	Node*** grid = (Node***)malloc(SIZE_Y * sizeof(Node***)); // ! A wild TRIPLE POINTER appears!
	for (int y = 0; y < SIZE_Y; ++y){ 						  // * You used ITERATE! It was very effective!
		Node** row = (Node**)malloc(SIZE_X * sizeof(Node**)); // ! Oh no! DOUBLE POINTER joins the fight!
		for (int x = 0; x < SIZE_X; ++x){ 					  // * You used ITERATE! Your function is now O(n^2)! Fuck!
			row[x] = new_Node(x, y, 0);						  // * You tried ASSIGN! ROW[X] is not dead inside anymore!
		}
		grid[y] = row; 										  // ! TRIPLE POINTER hit the GRID[Y]! It then FUCKING DIED!
	}
	return grid; 											  // * You have successfully captured TRIPLE POINTER!
}

void RandomizeWalls(Node*** grid, int chance){
	for (int y = 0; y < SIZE_Y; ++y){
		for (int x = 0; x < SIZE_X; ++x){
			int n = rand() % 100;
			if (n <= chance){
				grid[y][x]->wall = 1;
			}
		}
	}
}

int InBounds(x, y){
	if (x < 0 || x >= SIZE_X || y < 0 || y >= SIZE_Y)
		return 0;
	return 1;
}

// * Note that this BFS function works in place; that is, the values of the original grid's nodes are modified forever.
NodeListItem* BFS(Node*** grid, int* start, int* end){
	// No pathfinding if endpoint is out of bounds or endpoints are same
	if (!InBounds(start[0], start[1]) || !InBounds(end[0], end[1])){
		printf("Endpoint out of bounds!\n");
		return NULL;
	}

	else if (start[0] == end[0] && start[1] == end[1]){
		printf("Endpoints are the same; no pathfinding necessary!\n");
		return NULL;
	}

	else if (grid[start[1]][start[0]]->wall || grid[end[1]][end[0]]->wall){
		printf("Either or both endpoints are a wall; pathfinding cannot occur!\n");
		return NULL;
	}

	// Mark start and finish
	grid[start[1]][start[0]]->isSpecial = 0;
	grid[end[1]][end[0]]->isSpecial = 1;

	// Generate directions
	int DIRECTIONS[][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

	// Initialize list of node items, append start to it, mark start as so using special flag
	NodeListItem* exploredNodes = NULL;
	Append(&exploredNodes, grid[start[1]][start[0]]);
	while (!Contains(&exploredNodes, grid[end[1]][end[0]])){
		int foundNewNeighbor = 0;
		for (int i = 0; i < Count(&exploredNodes); ++i){
			for (int j = 0; j < 4; ++j){ // 4 is the amount of directions				
				// Get coordinates of prospect neighbor
				int x = Index(&exploredNodes, i)->position[0] + DIRECTIONS[j][0];
				int y = Index(&exploredNodes, i)->position[1] + DIRECTIONS[j][1];
				
				// Check if coordinates are in bounds
				if (!InBounds(x, y)){
					continue;
				}
				
				// If so, match coordinates to grid neighbor and append this neighbor to explored; modify attributes accordingly
				if (!grid[y][x]->wall && !grid[y][x]->explored && !Contains(&exploredNodes, grid[y][x])){
					if (!foundNewNeighbor){
						foundNewNeighbor = 1;
					}
					
					Append(&exploredNodes, grid[y][x]);
					grid[y][x]->explored = 1;
					grid[y][x]->exploredFrom = Index(&exploredNodes, i);
				}
			}
		}
		
		// If none of our explored nodes was able to find neighbors, no path can exist
		if (!foundNewNeighbor){
			printf("No paths exist!");
			return NULL;
		}
	}

	// Once exploring is done, we generate a path of nodes, append the end node to it
	NodeListItem* path = NULL;
	Append(&path, grid[end[1]][end[0]]);

	// TODO This part seems broken
	// If exploring is done and there are no more than four neighbors; it means we have reached the destination. Therefore, a neighbor of start is the end node. Return the end node only.
	if (Count(&exploredNodes) <= 4){
		printf("Done pathfinding! Appended end node to path only, as it was adjacent to start!\n");
		return path;
	}
	
	// Otherwise traceback each explored tile's exploredFrom starting at end to figure out path
	Node* current = Index(&path, 0);
	while (current != grid[start[1]][start[0]]){
		Append(&path, current);
		
		current->inPath = 1;
		current = current->exploredFrom;
	}
	
	// Append the start node at the end
	Append(&path, grid[start[1]][start[0]]);

	// ! As of right now program does not free used memory; this is a fatal issue, as this particular implementation is memory-heavy

	// Return built path
	printf("Done pathfinding, appended %d nodes to path!\n", Count(&path));
	return path;
}

void ShowGrid(Node*** grid){
	for (int y = 0; y < SIZE_Y; ++y){
		for (int x = 0; x < SIZE_X; ++x){
			if (grid[y][x]->wall){
				printf(ANSI_COLOR_BLUE " # " ANSI_COLOR_RESET);
			}
			else if(grid[y][x]->isSpecial == 0)
				printf(ANSI_COLOR_YELLOW " S " ANSI_COLOR_RESET);
			else if (grid[y][x]->isSpecial == 1){
				printf(ANSI_COLOR_RED " F " ANSI_COLOR_RESET);
			}
			else if (grid[y][x]->inPath){
				printf(ANSI_COLOR_GREEN " x " ANSI_COLOR_RESET);
			}
			else{
				printf(" . ");
			}
		}
		printf("\n");
	}
}

int main(){
	srand(time(NULL));
	
	Node*** GRID = InitializeGrid();
	
	printf("Randomizing walls!\n");
	
	RandomizeWalls(GRID, 15);
	ShowGrid(GRID);
	
	int sx = rand() % SIZE_X;
	int sy = rand() % SIZE_Y;
	int ex = rand() % SIZE_X; 
	int ey = rand() % SIZE_Y;

	int s[2] = {sx, sy};
	int e[2] = {ex, ey};
	
	NodeListItem* PATH = BFS(GRID, s, e);

	ShowGrid(GRID);

	return 0;
}
