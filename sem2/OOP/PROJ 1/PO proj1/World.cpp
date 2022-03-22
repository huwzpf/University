#include "World.h"
#include "Wolf.h"
#include "Sheep.h"
#include "Fox.h"
#include "Antelope.h"
#include "Tortoise.h"
#include "Dandelion.h"
#include "Grass.h"
#include "WolfBerry.h"
#include "SosnowskyHogweed.h"
#include "Guarana.h"
#include "Human.h"

void World::simulate() {
		int c;
		cout << "PRESS P TO RANDOMLY POPUALTE WORLD OR PRESS L TO LOAD WORLD FROM SAVE FILE " << endl;
		while (c = _getch()) {
			if (c == 'p') {
				int n;
				cout << "How many organisms there will be ?" << endl;
				cin >> n;
				populate_world(n);
				break;
			}
			else if (c == 'l') {
				load_game();
				execute_turn();
				break;
			}
		}

		while (human != nullptr) // loop is active as long as human lives
			execute_turn();

		if(human==nullptr){
			while (c = _getch()) {
				if (c == 'c' ) {
					execute_turn();
				}
				else if (c == 's')
					save_game();
				else if (c == 'q')
					break;
			}
		}
		system("cls");
		cout << "END OF GAME";
}



World::World(int w, int h) :width(w), height(h) {
	// set map's width and height
	Map = new Organism * *[width];
	for (int i = 0; i < width; i++) {
		Map[i] = new Organism * [height];
		for (int j = 0; j < height; j++) { //set every element in map as nullptr since we don't initially add any ogranisms
			Map[i][j] = nullptr;
		}
	}
	//initialize random engine
	random_device r;
	rand_engine = mt19937(r());
	// initialize distribution
	utility_real_distribution_0_1 = uniform_real_distribution<double>(0.0, 1.0);
	human = nullptr;
	//open log file
	game_log.open("log.txt", ios::out);
};

World::~World() {
	organisms.clear();
	delete[] Map;
}

void World::draw_world(){
	system("cls");//clear console
	cout << " PO_PROJ1 PIOTR_SIENSKI_184297    TURN NUMBER: " << turn_counter<<" ALIVE ORGANISMS: " <<organisms.size() << endl;
	if (human != nullptr) {
		cout << "HUMAN ALIVE, AND MARKED AS 'X' ON MAP,  USE ARROWS TO MOVE HUMAN" << endl;
		cout << "PRESS 'X' TO ACTIVATE HUMAN ABILITY, PRESS 'C' TO SKIP TURN, PRESS 'S' TO SAVE GAME STATE" << endl;
	}
	else
		cout << "HUMAN IS DEAD, PRESS 'C' TO MOVE TO NEXT TURN OR  PRESS 'S' TO SAVE GAME STATE" << endl;
	cout << "    "; // make spacing to correctly line up axis 
	for (int i = 0; i < width; i++) {
		cout.width(3); // 3 is number that seems correct, making a nice spacing between characters
		cout << i;
	}
	cout  << endl << "   "; // make spacing to correctly line up axis
	for (int i = 0; i <= width; i++) {
		cout << "---"; // cout 3 chars instead of one char at cout.width(3)
	}
	cout << endl;
	for (int i = 0; i < height; i++) { //iterate through whole map and print all elements 
		cout.width(3);
		cout << i <<"|" ;
		for (int j = 0; j < width; j++) {
			cout.width(3);
			if (Map[j][i] != nullptr) {

				Map[j][i]->draw();
			}
			else {
				cout << "."; // draw '.' if this field is empty
			}
		}
		cout.width(3);
		cout <<"|";
		cout << endl;
	}
	cout << "   ";
	for (int i = 0; i <= width; i++) {
		cout << "---";
	}
	cout << endl;

	cout << "TURN LOGS\n";
	for (auto s : turn_log) {
		cout << endl << s;
	}
	turn_log.clear(); 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), { 0,0 }); //move cursor to the top so map doesnt scroll up with big turn log
}

