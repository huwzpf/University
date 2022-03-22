#define _CRT_SECURE_NO_WARNINGS
#include "Tree.h"
#include <cmath>
#define MAX_NUMBER_LENGHT 12
void Tree::insert(int val) {
	if (root == nullptr) {//tree is empty 
		
		root = new Node(T, true); //create new root
		root->insert_not_full(val); //and insert value int newly created root
	}
	else {
		if (root->is_full()) { //if root is full tree grows up
			Node* new_root =new Node(T, false); 

			new_root->sons[0]=root; //add old root as child of new one 
			new_root->split_son(0); //split old root

			if (new_root->keys[0] < val) //new root now has one key, if value that we are adding is lesser than it, add to left branch else add to right one 
				new_root->sons[1]->insert_not_full(val);
			else
				new_root->sons[0]->insert_not_full(val);

			root = new_root; //switch root pointer to new one  
		}
		else
			root->insert_not_full(val);
	}
}

int convert_to_int(char* number) { //function to convert from char[] to int, used for loading tree
	int n = 0;
	bool negative = false;
	if (number[0] == '-')
		negative = true;

	int digits = 0;
	for (int i = 0; i < MAX_NUMBER_LENGHT; i++) { //count digits
		if (number[i] == '\0')
			break;
		digits++;
	}

	if (negative)
		digits--;


	for (int i = 0; i < MAX_NUMBER_LENGHT; i++) {
		if (number[i] == '\0')
			break;
		else {
			n +=(int)( pow(10, (digits - 1 - i)) * (number[i]-'0'));
		}
	}
	return n;
}


void Tree::print(FILE* stream, bool values) {
	if(!values) //print tree order if we are not printing  only values
		printf("%d\n", T);
	if(root!=NULL)
		root->print_down(stream, values); // call recursive function for root 
}

construction_return Tree::construct_branch(List* l, element* begin, element* end) { //function which constructs tree branch from list of characters, used for tree loading purpose
																					//function returns struct construction_return because we want to save current pointer in order not to process parts of list twice 

	Node* ret = new Node(T, true);  // function returns node (first call return noode which will be root), it is assumed that node is leaf, if it is not it will be changed as soon as child node will be found 
	
	//keeping track of son and node which we are now processing
	int sons_counter=0; 
	int keys_counter = 0;

	element* current = begin;

	while (current != end) {
	
		if (current->content == ' ') { //list passed as argument contains whitespaces in order to separate keys in nodes, but keys are processed in later part of function so it can just be passed
			current = current->next;
			continue;
		}
		
		else { // not a whitespace, so start processing a node 

			while (current != NULL && current->content != ')') { //iterate until we have found end of current node 

				if (current->content == '(') { //if found a new node 

					construction_return returned = construct_branch(l, current->next, end); // recursively call function to create new branch (using struct to mantain current pointer at right place (dont process parts twice ) ) 
					ret->sons[sons_counter] = returned.returned_node;
					current = returned.current;

					ret->is_leaf = false; // it means current node has sons, so it is not leaf 
					sons_counter++;
				}
				else if (current->content != ' ') { // current -> content must be  a number 

					int counter = 0;

					char number[MAX_NUMBER_LENGHT]; // predefined number lenght instead of reallocating memory after every char read from input or using list, assumption is that it will not be longer than 4 bytes int

					while (current->content != ' ') { //read char from input until it sees whitespace meaning end of number

						number[counter] = current->content;
						current = current->next;
						counter++;

					}
					number[counter] = '\0';

					current = current->next; //skip whitespace after number (it must be there, so why not skip it here)

					ret->keys[keys_counter] = convert_to_int(number);
					ret->keys_count++;
					keys_counter++;
				}

				else { //current -> content must be a whitespace, so we skip it
					current = current->next;
				}
			}


			return{ ret, current ? current->next : current }; //while loop finished so it must have encountered end of node or end of list, either way return porcessed node
		}
	}
	return { ret, current ? current->next : current }; //outer while loop encountered end of list, return what it has processed

}

void Tree::construct_tree() {

	List* l = new List; //create list of characters read from input
	int loaded;
	loaded = getchar(); // skip \n as function shall be used just afrer instruction constructing new tree

	while (loaded = getchar()) {
		if (loaded == '\n') //tree structure must be in one line 
			break;
		l->insert_last((char)(loaded)); //insert last to keep right order of characters
	}
	
	root = construct_branch(l, l->beg->next, l->end).returned_node; // call recursive function to load branch and set root to node returned by it

}

