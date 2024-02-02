// Copyright (c) 2023 Davis Mann CU Boulder
// MovieHashTable cpp file
// Implementation of a hash-table

#include<iostream>
#include <vector>
#include "MovieHashTable.hpp"
using namespace std;

// Constructor for MovieHashTable with default size
MovieHashTable::MovieHashTable() {
    table_size = 2000;
    table = new MovieNode*[table_size];
        for(int i=0; i < table_size; i++) 
        {
            table[i] = nullptr;
        }
    n_collisions = 0;
}   

// Constructor for MovieHashTable with given size
MovieHashTable::MovieHashTable(int s) {
    table_size = s;
    table = new MovieNode*[table_size];
        for(int i=0; i < table_size; i++) 
        {
            table[i] = nullptr;
        }
    n_collisions = 0;
}

// Destructor for MovieHashTable that deletes all nodes in the hash table
MovieHashTable::~MovieHashTable() {
    
    MovieNode *curr_next; // stores the next MovieNode of current
    for(int i=0; i < table_size; i++) // loop through the table
    {
        MovieNode *curr = table[i]; // curr is the MovieNode pointer at i
        while(curr != nullptr) // loop through the curr's LL
        {
            curr_next = curr->next; // store next MovieNode
            delete curr; // delete current MovieNode
            curr = curr_next; // current MovieNode is now the next MovieNode
        }
        table[i] = nullptr; // once all LL nodes of MovieNode pointer at i have been deleted, set MovieNode pointer at i to nullptr
    }
    delete []table; // delete the entire table
}

// Hash function for MovieHashTable that returns an index in the hash table for a given movie title
// Input: title
// Return: int
int MovieHashTable::hash(string title) {
    
    // hash_key is the final key and compute stores key until modulo
    int hash_key = 0;
    int compute = 0;
    for(int i=0; i < title.length(); i++) // loop through to find the sum of the title (add all ASCII together)
    {
        compute += title[i];
    }
    if(title.length() > 8) // if title is greater than all numbers in my identikey
    {
        compute += title[6]+'D' + title[7]+'A' + title[6]+'M' + title[7]+'A'; // takes the sum of a char in the movie title at index of my numbers in my identikey and the ASCII values of my letters in my identikey. Once each sum has been added (4 sums), I add all those together to compute
 
    }
    else
    {
        compute += 'D' + 'A' + 'M' + 'A'; // add ASCII of DAMA to compute
    }
    hash_key = compute % table_size; // modulo to fit table
    return hash_key;
}

// Inserts a movie node into the hash table with the specified title
// Input: title, movie
// Return: void
void MovieHashTable::insert(string title, MovieNode* movie) {

   // get the hash from the movie title
   int hashIndex = hash(movie->title);

   // if index is open, add movie and return
   if(table[hashIndex] == nullptr){
        table[hashIndex] = movie; 
        return;
   }
   // if index is not open, chain movie at that index and movie's next is nullptr
   else{
        setCollisions();
        MovieNode *curse = table[hashIndex];
        while(curse->next) // traverse to end of chain
        {
            curse = curse->next;
        }
        curse->next = movie; // previous end of chain is connected to movie
        movie->next = nullptr; // movie is now the end of chain to its next is nullptr
   }
}

// Searches for a node in the hash table with the specified title
// Input: title
// Return: MovieNode*
MovieNode* MovieHashTable::search(string title) {
    
    // get the hash from title
    int hashIndex = hash(title);

    // if table at hashIndex does not have anything at it
    if(table[hashIndex] == nullptr){
        return nullptr;
    }
    // loop through hashIndex's LL, if the title matches return, else table[hashIndex] becomes its next
    else{
        MovieNode *curse = table[hashIndex];
        while(curse != nullptr){
            if(curse->title == title){
                return curse;
            }
            else{
                curse = curse->next;
            }
        }
    }
return nullptr;
}

// Returns the number of collisions that have occurred during insertion into the hash table
// Input: N/A
// Return: void
int MovieHashTable::getCollisions() {
    return n_collisions;
}

// Increments the number of collisions that have occurred during insertion into the hash table
// Input: N/A
// Return: void
void MovieHashTable::setCollisions() {
    n_collisions++; 
}