void World::populate_world(int number) {
	populate_probabilities_vector();
	coordinates human_pos = { (int)(get_random_number() * (width - 1)),(int)(get_random_number() * (height - 1)) }; // set human to random position, it isnt done in generate rand organism function since there always is only one human
	human = new Human(turn_counter, human_pos, this);
	add_organism(human , human_pos);

	Organism** arr = generate_random_organisms(number);
	for (int i = 0; i < number; i++)
		add_organism(arr[i], arr[i]->get_position());
	
}

void World::execute_turn() { 
	turn_counter++;
	Organism* organism;
	action_result act;
	for (int i = 0; i < organisms.size();i++) {
		organism = organisms[i];
		if (organism->is_waiting()) {
			organism->stop_waiting(); //make organisms that were born this turn active 
		}
		else {
			act=organism->action();
			switch (act) {
			case MOVED:
				//only animals can move
				handle_move(organism, dynamic_cast<Animal*>(organism)->get_move());
				break;
			case SPREADED:
				breed_organisms(organism->get_position(), organism);
				//ony plants return this action result
				//if given plant can attempt to spread more than once
				for (int i = 1; i < dynamic_cast<Plant*>(organism)->get_spread_attempts(); i++) {
					if (organism->action() == SPREADED)
						breed_organisms(organism->get_position(), organism);
				}

				break;
			case STAYED:
				//log(organism->display() + " didnt move");
				break;
			}
			if(organism->does_murder_enrything_around())
				kill_everything_around(organism->get_position());
		}

	}

	save_to_game_log();
	draw_world();
	sort(organisms.begin(), organisms.end(), // sort vector to assure right order of organisms
		[](Organism* a, Organism* b) {
			return *a < *b;
		}
		);
}

void World::save_to_game_log()
{
	game_log << "TURN NUMBER " << turn_counter << "\n";
	for (auto s : turn_log) {
		game_log << s << '\n';
	}
}

void World::handle_move(Organism* organism, coordinates moved_pos)
{
	collision_result collision;

	if (Map[moved_pos.x][moved_pos.y] != nullptr && Map[moved_pos.x][moved_pos.y] != organism ) { //if organism moved to non empty field and assure that organism did move 
		collision = Map[moved_pos.x][moved_pos.y]->collision(organism); // call collision on 'defending' organism

		switch (collision) {
		case BOTH_DIED:
			remove_organism(organism);
			remove_organism(Map[moved_pos.x][moved_pos.y]);
			break;
		case DIED:
			remove_organism(Map[moved_pos.x][moved_pos.y]);
			move_organism(organism, moved_pos);
			break;
		case KILLED_OPPONENT:
			remove_organism(organism);
			break;
		case DEFENDER_ESCAPED:
			handle_escape(moved_pos);
			break;
		case ATTACKER_ESCAPED:
			log("attacker escaped");
			break;
		case BREEDED:
			breed_organisms(organism->get_position(), organism);
			break;
		}
		
	}
	else {
		move_organism(organism, moved_pos); //organism moved to empty field
	}
}

void World::handle_escape(coordinates moved_pos)
{
	coordinates position_to_escape = find_free_adjacent(moved_pos, 1, false);
	if (position_to_escape != moved_pos) { //make sure free adjecent field was found 
		move_organism(Map[moved_pos.x][moved_pos.y], position_to_escape);
	}
}

void World::breed_organisms(coordinates moved_pos, Organism* organism)
{
	coordinates new_pos = find_free_adjacent(moved_pos, 1, false);
	if (new_pos != moved_pos) { //  new_pos == moved pos means there is no  free adjecent field 
		add_organism(generate_organism(organism->get_species_name(), new_pos), new_pos);
	}
	else {
		//log("no place for " + (string)(organism->get_species_name()) +" to spread ");
	}

}

void World::move_organism(Organism* o, coordinates new_position) {
	log(o->display() + " moves to " + new_position.convert_to_string());
	Map[o->get_position().x][o->get_position().y] = nullptr;
	Map[new_position.x][new_position.y] = o;
	o->set_position(new_position);
}

