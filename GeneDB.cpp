/*
 * GeneDB.cpp
 * Project 1, Spring 2019, Tufts University
 * Modified by: Timothy White
 * Date: 03/ 28/ 2019
 */
 
#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;

//Constants
const string print_db = "p";
const string CAN_mutate = "cm";
const string find_cs = "fcs";
const string PRINT_all = "pa";
const string CAN_evolve = "ce";
const string can_cycle_mut = "ccm";
const string QUIT = "q";

// Struct for genes
struct Gene;
struct Mutation
{
    Gene *DNA;
    int energy;
};
struct Gene
{
    string source_DNA;
    int num_mutations = 0;
    Mutation mutatable_DNA[5];
};

// Function prototypes
void print_database(Gene database[], int data_length);
bool can_mutate(Gene source, Gene target, int max_allwed);
void perform_can_mutate(Gene database[], int frst_line);
void find_common_sequence(Gene database[], int data_length);
void print_all(Gene source, Gene target, int max_mutate_allowed, 
               string mut_add );
void perform_print_all(Gene database[], int frst_line);
int can_evolve(Gene source, Gene target, int max_evolve_cost);
void perform_can_evolve(Gene database[], int frst_line);
void can_cyclically_mutate(Gene source, int max_mutate_allowed);
void perform_can_cycle(Gene database[], int frst_line);
void fill_sources (Gene database[], string file, int frst_line);
void read_command (string command, Gene database[], int frst_line);
void fill_mutatations(Gene database[], string file, int frst_line);
int read_first_line(string data_file);
int compute_array_size (Gene database[], int data_length);
int create(string name, Gene database[], int frst_line);
int lookup(string name, Gene database[], int frst_line);
char to_lowercase(char curr);


int main() {
    string command, data;
    cout << "Enter file ";
    cin >> data;
    int frst_line = read_first_line(data);
    Gene database[frst_line];
    for (int i = 0; i < frst_line; i++){
        database[i].source_DNA = "-";
    }
    fill_sources(database, data, frst_line);
    cout << "Enter a command (p, cm, fcs, pa, ce, ccm, or q) ";
    cin >> command;
    for (char &curr : command){
    curr = to_lowercase(curr);
    }
    while (command != QUIT){
        read_command(command, database, frst_line);
        cout << "Enter a command (p, cm, fcs, pa, ce, ccm, or q) ";
        cin >> command;
        for (char &curr : command){
            curr = to_lowercase(curr);
        }
    }
    if (command == QUIT){
     return 0;
    }
}
/* Name: read_command
 * Purpose: Reads the user input into the program
 * Return: Nothing
 * Paramater: Gene, string, int
 */
void read_command (string command , Gene database[], int frst_line){
    if (command == print_db){
        print_database(database, frst_line);
    }
    if (command == CAN_mutate){
        perform_can_mutate(database, frst_line);
    }
    if (command == find_cs){
        find_common_sequence(database, frst_line);
    }
    if (command == PRINT_all){
        perform_print_all(database, frst_line);
    }
    if (command == CAN_evolve){
        perform_can_evolve(database, frst_line);
    }
    if (command == can_cycle_mut){
        perform_can_cycle(database, frst_line);
    }
}
/* Name: to_lowercase
 * Purpose: Converts first cin string to all lowercase characters
 * Return: The lowered characters
 * Paramater: char
 */
char to_lowercase(char curr){
    if (curr >= 'A' and curr <= 'Z'){
        return curr + 32;
    }
    return curr;
}
/* Name: perform_can_cycle
 * Purpose: Performs the can cyclically mutate function
 * Return: Nothing
 * Paramater: Gene,int
 */
void perform_can_cycle(Gene database[], int frst_line){
    string source;
    int max;
    cin >> source >> max;
    int x = lookup(source, database, frst_line);
    if (x == -1){
        cout << "Source not in database. ";
        return;
    }
    Gene source_gene = database[x];
    can_cyclically_mutate(source_gene, max); 
}
/* Name: perform_print_all
 * Purpose: Performs the print all function
 * Return: Nothing
 * Paramater: Gene,int
 */
void perform_print_all(Gene database[], int frst_line){
    string source, target;
    int max;
    cin >> source >> target >> max;
    int x = lookup(source, database, frst_line);
    if (x == -1){
        cout << "Source not in database. ";
        return;
    }
    int y = lookup(target, database, frst_line);
    if (y == -1){
        cout << "Target not in da database mane. ";
        return;
    }
    Gene source_gene = database[x];
    Gene target_gene = database[y];
    string path;
    bool timmy = can_mutate(source_gene, target_gene, max);
    if (timmy == false){
        cout << "There are no mutation paths from " 
        << source_gene.source_DNA << " to " << target_gene.source_DNA 
        << endl;
    }
    print_all(source_gene, target_gene, max, path);
}
/* Name: perform_can_mutate
 * Purpose: Performs the can mutate function
 * Return: Nothing
 * Paramater: Gene,int
 */
