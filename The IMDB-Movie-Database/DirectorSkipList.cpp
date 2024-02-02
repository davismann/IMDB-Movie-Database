// Copyright (c) 2023 Davis Mann CU Boulder
// DirectorSkipList cpp file
// Implementation of a skip-list

#include<iostream>
#include <vector>
#include "DirectorSkipList.hpp"
using namespace std;

// Constructor for DirectorSkipList with default capacity and levels
DirectorSkipList::DirectorSkipList() {
    levels = DEFAULT_LEVELS;
    capacity = DEFAULT_CAPACITY;
    size = 0;
    head = new DirectorSLNode("dummy", levels);
}

// Constructor for DirectorSkipList with given capacity and levels
DirectorSkipList::DirectorSkipList(int _cap, int _levels) {
    levels = _levels;
    capacity = _cap;
    size = 0;
    head = new DirectorSLNode("dummy", levels);
}

// Destructor for DirectorSkipList that deletes all nodes in the skip list (but not the MovieNodes, which are shared with other data structures)
DirectorSkipList::~DirectorSkipList() {
    
    // since level 0 will have all the nodes, loop through it until the next is not nullptr
    DirectorSLNode *curr = head;
    DirectorSLNode *curr_next = curr->next[0];
    while(curr->next[0] != nullptr)
    {
        delete curr; // delete the node
        curr = curr_next; // store curr as curr_next
        curr_next = curr->next[0]; // curr_next is now curr's next
    }
    delete curr;
    delete curr_next;
}

// Inserts a movie node into the skip list with the specified director
// Input: director, _movie
// Return: void
void DirectorSkipList::insert(string director, MovieNode* _movie) {
    
    DirectorSLNode *searcher = search(director);

    if(searcher != nullptr) // IF DIRECTOR DOES EXIST, ADD MOVIE TO VECTOR
    { 
        searcher->movies.push_back(_movie);
    }
    else // DIRECTOR DOES NOT EXIST, CREATE NEW NODE AND ADD TO SKIPLIST
    {
        // add size of next vector
        int add_levels = 1;
        while(add_levels < levels && rand() % 2 == 0){
            add_levels++;
        }
        DirectorSLNode *new_node = new DirectorSLNode(director, levels);
        new_node->movies.push_back(_movie);
        DirectorSLNode *prev[levels];
        DirectorSLNode *curr = head;

        // loop through levels from top to find spot to insert new_node
        for(int i=levels-1; i >= 0; i--){
            while(curr->next[i] != nullptr && curr->next[i]->director < director)
            {
                curr = curr->next[i];
            }
            prev[i] = curr;
        }

        // connect previous node to new node and new node to previous next at each level (Essentially filling next[] for the new node & connecting previous node to new node)
        for(int i=add_levels-1; i >= 0; i--){
            new_node->next[i] = prev[i]->next[i];
            prev[i]->next[i] = new_node;
        }
    }
}

// Searches for a node in the skip list with the specified director
// Input: director
// Return: DirectorSLNode*
DirectorSLNode *DirectorSkipList::search(string director) {
    
    DirectorSLNode *curr = head;

    // LOOP THROUGH SKIP-LIST AND ONLY MOVE CURR IF NEXT IS DIRECTOR IS LESS THAN THE ONE YOU ARE LOOKING FOR
    for(int i=levels-1; i >= 0; i--){
        while(curr->next[i] != nullptr && curr->next[i]->director < director)
        {
            curr = curr->next[i];
        }
    }

    // CURR SHOULD BE THE NODE PREVIOUS TO THE ONE WE ARE LOOKING FOR, THEREFORE CHANGE IT TO ITS NEXT AT BOTTOM LEVEL
    curr = curr->next[0];
    if(curr != nullptr && curr->director == director){
        return curr;
    }
    return nullptr;
}

// Pretty-prints the skip list
// Input: N/A
// Return: void
void DirectorSkipList::prettyPrint() {
    
    // loop through levels from top, and print the level
    for(int i=levels-1; i >= 0; i--){
        DirectorSLNode *curr = head;
        cout << "Level " << i << ": ";

        // loop through curr's next vector until reaches nullptr (end of i level)
        while(curr != nullptr)
        {
            cout << curr->director << " -> ";
            curr = curr->next[i];
        }
        cout << "\n";
    }
}