Node* Tree::search(int x) const  {
	//iterate through all levels of tree starting from root and going down

	Node* current=root;
	while (current != nullptr) { 
		int i = 0;

		while (i < current->keys_count && current->keys[i] <= x) { //find largest key that is lower or equal to x
			if (current->keys[i] == x) //x was found
				return current;
			else
				i++; //move to next key 
		}

		//loop above didnt find x
		if (current->is_leaf) // x is not present in tree 
			return nullptr;
		else // if current is not leaf, go down into son that might contain x
			current = current->sons[i];
	}

	return nullptr; 
}

int Tree::count_search(int x) const { // function needed for calculating caching impact 
	Node* current = root;
	int count=1; // accessing root is also counted 

	//pretty much the same as regular search() function, but return count
	while (current != nullptr) {
		int i = 0;
		while (i < current->keys_count && current->keys[i] <= x) {
			if (current->keys[i] == x)
				return count;
			else
				i++;
		}
		if (current->is_leaf)
			return count; //element was not found, but it still counts as traversing tree 
		else {
			current = current->sons[i];
			count++; //it goes down to lower level of tree so count needs to be incremented
		}
			
	}

	//there was no return in loop above so current was nullptr and it wasnt leaf so some kind of error occured
	if (current != nullptr && !current->is_leaf)
		throw(std::logic_error("there is branch ending with non-leaf node"));

	return count;
}

void Tree::remove(int x) {
	root->remove(x);
	if (root->keys_count == 0)
		root=root->sons[0];
}

void Tree::calculate_cache(FILE* in) const {

	//variables used to keep track of all needed parameters
	int cache_size;
	int count_without_cache = 0;
	int count_with_cache = 0;
	int numbers_in_cache = 0;


	int number;

	//scan size of used cache
	fscanf(in, "%d", &cache_size);
	int* arr =new int[cache_size]; //array serving as a queue, no need to implement actual queue since the assumption is that cache will be of a small size 

	while (fscanf(in, "%d", &number)) {
		int search_depth = count_search(number);
		bool cached = false;//assume that current number is not cached


		count_without_cache += search_depth; //adding to both, count without cache will be lowered if number is currently in cache 
		count_with_cache += search_depth;

		for (int i = 0; i < numbers_in_cache; i++) { //search if number is already in cache
			if (arr[i] == number) {
				count_with_cache -= search_depth;
				cached = true; //if it was found no need to add it, raise cached flag

				//FIFO cache - uses queue and does nothing if it was found 

				/*
				//Other cache proposition - LRU cache

				//in comparison to FIFO cache - it adds moving most recently used elements to front of the queue
				//
				// for example input :C 4   1 1 5 6 1 7 8 9 1 5 1 33 1 5 1 6 1 8 9 1 5 3 7 1 11 1
				// FIFO cache gives 46, but LRU gives 37

				int temp = arr[i]; //move number found in cache to the front of queue, since we delete least recently used and keep recently used ones
				for (int j = i; j<numbers_in_cache; j++) { // make space at the front and move others back
					arr[j] = arr[j+1];
				}
					arr[numbers_in_cache-1] = temp; //insert at end of queue, since first one is deleted first

				*/
				break;
			}
		}
		if (cached == false) { //if number was not found in cache 


			if (numbers_in_cache == cache_size) { //cache is full

				for (int i = 0; i < cache_size - 1; i++) // make space by shifting elements to left, front of queue is deleted by overwritting it 
					arr[i] = arr[i + 1];

				arr[numbers_in_cache - 1] = number;

			}
			else { // if cache is not full just put recently used number at end
				numbers_in_cache++;
				arr[numbers_in_cache - 1] = number;

			}

		}



	}
	printf("NO CACHE: %d CACHE: %d\n", count_without_cache, count_with_cache);
	delete[](arr);

}

void Tree::delete_branch(Node* n) {
	if (n->is_leaf)
		delete(n);
	else {
		for (int i = 0; i <= n->keys_count; i++) {
			delete_branch(n->sons[i]);
		}
		delete(n);
	}
}


Tree::~Tree() {
	delete_branch(root);
}