void perform_can_mutate(Gene database[], int frst_line){
    string source, target;
    int max;
    cin >> source >> target >> max; 
    int x = lookup(source, database, frst_line);
    if (x == -1){
        cout << "Source not in database. ";
        return;
    }
    int y = lookup(target, database, frst_line);
    if (y == -1){
        cout << "Target not in da database mane. ";
        return;
    }
    Gene source_gene = database[x];
    Gene target_gene = database[y];
    bool mutatable = can_mutate(source_gene, target_gene, max);
    if (mutatable == true){
        cout << source << " can mutate into " << target << " in " << max
        << " steps or less." << endl;
    }
    if (mutatable == false){
        cout << source << " cannot mutate into " << target << " in " << max
        << " steps or less." << endl;
    }
}
/* Name: perform_can_evolve
 * Purpose: Performs the can evolve function
 * Return: Nothing
 * Paramater: Gene,int
 */
void perform_can_evolve(Gene database[], int frst_line){
    string source, target;
    int max;
    cin >> source >> target >> max;
    int x = lookup(source, database, frst_line);
    if (x == -1){
        cout << "Source not in database. ";
        return;
    }
    int y = lookup(target, database, frst_line);
    if (y == -1){
        cout << "Target not in da database mane. ";
        return;
    }
    Gene source_gene = database[x];
    Gene target_gene = database[y];
    int evolvable = can_evolve(source_gene, target_gene, max);
    if (evolvable == 1){
        cout << source << " can mutate into " << target << " with <= " 
        << max << " evolutionary costs." << endl;
    }
    if (evolvable == 0){
        cout << source << " cannot mutate into " << target
        << " with only <= " << max << " evolutionary costs." << endl;
    }  
    if (evolvable == -1){
        cout << "It is not possible for " << source << " to mutate into " 
        << target << endl;
    }
}
/* Name: read_first_line
 * Purpose: Reads the first line of the file
 * Return: Number of the file's first line
 * Paramater: string
 */
int read_first_line(string data_file){
    ifstream in; 
    in.open(data_file);
    if (not in.is_open()){
        cout << "File can't be opened ";
        return 0;
    }
    int x;
    in >> x;
    in.close();
    return x;
}
/* Name: fill_sources
 * Purpose: Fills the sources portion of the database
 * Return: Nothing
 * Paramater: Gene, string, int
 */
void fill_sources(Gene database[], string file, int frst_line){
    ifstream in;
    string line;
    in.open(file);
    if (not in.is_open()){
        cout << "File can't be opened ";
    }
    int skip_first;
    string skip_endl, tempstring;
    in >> skip_first;
    getline(in, skip_endl);
    while (not in.eof()) {
        getline(in, line);
        stringstream ss(line);
        ss >> tempstring;
        string mutation_name, mutation_cost;
        int a = lookup(tempstring, database, frst_line);
        if (a == -1){
            create(tempstring, database, frst_line);
        }
    }
    in.close();
    fill_mutatations(database, file, frst_line);
}
/* Name: print_database
 * Purpose: Prints out entire database
 * Return: Nothing
 * Paramater: Gene, int
 */
void print_database(Gene database[], int data_length){
 for (int i = 0; i < data_length; i++){
     cout << "== " << database[i].source_DNA << " ==" << endl;
     cout << "Mutations: ";
     if (database[i].num_mutations == 0) {
        cout << "None" << endl;
     }
     for (int j = 0; j < database[i].num_mutations; j++){
        cout << database[i].mutatable_DNA[j].DNA -> source_DNA
        << " - Cost: " << database[i].mutatable_DNA[j].energy << endl;
    }
 }
}
/* Name: fill_mutations
 * Purpose: Fills the mutations portion of the database
 * Return: Nothing
 * Paramater: Gene, string, int
 */
void fill_mutatations(Gene database[], string file, int frst_line){
    ifstream in;
    string line, tempstring;
    int i = 0;
    in.open(file);
    if (not in.is_open()){
        cout << "File can't be opened ";
    }
    getline(in, tempstring);
    while (not in.eof()) {
        int energy = 0;
        int j = 0;
        getline(in, line);
        stringstream ss(line);
        ss >> tempstring;
        string mutation_name;
        while (ss >> mutation_name){
            int k =lookup(mutation_name, database, frst_line);
            database[i].mutatable_DNA[j].DNA = &(database[k]);
            ss >> energy;
            database[i].mutatable_DNA[j].energy = energy;
            j++;
            database[i].num_mutations ++;
        }
        i++;
    }
    in.close();
}
/* Name: can_mutate
 * Purpose: Checks if a given source can evolve into target with given steps
 * Return: Returns true if some sequence of mutations exist false otherwise
 * Paramater: Gene(2), int
 */
