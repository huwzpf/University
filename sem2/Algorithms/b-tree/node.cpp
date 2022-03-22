#include "Node.h"


Node::Node(int T, bool leaf):T(T), is_leaf(leaf) { //allocate all needed memory and initialize sons pointers as NULL 
	keys_count = 0;
	keys = new int[2 * T - 1];
	sons = new Node * [2 * T];
	for (int i = 0; i < 2 * T; i++) {
		sons[i] = nullptr;
	}
}


void Node::insert_not_full(int value) { //we assume right usage (node is not full)
	if (is_leaf) { 
		int i = keys_count - 1;
		for (; i >= 0 && keys[i] > value; i--) { //move keys that are larger than value to right in order to make space for new value 
			keys[i + 1] = keys[i];
		}
		keys[i + 1] = value; // simply insert value at found spot ( increment value, because keys[i] is largest key smaller than value and keys[i+1] is its copy)
		keys_count++;

	}

	else {

		int i = keys_count - 1;
		for (; i >= 0 && keys[i] > value; i--) {} //find suitable place to insert value 
		i++; // increment value, because keys[i] is largest key smaller than value and since we want to insert into son, sons[i+1] contains values larger than keys[i] but smaller than keys[i+1] 
		if (sons[i]->is_full()) { 

			split_son(i);

			if (keys[i] < value) { //after splitting son determine if value shall belong to new son containing smaller or larger values than newly inserted (by split_son()) keys[i];
				sons[i + 1]->insert_not_full(value);
			}
				
			else {
				sons[i]->insert_not_full(value);
			}
				
		}
		else { //if son isnt full recursively call this function 
			
			sons[i]->insert_not_full(value);
		}

	}
	
}

void Node::split_son(int position) {
	Node* new_node= new Node(T, sons[position]->is_leaf);

	new_node->keys_count = T - 1; 

	//copying keys and sons from node that is being split into new one

	for (int i = 0; i < T - 1; i++) {
		new_node->keys[i] = sons[position]->keys[i + T]; // sons[position] has 2T-1 keys, copy first T-1 into new node 
	}

	for (int i = 0; i < T; i++) {
		new_node->sons[i] = sons[position]->sons[i + T]; //sosn[position] has 2T keys, so copy first T into new node
	}

	sons[position]->keys_count = T - 1; //all copied, now we can reduce keys count 

	//make space in current node (parent to one that is being split)

	for (int i = keys_count; i >= position; i--) { //moving sons to right
		sons[i + 1] = sons[i];
	}


	for (int i = keys_count - 1; i >= position; i--) { //moving keys to right
		keys[i + 1] = keys[i]; 
	}

	keys[position] = sons[position]->keys[T-1]; // move median from splitted node up (we can acces it even if we set keys_count to T-1 above because we dont free memory)
	keys_count++;

	sons[position+1] = new_node;
}



void Node::print_down(FILE* stream, bool values) { //bool values determines if we want to print just values or nodes representes by ( ) 
	if (!values)
		fprintf(stream, "( ");



	if (!is_leaf) {
		for (int i = 0; i <= keys_count; i++) {//if function prints node

		//it is printing in order, so in i-th iteration of loop (which goes through every key in node) we 1) print i-th son 2) print i-th key 

			sons[i]->print_down(stream, values); //recursively print i-th son 

			if (i < keys_count) { // there are one more sons than keys so i must stay in right bounds

				fprintf(stream, " %d ", keys[i]);
			}

		}
	}
	else { // if it prints leaf - just print every key
		for (int i = 0; i < keys_count; i++) {
			fprintf(stream, "%d", keys[i]);

			if (i < keys_count - 1) //make sure we put whitespaces in correct places, no need to double whitespace at end of sequence (it will be printed elsewhere - in loop printing nodes)
				fprintf(stream, " ");
		}
	}

	if (!values)
		fprintf(stream, " )");
}

void Node::remove(int x) {
	int index = 0;

	while ( index < keys_count  && keys[index] < x ) { index++; } // i is index of largest key that is smaller than x
	
	if (is_leaf) { // cant go lower, if k was found delete it, if it wasnt it is not it the tree


		if (index<keys_count && keys[index] == x) {	//If the key k is in node x and x is a leaf, delete the key k from x
		
			remove_from_leaf(index); 
		}
		else {
			//no node to remove
			//might print some message 
		}
	}
	else { // can go lower, since current node is not a leaf 
		if (index < keys_count && keys[index] == x) {
			//the key k is in node x and x is an internal node
			remove_from_node(index); //delete k 
		}
		else { //k is not present in internal node x, determine the root of the appropriate subtree that must contain k, if k is in the tree at all
			check_node(index);//make sure it moves down to node that contains at least T-1 sons 
			if (index > keys_count)//finish by recursing on the appropriate son
				sons[index - 1]->remove(x);
			else
				sons[index]->remove(x); 
				
		}
			
	}

}


void Node::check_node(int pos) {
	if (sons[pos]->keys_count < T) {
		if (pos<keys_count && sons[pos + 1]->keys_count > T-1) { //borrow from right sibling
			borrow_right(pos);
		}
		else if (pos > 0 && sons[pos - 1]->keys_count > T - 1) { //borrow from left sibling
			borrow_left(pos);
		}

		else { // If and both immediate siblings have t - 1 keys, merge sons[pos] with one sibling
			if (pos == keys_count)
				merge(pos - 1);
			else
				merge(pos);
		}
		

	}
	//else checked son needs no modification 
}



void Node::remove_from_leaf(int pos) { //simply remove value from node
		for (int i = pos; i <keys_count; i++) {
			keys[i] = keys[i + 1];
		}
		keys_count--;
}