void World::remove_organism(Organism* o) {
	if (human == o)
		human = nullptr;
	Map[o->get_position().x][o->get_position().y] = nullptr;
vector<Organism*>::iterator it = find(organisms.begin(), organisms.end(), o); //find organism in vector and remove it 
	organisms.erase(it);
}

void World::add_organism(Organism* o, coordinates position) {
	organisms.push_back(o);
 	Map[position.x][position.y] = o;
}

void World::kill_everything_around(coordinates position) {
	for (int i = -POISON_RADIUS; i <= POISON_RADIUS; i++) { // check everywhere around 
		for (int j = -POISON_RADIUS; j <= POISON_RADIUS; j++) {
			if (i == 0 && j == 0)
				continue; //except place where hogweed grows 
			else {
				if (position.x + i >= 0 && position.x + i < width && position.y + j < height && position.y + j + j >= 0) { //if checked field is in bound of map
					Organism* organism = Map[position.x+i][position.y+j];
					if (organism != nullptr) {
						log(organism->get_species_name() + (string)" died"); //kill organism standing there 
						remove_organism(organism);
					}
				}

			}
		}
	}
}


coordinates World::find_free_adjacent(coordinates position, int range, bool search_wider_if_not_found) const { //range defines how far away should be searched


	for (int i = -range; i <= range; i++) {// searching in 2xrange+1 square grid around position  
		for (int j = -range; j <= range; j++) {
			if (position.x + i >= 0 && position.x + i < width && position.y + j < height && position.y + j >= 0) //make sure checked position is not out of map's bounds
				if (Map[position.x + i][position.y + j] == nullptr) // if free field was found
					return { position.x + i, position.y + j };
		}
	}
	//not found, search in wider range
	if (search_wider_if_not_found) {
		if (position.x + range + 1 < width && position.x - range - 1 >= 0 && position.y + range + 1 < height && position.y - range - 1 >= 0) //make sure checked position is not out of map's bounds
			return find_free_adjacent(position, range++, true);
	}


	return position; //if function didnt return before, return initial position meaning no free adjecent field was found 
}



Organism** World::generate_random_organisms(int n) { // function takes number of organisms to generate
	Organism** ret = new Organism * [n]; // returns array of organisms
	double proabilities_sum = 0;
	for (generation_probability p : probabilities_vector) { //count sum of all probabilities
		proabilities_sum += p.probability;
	}

	//initialize distributions used for all random factors
	uniform_int_distribution<> position_x_distribution(0, width-1);
	uniform_int_distribution<> position_y_distribution(0, height-1);
	uniform_real_distribution<> organism_probabilities_distribution(0, proabilities_sum);
	double generated_number;
	
	for (int i = 0; i < n; i++) {
		int sum = 0;
		generated_number = organism_probabilities_distribution(rand_engine); //generate number that will determine what kind of organism will be generated
		for (int j = 0; j < probabilities_vector.size(); j++) {
			if (generated_number < probabilities_vector[j].probability + sum) { //found right organism to generate 
				// example of used algorithm :
				// 3 organisms with probabliliteis 1,2,3 distribution(assumung int not real) is -0-1-2-3-4-5-6- if generated number is in range (0;1) choose first organism, if in (1;3) choose second, if in (3;6) choose third 
				// this algorithm assures it doesnt need all probabilities to sum up to some defined number and assures easy scaling
				coordinates new_organism_position;
				//choose random coordinates from int distributions
				new_organism_position.x = position_x_distribution(rand_engine);
				new_organism_position.y = position_y_distribution(rand_engine);
				// if random coordinates are not free, seek around it
				if (Map[new_organism_position.x][new_organism_position.y] != nullptr)
					new_organism_position = find_free_adjacent(new_organism_position,1,true);

				if (Map[new_organism_position.x][new_organism_position.y] != nullptr) { //no free space nearby
					i--; // try again
					break;
				}

				ret[i]=generate_organism(probabilities_vector[j].organism_type, new_organism_position); // generate choosen organism at choosen field
				break;
			}
			else sum += probabilities_vector[j].probability; // add current organism's probability to sum
		}
	}
	
	return ret;
}



