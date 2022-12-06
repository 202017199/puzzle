#include <iostream>
#include <vector>
#include <utility>
#include <queue>
#include <stack>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <bits/stdc++.h>
using namespace std;


// state space tree nodes
struct Node
{
    // stores the parent node of the current node
    // helps in tracing path when the answer is found
    Node* parent;

    // stores matrix
    int mat[3][3];

    // stores blank tile coordinates
    int x, y;

    // stores the number of misplaced tiles
    int cost;

    // stores the number of moves so far
    int level;
};

// Function to print N x N matrix
int print(int mat[3][3])
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
            printf("%dr", mat[i][j]);
        printf("\n");
    }
    return 0;
}

// Function to allocate a new node
Node* newNode(int mat[3][3], int x, int y, int newX,
            int newY, int level, Node* parent)
{
    Node* node = new Node;

    // set pointer for path to root
    node->parent = parent;

    // copy data from parent node to current node
    memcpy(node->mat, mat, sizeof node->mat);

    // move tile by 1 position
    swap(node->mat[x][y], node->mat[newX][newY]);

    // set number of misplaced tiles
    node->cost = INT_MAX;

    // set number of moves so far
    node->level = level;

    // update new blank tile cordinates
    node->x = newX;
    node->y = newY;

    return node;
}

// bottom, left, top, right
int row[] = { 1, 0, -1, 0 };
int col[] = { 0, -1, 0, 1 };

// Function to calculate the number of misplaced tiles
// ie. number of non-blank tiles not in their goal position
int heuristic_value(int given_state[3][3], int goal_state[3][3])
{
    int count = 0;
    for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
        if (given_state[i][j] && given_state[i][j] != goal_state[i][j])
        count++;
    return count;
}

// Function to check if (x, y) is a valid matrix cordinate
int isvalid(int x, int y)
{
    return (x >= 0 && x < 3 && y >= 0 && y < 3);
}

// print path from root node to destination node
int Intermediate_steps(Node* root)
{
    if (root == NULL)
        return 0;
    int matrix_count_till_now=Intermediate_steps(root->parent);
    print(root->mat);

    cout<<endl;
    return 1+matrix_count_till_now;
}

// Comparison object to be used to order the heap
struct comp
{
    bool operator()(const Node* lhs, const Node* rhs) const
    {
        return (lhs->cost + lhs->level) > (rhs->cost + rhs->level);
    }
};

// Function to solve N*N - 1 puzzle algorithm using
// Branch and Bound. x and y are blank tile coordinates
// in initial state
int hill_climbing(int given_state[3][3], int row_value, int col_value, int goal_state[3][3])
{
    // Create a priority queue to store live nodes of
    // search tree;
    priority_queue<Node*, std::vector<Node*>, comp> pq;

    // create a root node and calculate its cost
    Node* root = newNode(given_state, row_value, col_value, row_value, col_value, 0, NULL);
    root->cost = heuristic_value(given_state, goal_state);

    // Add root to list of live nodes;
    pq.push(root);

    // Finds a live node with least cost,
    // add its childrens to list of live nodes and
    // finally deletes it from the list.
    while (!pq.empty())
    {
        // Find a live node with least estimated cost
        Node* min = pq.top();

        // The found node is deleted from the list of
        // live nodes
        pq.pop();

        // if min is an answer node
        if (min->cost == 0)
        {
            // print the path from root to destination;
            return Intermediate_steps(min);

        }

        // do for each child of min
        // max 4 children for a node
        for (int i = 0; i < 4; i++)
        {
            if (isvalid(min->x + row[i], min->y + col[i]))
            {
                // create a child node and calculate
                // its cost
                Node* child = newNode(min->mat, min->x,
                            min->y, min->x + row[i],
                            min->y + col[i],
                            min->level + 1, min);
                child->cost = heuristic_value(child->mat, goal_state);

                // Add child to list of live nodes
                pq.push(child);
            }
        }
    }
    return 0;
}


int main()
{
    int goal_state[3][3];
    int given_state[3][3];

     // Value 0 is used for empty space

    // Blank tile coordinates in initial
    // configuration

    int row_value = 0, col_value = 0;

    for(int i=0;i<3;++i)
    {
        for(int j=0;j<3;++j)
        {
            cin>>given_state[i][j];
            if(given_state[i][j]==0)
            {
                row_value=i;
                col_value=j;
            }
            cin>>goal_state[i][j];
        }
    }
    int ans=hill_climbing(given_state, row_value, col_value, goal_state);
    cout<<"Number of matrixes in the most optimal path are "<<ans;

    return 0;
}