void Node::remove_from_node(int pos) {

	//If the child that precedes k in node x has at least T keys;
	 if (sons[pos]->keys_count >= T) {
		keys[pos] = get_pred(pos); //find the predecessor k0 of k and replace k by k0
		sons[pos]->remove(keys[pos]); //Recursively delete k0
	}

	//If the child that precedes k has fewer than t keys, then, symmetrically, examine the child that follows k in node x.
	else if (pos<keys_count && sons[pos + 1]->keys_count >= T) {  // If it has at least t keys
		keys[pos] = get_succ(pos); //find the successor k0 of k and replace k by k0
		sons[pos + 1]->remove(keys[pos]); //Recursively delete k0
	}
	// Otherwise, if both  have only t - 1 keys
	// merge k and both sons, so that x loses both k and the pointer to son successing k, and son that was predecessing k now contains 2t + 1 keys.
	else { 
		 int k = keys[pos];
		if (pos == keys_count) //since merge takes position of son that will remain and merges it with next one, make sure that it doesnt try to merge last son with son that doesnt exist
			merge(pos - 1);
		else
			merge(pos);

		//Then recursively delete k from son that was predecessing it;
		sons[pos]->remove(k);
	}
}

void Node::merge(int pos) {
	sons[pos]->keys[sons[pos]->keys_count] = keys[pos]; // move median at T-th position, it will be deleted below by overwritting
	sons[pos]->keys_count++;

	for (int i = pos; i < keys_count-1; i++)//shift keys
			keys[i] = keys[i + 1];

		keys_count--;

	for (int i = 0; i <= sons[pos + 1]->keys_count; i++) {
			if (i < sons[pos + 1]->keys_count) { //copy values from node that is being deleted into one that stays 
				sons[pos]->keys[sons[pos]->keys_count] = sons[pos + 1]->keys[i];
				sons[pos]->keys_count++;
			}

			sons[pos]->sons[T+i] = sons[pos + 1]->sons[i]; //copy sons pointers
		}

	delete(sons[pos + 1]); //delete son that was merged

	for (int i = pos+1; i <= keys_count; i++) { //shift sons
		sons[i] = sons[i + 1];
	}


}

int Node::get_succ(int pos) const {
	//search lowest next number 
	Node* current = sons[pos+1];
	while (!current->is_leaf) {
		current = current->sons[0];
	}
	return current->keys[0];
}

int Node::get_pred(int pos) const {
	//search highest predecessing number
	Node* current = sons[pos];
	while (!current->is_leaf) {
	current = current->sons[current->keys_count];
	}
	return current->keys[current->keys_count - 1];

}

void Node::borrow_right(int pos) {

	sons[pos]->keys[sons[pos]->keys_count] = keys[pos];
	keys[pos] = sons[pos + 1]->keys[0];
	for (int i = 1; i < sons[pos + 1]->keys_count; i++) {//make space for key that will be inserted
		sons[pos + 1]->keys[i - 1] = sons[pos + 1]->keys[i];
	}

	if (!sons[pos]->is_leaf) { //if son is not leaf, move its sons in order to match moved keys
		sons[pos]->sons[sons[pos]->keys_count + 1] = sons[pos + 1]->sons[0];

		for (int i = 1; i <= sons[pos + 1]->keys_count; i++) {
			sons[pos + 1]->sons[i - 1] = sons[pos + 1]->sons[i];
		}
	}
	
	sons[pos]->keys_count++;
	sons[pos + 1]->keys_count--;

}


void Node::borrow_left(int pos) {
	for (int i = sons[pos]->keys_count - 1; i >= 0; i--) {
		sons[pos]->keys[i + 1] = sons[pos]->keys[i]; //make space for key that will be inserted
	}
	sons[pos]->keys[0] = keys[pos - 1];
	keys[pos - 1] = sons[pos - 1]->keys[sons[pos - 1]->keys_count - 1];

	if (!sons[pos]->is_leaf) { //if son is not leaf, move its sons in order to match moved keys
		for (int i = sons[pos]->keys_count; i >= 0; i--) {
			sons[pos]->sons[i + 1] = sons[pos]->sons[i];
		}

		sons[pos]->sons[0] = sons[pos - 1]->sons[sons[pos - 1]->keys_count];
	}
	sons[pos]->keys_count++;
	sons[pos - 1]->keys_count--;
	
}
Node::~Node() {
	delete[] keys;
	delete[] sons;
}

Node& Node::operator= (const Node& other) {
	if (&other != this) {
		T = other.T;
			is_leaf = other.is_leaf;
			keys_count = other.keys_count;

			//copy keys

			delete[] keys;
			keys = new int[2 * T - 1];

			for (int i = 0; i < keys_count; i++) {
				keys[i] = other.keys[i];
			}


		//copy sons

		delete[] sons;
		sons = new Node * [2 * T];

		for (int i = 0; i <= keys_count; i++) {
			sons[i] = other.sons[i];
		}
		//set not used sons to nullptrs
		for (int i = keys_count; i < 2 * T; i++) {
			sons[i] = nullptr;
		}
	}
	return *this;
}

Node::Node(const Node& other) {
	T = other.T;
	is_leaf = other.is_leaf;
	keys_count = other.keys_count;

	
	//allocate memory
	sons = new Node * [2 * T];
	keys = new int[2 * T - 1];

	//copy keys

	for (int i = 0; i < keys_count; i++) {
		keys[i] = other.keys[i];
	}

	//copy sons

	for (int i = 0; i <= keys_count; i++) {
		sons[i] = other.sons[i];
	}
	//set not used sons to nullptrs
	for (int i = keys_count; i < 2 * T; i++) {
		sons[i] = nullptr;
	}

}