Organism* World::generate_organism(const char* c, coordinates position) { // function taking spiecie name and returning object of this spiecie
	if (!strcmp(c, "sheep"))
		return new Sheep(turn_counter, position, this);
	else if (!strcmp(c, "wolf"))
		return new Wolf(turn_counter, position, this);
	else if (!strcmp(c, "tortoise"))
		return new Tortoise(turn_counter, position, this);
	else if (!strcmp(c, "fox"))
		return new Fox(turn_counter, position, this);
	else if (!strcmp(c, "antelope"))
		return new Antelope(turn_counter, position, this);
	else if (!strcmp(c, "dandelion"))
		return new Dandelion(turn_counter, position , this);
	else if (!strcmp(c, "grass"))
		return new Grass(turn_counter, position , this);
	else if (!strcmp(c, "guarana"))
		return new Guarana(turn_counter, position , this);
	else if (!strcmp(c, "wolfberry"))
		return new WolfBerry(turn_counter, position , this);
	else if (!strcmp(c, "hogweed"))
		return new SosnowskyHogweed(turn_counter, position , this);
	else if (!strcmp(c, "human")) {
		Organism* ret = new Human(turn_counter, position, this);
		human = ret; //assign human pointer if human organism is created with this function
		return ret;
	}
		
	else
		return nullptr;
}

void World::populate_probabilities_vector() { //populate vector containing probabilities of generating each organism
	probabilities_vector.push_back({ "sheep", SHEEP_GENERATION_PROBABILITY });
	probabilities_vector.push_back({ "wolf", WOLF_GENERATION_PROBABILITY });
	probabilities_vector.push_back({ "tortoise", TORTOISE_GENERATION_PROBABILITY });
	probabilities_vector.push_back({ "fox", FOX_GENERATION_PROBABILITY });
	probabilities_vector.push_back({ "antelope", ANTELOPE_GENERATION_PROBABILITY });
	probabilities_vector.push_back({ "dandelion", DANDELION_GENERATION_PROBABILITY });
	probabilities_vector.push_back({ "grass", GRASS_GENERATION_PROBABILITY });
	probabilities_vector.push_back({ "guarana", GUARANA_GENERATION_PROBABILITY });
	probabilities_vector.push_back({ "wolfberry", WOLFBERRY_GENERATION_PROBABILITY });
	probabilities_vector.push_back({ "hogweed",HOGWEED_GENERATION_PROBABILITY });
	
}

void World::log(string s) {
	turn_log.push_back(s);
}



bool World::check_game_state_validity() { //check if all existing organisms have valid positions
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			if (Map[i][j] != nullptr) {
				coordinates pos = Map[i][j]->get_position();
				if (pos.x >= width || pos.x < 0 || pos.y < 0 || pos.y >= height)
					throw(logic_error("incorrect organism position"));
			}
				
		}
	}

}

void World::save_game() {
	check_game_state_validity(); // make sure it saves valid game state 

	ofstream file;
	file.open("save.txt", ios::out);
	file << organisms.size() << '\n'; //first line means amount of organisms
	for (Organism* organism : organisms) { // every line in save file means one organism in format: name, x position, y position, age
		file << organism->get_species_name() << " " << organism->get_position().x << " " << organism->get_position().y << " " << organism->get_age() <<'\n';
	}
	file.close();
}

void World::load_game() {
	//clear all organism-containing structures
	organisms.clear();
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			Map[i][j] = nullptr;
		}
	}
	game_log << "NEW GAME\n";
	//load organisms in format described in save_game()
	ifstream file;
	file.open("save.txt", ios::in);
	int size;
	int x_pos;
	int y_pos;
	int age;
	string name=" ";
	file >> size;
	for (int i = 0; i < size; i++) {
		file >> name >> x_pos >> y_pos >> age;
		turn_counter = age;
		add_organism(generate_organism(name.c_str(), { x_pos,y_pos }), { x_pos,y_pos });
	}
	turn_counter = 0;
	file.close();
}