bool can_mutate(Gene source, Gene target, int max_allwed){
    if (max_allwed == 0){
        return false;
    }
    for (int i = 0; i < source.num_mutations; i++){
        if (source.mutatable_DNA[i].DNA ->source_DNA == target.source_DNA){
            return true;
        }
    }
    for (int j = 0; j < source.num_mutations; j++){
        bool x = can_mutate(*source.mutatable_DNA[j].DNA, target, 
                            max_allwed - 1);
        if (x == true){
            return true;
        }
        if (max_allwed - 1 == 0){
            break;
        } 
    }
    return false; 
}
/* Name: find_common_sequence
 * Purpose: Finds any common sequences in the database
 * Return: Nothing
 * Paramater: Gene, int
 */
void find_common_sequence(Gene database[], int data_length) {
    int max_mutes = 10;
    bool x = true;
    bool y = false;
    for (int i = 0; i < data_length; i++){
        for(int j = 0; j < data_length; j++){
            if (i == j) {
                continue;
            }
            x = can_mutate(database[i], database[j], 
                           max_mutes);
            if (x == false){
                break;
            }
        }
        if (x == true){
            cout << database[i].source_DNA << " is a common sequence. " << endl;
            y = true;
        }
    }
    if (y == false){
        cout << "There are no common sequences in the database. " << endl;
    }
}   
/* Name: print_all
 * Purpose: Prints all evolve paths possible from a given source to target
 * Return: Nothing
 * Paramater: Gene(2), int
 */
void print_all(Gene source, Gene target, int max_mutate_allowed, 
               string mut_add){
    if (mut_add == ""){
        mut_add = source.source_DNA;
    } 
    else{
    mut_add += " --> " + source.source_DNA;
    }
    if (max_mutate_allowed == 0){
        return;
    }
    for (int i = 0; i < source.num_mutations; i++){
        if (source.mutatable_DNA[i].DNA ->source_DNA == target.source_DNA){
            mut_add += "-->" + target.source_DNA;
            cout << mut_add << endl;
        }
    }
    for (int j = 0; j < source.num_mutations; j++){
        print_all(*source.mutatable_DNA[j].DNA, target, 
                            max_mutate_allowed - 1, mut_add);
        if (max_mutate_allowed - 1 == 0){
            break;
        } 
    } 
}
/* Name: can_evolve
 * Purpose: Checks if a given source can evolve into target with given energy
 * Return: Returns 0 if possible w/ more energy, 1 if possible w/ given energy,
           or -1 if not possible at all
 * Paramater: Gene(2), int
 */
int can_evolve(Gene source, Gene target, int max_evolve_cost){
    int evolve_track = -1;
    if (max_evolve_cost < 0){
        if (can_mutate(source, target, 10))
            return 0;
        else
            return -1;
    }
    for (int i = 0; i < source.num_mutations; i++){
        if (source.mutatable_DNA[i].DNA ->source_DNA == target.source_DNA){
            max_evolve_cost -= source.mutatable_DNA[i].energy;
            if (max_evolve_cost < 0)
                return 0;
            else
                return 1;
        }
    }
    for (int j = 0; j < source.num_mutations; j++){
        if (can_mutate(*source.mutatable_DNA[j].DNA, target, 10)){ 
            evolve_track = can_evolve(*source.mutatable_DNA[j].DNA, target, 
                           max_evolve_cost - source.mutatable_DNA[j].energy);
            if (evolve_track == 0)
                return 0;
            if (evolve_track == 1)
                return 1;
        }
    }
    return evolve_track;
}
/* Name: can_cyclically_mutate
 * Purpose: Checks if a given DNA source can cyclically mutate
 * Return: Nothing
 * Paramater: Gene, int
 */
void can_cyclically_mutate(Gene source, int max_mutate_allowed){
    int x = can_mutate(source, source, max_mutate_allowed);
    if (x == true){
        cout << source.source_DNA << " can perform cyclic mutation in "
        << max_mutate_allowed << " steps or less." << endl;
    }
    else {
        cout << source.source_DNA << " cannot perform cyclic mutation in "
        << max_mutate_allowed << " steps or less." << endl;
    }
}
/* Name: lookup
 * Purpose: Sees if the string (name) given is in the database
 * Return: Returns the index of name if in database or -1 if not in database
 * Paramater: int
 */
int lookup(string name, Gene database[], int frst_line){
    for (int i = 0; i < frst_line; i++){
        if (database[i].source_DNA == name){
            return i;
        }
    }
    return -1;
}
/* Name: create
 * Purpose: Adds source DNA to database
 * Return: Returns the current position of the source added
 * Paramater: string, Gene, int
 */
int create(string name, Gene database[], int frst_line){
    int curr_pos = 0;
    for (int i = 0; i < frst_line; i++){
        if (database[i].source_DNA  == "-"){
            database[i].source_DNA = name;
            curr_pos = i;
            return curr_pos;
        }
    }
    return curr_pos;
}
