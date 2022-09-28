/* 
	So it turns out this little fucking program decided to segfault.
	GREAT! Because I TOTALLY know why the fuck it is segfaulting.
	I can TOTALLY fix it right away! It's not like I made it 239212 times harder by using fucking TRIPLE POINTERS
	What the fuck am I doing? Why? Why the fuck?
	And also, WHY THE FUCK DOES C NOT SAY WHERE THE SEGFAULT HAPPENS?
	I get this motherfucking dumb ass language was made in ancient greece and shit, but like, GAH DAMN
	I'm gonna go take a break now. I'm probably gonna forget what this code does in the meantime, so enjoy that :D
	P.D Check out line 143, you can see me have a Pokemon-themed neuronal breakdown over there ;D
*/

#include <stdio.h>
#include <stdlib.h>

#define SIZE_X 10
#define SIZE_Y 10
#define MAX_PATH_SIZE 128

typedef struct Node{
	int* position;
	int wall;
	int explored;
	int inPath;
	int isSpecial;
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

Node* GetAtIndex(NodeListItem** head, int index){
	// Return list item at given index
	NodeListItem* current = *head;
	
	int currentIndex = 0;
	while (current != NULL){
		if (currentIndex == index){
			return current->node;
		}
		current = current->next;
		index++;
	}
	
	return NULL;
}

int ContainsNode(NodeListItem** head, Node* node){
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

int InBounds(x, y){
	if (x < 0 || x >= SIZE_X || y < 0 || y >= SIZE_Y)
		return 0;
	return 1;
}

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

	// Generate directions
	int DIRECTIONS[][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};

	// Initialize list of node items, append start
	NodeListItem* exploredNodes = (NodeListItem*)malloc(sizeof(NodeListItem));
	Append(&exploredNodes, grid[start[0]][start[1]]);
	printf("----%d----", GetAtIndex(&exploredNodes, 0)->position[0]);
	while (!ContainsNode(&exploredNodes, grid[end[0]][end[1]])){
		int foundNewNeighbor = 0;
		for (int i = 0; i < Count(&exploredNodes); ++i){
			for (int j = 0; j < 4; ++j){ // 4 is the amount of directions
				int x = GetAtIndex(&exploredNodes, i)->position[0] + DIRECTIONS[j][0];
				int y = GetAtIndex(&exploredNodes, i)->position[1] + DIRECTIONS[j][1];
				if (!InBounds(x, y)){
					continue;
				}
				Node* neighbor = grid[x][y];
				if (!neighbor->wall && !neighbor->explored && !ContainsNode(&exploredNodes, neighbor)){
					if (!foundNewNeighbor){
						foundNewNeighbor = 1;
					}
					Append(&exploredNodes, neighbor);
					neighbor->explored = 1;
					neighbor->exploredFrom = GetAtIndex(&exploredNodes, i);
				}
			}
		}
		if (!foundNewNeighbor){
			printf("No paths exist!\n");
			return NULL;
		}
	}

	// Once pathfinding is done, we generate a path of nodes, append the end to it
	NodeListItem* path = (NodeListItem*)malloc(sizeof(NodeListItem));
	Append(&path, grid[end[0]][end[1]]);
	
	// Traceback each explored tile's exploredFrom starting at end to figure out path
	Node* current = GetAtIndex(&path, 0);
	while (current != grid[start[0]][start[1]]){
		Append(&path, current);
		current = current->exploredFrom;
	}
	
	// Append the start node at the end
	Append(&path, grid[start[0]][start[1]]);

	// ! // Free used memory for explored nodes [THIS DOES NOT WORK, WE NEED METHOD TO WIPE ENTIRE LIST, FOR NOW THERE IS MEMORY LEAK]
	// free(exploredNodes);

	// Return built path
	printf("Done pathfinding, appended %d nodes to path!\n", Count(&path));
	return path;
}

int main(){
	Node*** grid = InitializeGrid();
	
	for (int y = 0; y < SIZE_Y; ++y){
		for (int x = 0; x < SIZE_X; ++x){
			printf("[%d,%d] ", grid[y][x]->position[0], grid[y][x]->position[1]);
		}
		printf("\n");
	}

	int start[2] = {2, 0};
	int end[2] = {3, 5};
	
	NodeListItem* path = BFS(grid, start, end);

	return 0;
}