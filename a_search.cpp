/*
*A search algorithm
*/
#include <bits/stdc++.h> 
#include <iostream>
using namespace std; 

// bottom, left, top, right 
//for the 8 puzzle board
int row[] = { 1, 0, -1, 0 }; 
int col[] = { 0, -1, 0, 1 }; 


struct Open
{
	//stores the parent of the current node,
	// this will be used to trace path when goal is found
	struct Open* parent; 

	//stores the matrix of puzzle board
	int puzzle_board[3][3];

	//the coordinates of space or zero 
	int x,y;

	//misplaced tiles, will be used to calculate the heuristic(h1)
	int cost;

	//the number of moves going down to reach the goal(h2) manhattan distance
	int depth;
};

/*======================this function will be use to allocate new node=====================*/
Open* newOpen(int puzzle_board[3][3], int x, int y, int newX, 
			int newY, int depth, Open* parent) 
{ 
	//allocates memory size equal to the size of the new node-->newOPen
	Open* open = new Open; 

	// set pointer for the path going to the root
	open->parent = parent; 

	// copy data from parent node to current node --> helps in tracing back
	memcpy(open->puzzle_board, puzzle_board, sizeof open->puzzle_board); 

	// move tile by 1 position --> following A*
	swap(open->puzzle_board[x][y], open->puzzle_board[newX][newY]); 

	// set number of misplaced tiles 
	//INT_MAX->defines the maximum value of an int
	open->cost = INT_MAX; 

	// set number of moves going down expanding nodes
	open->depth = depth; 

	// update new space/zero cordinates 
	open->x = newX; 
	open->y = newY; 

	return open; 
} 
/*==========================================end===========================================*/

/*===================Compare object that will be used in heap============================*/
//heap will dynamically-allocate memory-->stored in RAM
struct compare 
{ 
	bool operator()(const Open* lhs, const Open* rhs)
	{ 
		return (lhs->cost + lhs->depth) > (rhs->cost + rhs->depth); 
	} 
}; 


/*======================================end==============================================*/


/*The next part will be the functions will be using in this program
*
*/

/*==========fumctions to check if x and y is a valid coordinate of space/zero tile=======*/
int valid_coordinate(int x, int y)
{
	return (x >= 0 && x < 3 && y >= 0 && y < 3); 
}
/*=========================================end==========================================*/

/*==============================calculation cost function===============================*/
int calc_cost(int initial[3][3], int final[3][3])
{
	int count = 0;
	for(int i = 0; i<3; i++)
	{
		for(int j=0;j<3;j++)
		{
			if(initial[i][j] && initial[i][j] != final[i][j])//this will check the non-blank tiles 
															//if they are in their goal position or not
			{
				count++;
			}
		}
	}
	return count;
}

/*======================================end=============================================*/

/*===========================function for printing the puzzle Board=====================*/
void puzzle_print(int puzzle_board[3][3])
{
	for(int i =0; i<3;i++)
	{
		for(int j = 0; j<3; j++)
		{
			cout << " "<< puzzle_board[i][j];
		}
		cout << "\n";
	}
}
/*==========================================end========================================*/

/*==============function for printing the path from initial to goal ==================*/
void path_print(Open* root)
 {
   if(root==NULL)
   	return;

   path_print(root->parent);
   puzzle_print(root->puzzle_board);

   cout << " \n";
 }
/*====================================end=============================================*/

/*================this function is for N*N-1 where 1 is the blank tile==============*/
//the algorithm used is branch and bound while attempting to use A* fringe(open and close) 
// not the A* with BFS
void solve_puzzle(int initial[3][3], int x, int y, 
		int final[3][3]) 
{ 
	
	priority_queue<Open*, std::vector<Open*>, compare> pq; // <-- this will Create a priority queue to store live nodes of the search tree


	Open* root = newOpen(initial, x, y, x, y, 0, NULL); //<-- creates a root node
	root->cost = calc_cost(initial, final); //<--calculates the cost

 
	pq.push(root); //<--Add root the open list/live nodes
	
	/*this will find the node with least cost->put it in Close
	*then it's children to the Open
	*then trace it back to find the best path->manhattan
	*/
	while (!pq.empty()) 
	{ 
		// Find a live node with least estimated cost 
		Open* min = pq.top(); //<--this will find the node with least cost
		pq.pop();//<--the found node will be transfer to Close list then deletes from here

		
		if (min->cost == 0)//<--if min is the goal node
		{ 
			
			path_print(min);//<--this will print the path from root to goal  
			return; 
		} 

		for (int i = 0; i < 4; i++)//<--the max children of node is 4, because its 3x3 
		{  							//it will loop for each child of least cost node
			if (valid_coordinate(min->x + row[i], min->y + col[i])) 
			{ 
				
				Open* child = newOpen(min->puzzle_board, min->x, //<--this will create a child node
							min->y, min->x + row[i], 
							min->y + col[i], 
							min->depth + 1, min); 
				child->cost = calc_cost(child->puzzle_board, final); //<--this will calculate the cost

				
				pq.push(child); //<--add child of the opened least cost node
			} 
		} 
	} 
} 

/*============================end=============================*/

int main()
{
	 
	int initial[3][3]; 
	int x =0, y = 0;

	int final[3][3] = //this is the puzzle goal
	{ 
		{1, 2, 3}, 
		{8, 0, 4}, 
		{7, 6, 5} 
	}; 
	
	cout << "Please enter the initial config of puzzle board: ";
	cout << "\n";
	cout << "put zero for the blank tile. ";
	cout << "\n";
	for(int i = 0; i<3;i++)// ask user to input the initial config of the 8 puzzle board
	{
		for(int j= 0; j<3;j++)
		{
			
			cin >> initial[i][j];

			if(initial[i][j] == 0)//this will find the coordinate of initial
			{					  //blank tile coordinate
				x = i;
				y = j;
			}
			
		}
	}


	 
	
	solve_puzzle(initial, x, y, final); 

	
	return 0;
}
