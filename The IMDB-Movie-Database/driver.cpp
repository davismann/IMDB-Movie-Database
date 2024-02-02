// Copyright (c) 2023 Davis Mann CU Boulder
// driver cpp file

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MovieHashTable.hpp"
#include "DirectorSkipList.hpp"

using namespace std;

// Keeps track of the number of collisions
int num_collision = 0;

// Function prototypes
MovieNode* parseMovieLine(string line);
void readMovieCSV(string filename,  MovieHashTable &movieTable, DirectorSkipList &directorList);
void display_menu();

// Function of main
// Input: argc, argv[]
// Return: int
int main(int argc, char* argv[]) {
    
    // MAKE SURE NEW RANDOM NUMBER FOR SKIP LIST LEVELS
    srand(time(0));

    // CORRECT COMMAND LINE ARGS
    while(argc != 4){
        cout << "Invalid number of arguments." << endl;
        cout << "Usage: ./<program name> <csv file> <hashTable size> <skipList size>" << endl;
    }

    // CREATE HASHTABLE AND SKIPLIST
    MovieHashTable my_table = MovieHashTable(stoi(argv[2]));
    DirectorSkipList my_skip = DirectorSkipList(stoi(argv[3]), 10);

    // READ MOVIES INTO HASHTABLE AND SKIPLIST
    readMovieCSV(argv[1], my_table, my_skip);

    // USER MENU
    num_collision = my_table.getCollisions();
    int choice = 0;
    string direct_name;
    string movie_name;
    MovieNode* hold;
    DirectorSLNode* hold2;
    while(choice != 6)
    {
        display_menu();
        cin >> choice;
        if(choice > 6 || choice < 1){ // if user inputs an not valid option
            cout << "Invalid option." << endl;
        }
        cin.ignore();

        if(choice == 1){ // search for movie in hash-table and return its director
            cout << "Enter movie name:" << endl;
            getline(cin, movie_name);
            hold = my_table.search(movie_name);
            if(hold != nullptr){
                cout << "The director of " << movie_name << " is " << hold->director << endl;
            }
            else{
                cout << "Database does not contain this movie." << endl;
            }
        }
        if(choice == 2){ // search for director in skip-list and return the number of movies they have produced
            cout << "Enter director name" << endl;
            getline(cin, direct_name);
            hold2 = my_skip.search(direct_name);
            if(hold2 != nullptr){
                cout << direct_name << " directed " << hold2->movies.size() << " movies" << endl;
            }
            else{
                cout << "Database does not contain this director." << endl;
            }
        }
        if(choice == 3){ // search for movie in hash-table and return its description & stats
            cout << "Enter movie name:" << endl;
            getline(cin, movie_name);
            hold = my_table.search(movie_name);
            if(hold != nullptr){
                cout << movie_name << " is a " << hold->year << " " << hold->genre << " film staring " << hold->actors << endl;
                cout << "Plot: " << hold->description << endl;
                cout << "Rating: " << hold->rating << "/10 Metascore: " << hold->metascore << "/100" << endl;
            }
            else{
                cout << "Database does not contain this movie." << endl;
            }
        }
        if(choice == 4){ // search for director in skip-list and return the titles of the movies he has produced
            cout << "Enter director name" << endl;
            getline(cin, direct_name);
            hold2 = my_skip.search(direct_name);
            if(hold2 != nullptr){
                cout << direct_name << " directed the following movies:" << endl;
                for(int i=0; i < hold2->movies.size(); i++){
                    cout << i << ": " << hold2->movies[i]->title << endl;
                }
            }
            else{
                cout << "Database does not contain this director." << endl;
            }
        }
        if(choice == 5){ // print the skip-list
            cout << "||| SKIP-LIST |||" << endl;
            my_skip.prettyPrint();
        }
        if(choice == 6){ // exit
            return 0;
        }
    }
}

// Function to parse a CSV line into a MovieNode object
// Input: line
// Return: MovieNode*
MovieNode* parseMovieLine(string line) {
    stringstream ss(line);
    vector<string> fields;
    string field;

    // Loop through the characters in the line
    bool in_quotes = false;
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        if (c == '\"') {
            in_quotes = !in_quotes;
        } else if (c == ',' && !in_quotes) {
            // add field to vector and reset for next field
            fields.push_back(field);
            field.clear();
        } else {
            field += c;
            // handle double quotes inside a quoted field
            if (in_quotes && c == '\"' && i < line.length() - 1 && line[i+1] == '\"') {
                field += '\"'; // add the second double quote and skip it
                ++i;
            }
        }
    }
    fields.push_back(field);

    if (fields.size() != 12) {
        cerr << "Error: Invalid movie line format" << line << endl;
        return nullptr;
    }

    int rank = stoi(fields[0]);
    string title = fields[1];
    string genre = fields[2];
    string description = fields[3];
    string director = fields[4];
    string actors = fields[5];
    int year = stoi(fields[6]);
    int runtime = stoi(fields[7]);
    float rating = stof(fields[8]);
    int votes = stoi(fields[9]);
    float revenue = stof(fields[10]);
    int metascore = stoi(fields[11]);

    // Create a new MovieNode object with the parsed fields
    MovieNode* movie = new MovieNode(rank, title, genre, description, director, actors, year, runtime, rating, votes, revenue, metascore);
    return movie;
}

// Function to read a CSV file into a vector of MovieNode objects
// Input: filename, &movieTable, &directorList
// Return: void
void readMovieCSV(string filename, MovieHashTable &movieTable, DirectorSkipList &directorList) {
    
    // open file
    ifstream myfile;
    string line;
    myfile.open(filename);
    if(myfile.is_open())
    {
        getline(myfile, line); // getline the first line in the csv (the headers)
        while(getline(myfile, line))
        {
            MovieNode* temp = parseMovieLine(line); // parse line and store into a MovieNode
            movieTable.insert(temp->title, temp); // insert the movie and title into the hash-table
            directorList.insert(temp->director, temp); // insert the movie and title into the skip-list
        }
    }
    myfile.close();
}

// Function to display the menu
// Input: N/A
// Return: void
void display_menu() {

    cout << "Number of collisions:" << num_collision << endl;
    cout << "Please select an option:" << endl;
    cout << "1. Find the director of a movie" << endl;
    cout << "2. Find the number of movies by a director" << endl;
    cout << "3. Find the description of a movie" << endl;
    cout << "4. List the movies by a director" << endl;
    cout << "5. Print the director skip-list" << endl;
    cout << "6. Quit" << endl;
    cout << "Enter an Option:" << endl;
}
