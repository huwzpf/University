#define _CRT_SECURE_NO_WARNINGS
#define MAX_BUF_LENGHT 10000
#define MAX_COLORS_NUMBER 6
#define SHUFFLE 2 //how many card deck needs to be shuffled 
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

using namespace std;

typedef enum
{
	blue = 0,
	red,
	violet,
	yellow,
	white,
	black,
	green
}color;

struct info {
	int players_number = 0;
	int green_count = 0;
	int color_count = 0;
	int green_value = 0;
	int others_number = 0;
	int player_number = 0;
	int explosion_t = 0;
	bool end = true;
}game_info;


struct card {
	color c_color;
	int value;
};

struct player {
	int cards_on_hand;
	int cards_on_deck;
	card* hand;
	card* deck;

};

struct pile {
	int count;
	card* cards;
	color pile_color = green;
};

struct cards_temp {
	int count;
	card* cards;
};

struct move_s {
	int card_number;
	int pile;
};

int compare(const void* a, const void* b) //needed for qsort
{
	return (*(int*)a - *(int*)b);
}

int convert(char* p) { //converts colors loaded from input into coressponding intiger values, p is pointer to string 
	if ((strcmp("blue", p) == 0) || (strcmp("blue\n", p) == 0)) { //stcmp(color\n) assures that it will convert correctly even if \n is placed right after color name
		return 0;
	}
	else if ((strcmp("red", p) == 0) || (strcmp("red\n", p) == 0)) {
		return 1;
	}
	else if ((strcmp("violet", p) == 0) || (strcmp("violet\n", p) == 0)) {
		return 2;
	}
	else if ((strcmp("yellow", p) == 0) || (strcmp("yellow\n", p) == 0)) {
		return 3;
	}
	else if ((strcmp("white", p) == 0) || (strcmp("white\n", p) == 0)) {
		return 4;
	}
	else if ((strcmp("black", p) == 0) || (strcmp("black\n", p) == 0)) {
		return 5;
	}
	else if ((strcmp("green", p) == 0) || (strcmp("green\n", p) == 0)) {
		return 6;
	}
	else return -1;
}






card* expand_arr(card* c, int n) { //expands array of cards to allow it to fit one more c is pointer to array and n is its initial size 
	c = (card*)realloc(c, (n + 1) * sizeof(card));
	if (c == NULL) {
		printf("error");
		exit(0);
	}
	return c;
}

card* move_arr_l(card* c, int size, int point) { //moves all the cards inn array to the left after removing one, c is pointer to array, point is point from where to start moving
	for (int i = 0; i < size - point - 1; i++) {
		c[point + i] = c[point + i + 1];
	}
	return c;
}

void explode(player* player, pile* pile) { //handles explosion event of pile triggered by player
	player->deck = (card*)realloc(player->deck, (player->cards_on_deck + pile->count) * sizeof(card)); //reallocs players deck so it will be able to fit all the cards from pile 
	if (player->deck == NULL) { 
		printf("error");
		exit(0);
	}
	for (int i = 0; i < pile->count; i++) {
		player->deck[player->cards_on_deck + i] = pile->cards[i];
	}
	player->cards_on_deck += pile->count; //sets correct numbers of cards on pile and deck 
	memset(pile->cards, NULL, pile->count - 1);// wipes all the cards from pile 
	pile->count = 0;

}

//----------------------------------------------------------calc move functions needed for performing moves---------------------------------------------------------------------

int find_max(card* cards, int count, int m) { //finds max card value from cards array but smaller than m
	int max = -1;
	for (int i = 0; i < count; i++) {
		if ((cards[i].value > max) && (m > 0 ? (cards[i].value < m) : 1) && (cards[i].value > 0)) //2nd and 3rd conditions assure that max is smaller than m but still positive
			max = cards[i].value;
	}
	return max;
}
int find_min(card* cards, int count) { //finds minimum positive card value from cards array
	int min = 1000;
	for (int i = 0; i < count; i++) {
		if ((cards[i].value < min) && (cards[i].value > 0))
			min = cards[i].value;
	}
	return min;
}

int sum_pile(pile pile) { //sums up all the values on given pile
	int sum = 0;
	for (int i = 0; i < pile.count; i++) {
		sum += pile.cards[i].value;
	}
	return sum;
}


int  check_pile_color(pile* piles_arr, color c) {// checks if any card of color c is already on any piles from given piles array
	int flag = 0;
	if (c != green) {
		for (int i = 0; i < game_info.color_count; i++) {
			if (piles_arr[i].pile_color == c)flag = i + 1;
		}
	}
	return flag; //returns on which pile the card was found and 0 if it wasnt found 
}


void choose_possible_pile(pile* piles_arr, int* possible_pile, int i) { //compares given pile with possible_pile number to pile with number i 
	if (*possible_pile > -1) { // means that there is no possible pile choosen already so it returns pile number i
		if (sum_pile(piles_arr[i]) < sum_pile(piles_arr[*possible_pile])) *possible_pile = i; // swaps possible_pile to i if sum is lower 
		if (sum_pile(piles_arr[i]) == sum_pile(piles_arr[*possible_pile])) { //if sums equal chooses one with less cards
			if (piles_arr[i].count < piles_arr[*possible_pile].count) *possible_pile = i;
		}
	}
	else
	{
		*possible_pile = i;
	}
}

void choose_pile(pile* piles_arr, int* possible_pile, color c) { //chooses most suitable pile for move c is color of card to be moved
	int flag = check_pile_color(piles_arr, c); //if there is already pile with cards of c color it marks it as suitable pile 
	if (flag == 0) {
		for (int i = 0; i < game_info.color_count; i++) { //loops through all the piles which have no assigned color or through all the piles if card's color is green 
			if (c == green || piles_arr[i].pile_color == green) {
				choose_possible_pile(piles_arr, possible_pile, i); 
			} //loop returns changed value of possible_pile
		}
	}
	else *possible_pile = flag - 1;
}


move_s calc_move_min(player player_c, pile* piles_arr) {// parameters are player who makes move and array of all piles - functions returns move which moves smallest card from hand to pile with smallest sum
	move_s move_c; // move_s struct contains pile on which card should be moved and cards number on player's hand
	int min = find_min(player_c.hand, player_c.cards_on_hand);//find smallest value on players hand 
	int possible_pile = -1;
	move_c.pile = -1;
	for (int j = 0; j < player_c.cards_on_hand; j++) { // loops through all the cards and chooses ones with min 
		if (player_c.hand[j].value == min) {
			color c = player_c.hand[j].c_color;
			choose_pile(piles_arr, &possible_pile, c); //choose suitable pile for this card's move
			if (move_c.pile > -1) {
				if (sum_pile(piles_arr[move_c.pile]) > sum_pile(piles_arr[possible_pile])) { // compares possible pile choosen above with move which is going to be made
					move_c.pile = possible_pile; // swaps choosen move to smaller one 
					move_c.card_number = j;
				}
			}
			else {
				move_c.pile = possible_pile; //if move to be made is not set jet it assigns first one matching to it 
				move_c.card_number = j;
			}
		}
	}

	return move_c;
}


move_s calc_move_max(player player_c, pile* piles_arr, int m) { // parameters are player who makes move and array of all piles, m is max value which could be returned,  by defalut m should be 0 to find just the max value  - functions returns move which moves highest card from hand to pile with smallest sum
	move_s move_c; // function works just like calc move_min except it returns max value but smaller than m
	int max = find_max(player_c.hand, player_c.cards_on_hand, m);
	int possible_pile = -1;
	move_c.pile = -1;
	for (int j = 0; j < player_c.cards_on_hand; j++) {
		if (player_c.hand[j].value == max) {
			color c = player_c.hand[j].c_color;
			choose_pile(piles_arr, &possible_pile, c);

			if (move_c.pile > -1) {
				if (sum_pile(piles_arr[move_c.pile]) > sum_pile(piles_arr[possible_pile])) {
					move_c.pile = possible_pile;
					move_c.card_number = j;
				}
			}
			else {
				move_c.pile = possible_pile;
				move_c.card_number = j;
			}
		}
	}

	return move_c;
}

move_s optimize(player player_c, pile* piles_arr, move_s move_c, int max) { //max is the highest value that could be returned by the function, 0 by default
	if ((sum_pile(piles_arr[move_c.pile]) + player_c.hand[move_c.card_number].value) >= game_info.explosion_t) { // if move doesnt make pile explode returns this move
		move_s move_c = calc_move_max(player_c, piles_arr, max); // recalculates max move, now regarding max value 
		max = find_max(player_c.hand, player_c.cards_on_hand, max); // finds new max value, lower than max given in function call
		if (max == find_min(player_c.hand, player_c.cards_on_hand))return move_c; //if max = min means that it cant do any better move and it returns current one 
		move_c = optimize(player_c, piles_arr, move_c, max); // calls the same function with new max vale and new move;

	}
	else return move_c;
}


move_s calc_optimized_move(player player_c, pile* piles_arr) {
	int max = find_max(player_c.hand, player_c.cards_on_hand, 0); // finds max value of values on hand
	move_s move_c = calc_move_max(player_c, piles_arr, 0); // calculates where to move max 
	move_c = optimize(player_c, piles_arr, move_c, 0); // if move wont make pile explode it it returns the same value, if it will make pile explode it returns move of highest card to this pile that it wont explode
	return move_c;
}

move_s calc_exp_max(player player_c, pile* piles_arr) { // called if every move makes some pile explode, calculates move which maximizes some color on player's deck
	int deck_colors[MAX_COLORS_NUMBER] = { 0 }; // array containing count of every color on player's deck
	move_s move;

	for (int i = 0; i < player_c.cards_on_deck; i++) { //loop counting how many card of a color appeared
		if (player_c.deck[i].c_color != green) {
			deck_colors[int(player_c.deck[i].c_color)]++;
		}
	}

	int max_color = 0;
	for (int i = 0; i < game_info.color_count; i++) { //chooses which one of the colors appeared the most times
		if (piles_arr[i].pile_color != green) {
			if (piles_arr[i].count + deck_colors[(int)piles_arr[i].pile_color] > max_color)
				max_color = i;
		}
	}

	int max_number = 0; 
	for (int i = 0; i < player_c.cards_on_hand; i++) { //chooses max value of card with max color to get rid of most dangerous card
		if (player_c.hand[i].c_color == max_color) {
			if (player_c.hand[i].value > player_c.hand[max_number].value) max_number = i;
		}
	}

	if ((player_c.hand[max_number].c_color != piles_arr[max_color].pile_color) && (player_c.hand[max_number].c_color != green)) { //if max number and color dont match (ex. there is pile with no color) it assigns the right pile  
		for (int i = 0; i < game_info.color_count; i++) {
			if (piles_arr[i].pile_color == player_c.hand[max_number].c_color) {
				move.pile = i;
				break;
			}
		}
	}

	move.card_number = max_number;
	move.pile = max_color;
	return move;
}

//---------------------------------------------move functions----------------------------------------------------------------------------------------------------------------




void move_card(player* player_c, pile* pile_c, int n) {// move card sitting on position n on player_c hand to pile_c pile
	if (pile_c->count != 0) { //doesnt expand empty pile 
		pile_c->cards = expand_arr(pile_c->cards, pile_c->count);
	}
	pile_c->count++;
	player_c->cards_on_hand--;
	pile_c->cards[pile_c->count - 1] = player_c->hand[n]; //swaps card 
	if ((pile_c->pile_color == green) && (player_c->hand[n].c_color != green)) pile_c->pile_color = player_c->hand[n].c_color; // assigns color to a pile if it doesnt have one
	player_c->hand = move_arr_l(player_c->hand, player_c->cards_on_hand, n); // moves hand array to left 


	if (sum_pile(*pile_c) > game_info.explosion_t)
	{
		explode(player_c, pile_c); // triggers explosion if it needs to
	}

	if (game_info.player_number == game_info.players_number) game_info.player_number = 1; //keeps track of number
	else
		game_info.player_number++;
}

//each of B C D moves follows a simillar pattern- taking players and piles arrays it return them but after move 
// it doesnt move if hand is empty, then chooses correct move to do and chooses what do do if move leads to explosion
void moveB(player* players_arr, pile* piles_arr) {
	if (players_arr[game_info.player_number - 1].cards_on_hand > 0) {
		move_s move_c = calc_move_min(players_arr[game_info.player_number - 1], piles_arr); // chooses the smallest card
		if ((sum_pile(piles_arr[move_c.pile]) + players_arr[game_info.player_number - 1].hand[move_c.card_number].value) >= game_info.explosion_t) { // chooses the highest card if previous move leads to explosion
			move_c = calc_move_max(players_arr[game_info.player_number - 1], piles_arr, 0);
		}
		move_card(&players_arr[game_info.player_number - 1], &piles_arr[move_c.pile], move_c.card_number);

	}

}


void moveC(player* players_arr, pile* piles_arr) {
	if (players_arr[game_info.player_number - 1].cards_on_hand > 0) {
		move_s move_c = calc_optimized_move(players_arr[game_info.player_number - 1], piles_arr); // moves the highest card that doesnt trigger pile explosion
		if ((sum_pile(piles_arr[move_c.pile]) + players_arr[game_info.player_number - 1].hand[move_c.card_number].value) >= game_info.explosion_t) { // maximizes the explosion
			move_c = calc_exp_max(players_arr[game_info.player_number - 1], piles_arr); 
		}
		move_card(&players_arr[game_info.player_number - 1], &piles_arr[move_c.pile], move_c.card_number);

	}

}



void moveA(player* players_arr, pile* piles_arr) { //moves first card from player's hand to the first suitable pile 
	int player_number = game_info.player_number;
	if (players_arr[game_info.player_number - 1].cards_on_hand > 0) { // doesnt move if hand is empty
		color c = players_arr[game_info.player_number - 1].hand[0].c_color; 
		int p_n = 0; //pile number
		if (c != green) {
			for (int i = game_info.color_count; i > 0; i--) { //search for first empty pile 
				if (piles_arr[i].pile_color == green) p_n = i;
			}
			int p = check_pile_color(piles_arr, c);
			if (p > 0) p_n = p-1;
		}

		move_card(&players_arr[player_number - 1], &piles_arr[p_n], 0);
	}

}



//--------------------------------------------------functions for single game-----------------------------------------------------------------------------------------------------
//----------------------------------------------------------gen------------------------------------------------------------------------------------------------------------------

void shuffle(card* cards_arr) {
	int n = (game_info.color_count * game_info.others_number) + game_info.green_count; // total number of cards in game
	srand(time(NULL));
	card buff;// needed to save one of the values while swapping
	for (int i = 0; i < game_info.others_number * SHUFFLE; i++) { //swaps random cards 
		int x = rand() % n;
		buff = cards_arr[x];
		cards_arr[x] = cards_arr[n - x - 1];
		cards_arr[n - x - 1] = buff;
	}
}

player* deal(card* cards_arr) {
	player* players_arr = (player*)malloc(game_info.players_number * sizeof(player)); //allocate all the players in array
	if (players_arr == NULL) {
		printf("error");
		exit(0);
	}
	for (int i = 0; i < game_info.players_number; i++) { //allocate player's hands and decks
		players_arr[i].hand = (card*)malloc(sizeof(card));
		if (players_arr[i].hand == NULL) {
			printf("error");
			exit(0);
		}
		players_arr[i].deck = (card*)malloc(sizeof(card));
		if (players_arr[i].deck == NULL) {
			printf("error");
			exit(0);
		}
		players_arr[i].cards_on_deck = 0; 
		players_arr[i].cards_on_hand = 0;

	}
	int n = (game_info.color_count * game_info.others_number) + game_info.green_count; //deals all the cards from shuffled deck to players evenly 
	for (int i = 0; i < n; i++) {
		players_arr[i % game_info.players_number].hand[players_arr[i % game_info.players_number].cards_on_hand] = cards_arr[i];
		players_arr[i % game_info.players_number].cards_on_hand++;
		players_arr[i % game_info.players_number].hand = (card*)realloc(players_arr[i % game_info.players_number].hand, (players_arr[i % game_info.players_number].cards_on_hand + 1) * sizeof(card)); //expands array
		if (players_arr[i % game_info.players_number].hand == NULL) {
			printf("error");
			exit(0);
		}
	}

	return players_arr;
}

pile* gen_piles() { //allocates all the piles, sets their count to 0 and their color to green what means its defaultly without color 
	pile* piles_arr = (pile*)malloc(game_info.color_count * sizeof(pile)); 
	if (piles_arr == NULL) {
		printf("error");
		exit(0);
	}
	for (int i = 0; i < game_info.color_count; i++) {
		piles_arr[i].cards = (card*)malloc(sizeof(card));
		if (piles_arr[i].cards == NULL) {
			printf("error");
			exit(0);
		}
		piles_arr[i].count = 0;
		piles_arr[i].pile_color = green;
	}
	return piles_arr;
}

card* scan_info() { // scan all the const numbers characterizing game from stdin and save them into global struct 
	int ov;
	if (!scanf("%d %d %d %d %d %d", &game_info.players_number, &game_info.color_count, &game_info.green_count, &game_info.green_value, &game_info.others_number, &game_info.explosion_t)) {
		printf("error, wrong input data");
		exit(0);
	}
	if (game_info.color_count > MAX_COLORS_NUMBER)printf("Color number must be lover than %d", MAX_COLORS_NUMBER);
	card* cards_arr = (card*)malloc(((game_info.color_count * game_info.others_number) + game_info.green_count) * sizeof(card));
	if (cards_arr == NULL) {
		printf("error");
		exit(0);
	}//fills allocated cards array with unshuffled cards
	for (int i = 0; i < game_info.others_number; i++) {
		if (!scanf("%d", &ov)) {
			printf("error, wrong input data");
			exit(0);
		}
		for (int j = 0; j < game_info.color_count; j++) {
			cards_arr[j + (game_info.color_count * i)].value = ov;
			cards_arr[j + (game_info.color_count * i)].c_color = (color)j;
		}
	}
	game_info.player_number = 1;
	for (int i = 0; i < game_info.green_count; i++) { //fills array with green cards
		cards_arr[(game_info.others_number * game_info.color_count) + i].value = game_info.green_value;
		cards_arr[(game_info.others_number * game_info.color_count) + i].c_color = green;
	}
	return cards_arr;
}

//--------------------------------------------------------------------load-------------------------------------------------------------------------------------------------
cards_temp* scan(int n, FILE* file) { //n is from which string it should start reading
	char buf[MAX_BUF_LENGHT];
	char* p = NULL;
	int count = 0;
	int pos;
	pos = ftell(file);
	if (fgetc(file) == EOF) return NULL;
	fseek(file, pos, SEEK_SET);

	cards_temp* cards_arr = (cards_temp*)malloc(sizeof(cards_temp));
	cards_arr->cards = (card*)malloc(sizeof(card));
	fgets(buf, MAX_BUF_LENGHT, file);//gets line
	p = strtok(buf, " ");//split line into tokens 
	for (int k = 0; k < n; k++) p = strtok(NULL, " "); //sets position to start reading 
	while ((p != NULL) && (strcmp("\n", p) != 0)) { 
		cards_arr->cards[count].value = atoi(p);//convert string red from input into int and save it 
		p = strtok(NULL, " ");
		cards_arr->cards[count].c_color = color(convert(p));//convert string into int meaning the color 

		if (cards_arr->cards[count].c_color == green) { //this part saves green values and checks them if all of them are correct
			if (game_info.green_value == 0)
				game_info.green_value = cards_arr->cards[count].value; //g
			else if (cards_arr->cards[count].value != game_info.green_value) {
				printf("Different green cards values occurred");
				exit(0);
			}
		}

		p = strtok(NULL, " ");
		count++;
		cards_arr->cards = (card*)realloc(cards_arr->cards, (count + 1) * sizeof(card)); //expand array size 
		if (cards_arr->cards == NULL) {
			printf("error");
			exit(0);
		}
	}
	cards_arr->count = count;
	memset(buf, 0, MAX_BUF_LENGHT); //reset buffer
	return cards_arr;
}


player* scan_players(FILE* file) {
	if (!fscanf(file, "%*s %*s = %d\n", &game_info.player_number))
		printf("Input data incorrect");
	if (!fscanf(file, "%*s %*s = %d\n", &game_info.players_number))
		printf("Input data incorrect");;
	if (!fscanf(file, "%*s %*s = %d\n", &game_info.explosion_t))
		printf("Input data incorrect");

	player* players_arr = (player*)malloc(game_info.players_number * sizeof(player));
	if (players_arr == NULL) {
		printf("error");
		exit(0);
	}

	for (int i = 0; i < game_info.players_number; i++) {
		players_arr[i].hand = (card*)malloc(sizeof(card)); //allocate first elements of hand and deck arrays
		if (players_arr[i].hand == NULL) {
			printf("error");
			exit(0);
		}
		players_arr[i].deck = (card*)malloc(sizeof(card));
		if (players_arr[i].deck == NULL) {
			printf("error");
			exit(0);
		}
		cards_temp* temp = scan(4, file); //gets first line and saves data from it in hand , number 4 means that first 4 tokens are discarded 
		players_arr[i].hand = temp->cards;
		players_arr[i].cards_on_hand = temp->count;
		temp = scan(4, file); //gets another line 
		players_arr[i].deck = temp->cards;
		players_arr[i].cards_on_deck = temp->count;
	}
	return players_arr;
}

pile* scan_piles(FILE* file) { //scan piles just like players
	pile* piles_arr = (pile*)malloc(MAX_COLORS_NUMBER * sizeof(pile)); //allocate all the piles
	if (piles_arr == NULL) {
		printf("error");
		exit(0);
	}
	int i = 0;
	while (1) {//gets all lines until scan() returns NULL
		cards_temp* temp = scan(3, file);
		if (temp == NULL) break;
		piles_arr[i].cards = temp->cards;
		piles_arr[i].count = temp->count;
		piles_arr[i].pile_color = green;
		i++;
	}
	game_info.color_count = i;
	return piles_arr;
}

//---------------------------------------------------game loop-----------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------check corrctness----------------------------------------------------------------------------------------------------------------

void loop_through(int* count, card* cards_arr, int cards_count, int** card_values_arr) { //from where to read, how many cards,where to save
	int color; //function loops through array of cards and saves it into one array of all cards
	for (int j = 0; j < cards_count; j++) {
		if (cards_arr[j].c_color < MAX_COLORS_NUMBER) {//assures that it doesnt count green cards
			color = int(cards_arr[j].c_color); //keeping every color in separate line of 2d array
			card_values_arr[color][count[color]] = cards_arr[j].value;
			count[color]++;
			card_values_arr[color] = (int*)realloc(card_values_arr[color], (count[color] + 1) * sizeof(int)); //expanding array
			if (card_values_arr[color] == NULL) {
				printf("error");
				exit(0);
			}
		}
	}
}

void check_pile(int* explode, int* color, pile pile) { //checks if pile has correct color and if it should have exploded earlier int color is flag raised if colors dont match
	int sum = 0;
	for (int j = 0; j < pile.count; j++) { //loop through all the cards in pile
		sum += pile.cards[j].value;
		if (pile.cards[j].c_color < 6) {
			if (pile.pile_color == green) pile.pile_color = pile.cards[j].c_color;
			else {
				if (pile.cards[j].c_color != pile.pile_color) color++;
			}
		}
	}
	if (sum > game_info.explosion_t) *explode = 1;
}

void check_hands(player* players_arr, int* incorrect_hand) { //checks if any player doesnt have more cards on hand than any other
	for (int i = 0; i < game_info.players_number; i++) {
		if (players_arr[0].cards_on_hand > players_arr[i].cards_on_hand + 1 || players_arr[0].cards_on_hand < players_arr[i].cards_on_hand - 1) incorrect_hand++; // compares to hand of 1st player
	}

}

void check_values(int** card_values_arr, int* incorrect_values, int* count) { //loops through all cards and checks if all colors have the same values
	for (int i = 0; i < game_info.color_count; i++) {
		for (int j = 0; j < count[i]; j++) {
			if (card_values_arr[0][j] != card_values_arr[i][j]) (*incorrect_values)++;  //raises flag if card values arent matching compares to values of 1st (blue) color
		}
	}
}

void check_colors(int* count, int* c) {//checks if there is equal number of cards of every color  int c is flag
	for (int i = 0; i < game_info.color_count; i++) {
		if (count[i] != count[0]) (*c)++; 
	}
}

void print_errors(int* explode, int* color, int* values, int* hand, int* number, int** card_values_arr, int* count) { //explode color ... etc are flags and arrays got from check_arrors function
	char cname[7][7] = { "blue","red","violet","yellow","white","black","green" }; // this array allows to convert color into string that is suitable to be written to stdout
	int ind = 0;
	for (int i = 0; i < game_info.color_count; i++) {
		if (explode[i] > 0) {
			ind++;
			printf("Pile number %d should explode earlier\n", i + 1);
		}
		if (color[i] > 0) {
			ind++;
			printf("Two different colors were found on the %d pile\n", i + 1);
		}
	}
	if (*values > 0) {
		ind++;
		printf("The values of cards of all colors are not identical:\n");
		for (int i = 0; i < game_info.color_count; i++) {
			printf("%s cards values:", cname[i]);
			for (int j = 0; j < count[i]; j++) printf(" %d", card_values_arr[i][j]);
			printf("\n");
		}

	}
	if (*hand > 0) {
		ind++;
		printf("The number of players cards on hand is wrong\n");
	}
	if (*number > 0) {
		ind++;
		printf("At least two colors with a different number of cards were found \n");
		for (int i = 0; i < game_info.color_count; i++) {
			printf("%s cards are %d\n", cname[i], count[i]);
		}
	}
	if (game_info.green_value == 0) {
		ind++;
		printf("\nGreen cards does not exist");
	}
	
	if (game_info.players_number == 0) {
		ind++;
		printf("\nerror, players number = 0");
		exit(1);
	}

	if (ind == 0) printf("Current state of the game is ok\n");
}


void check_corectness(player* players_arr, pile* piles_arr) {
	int count[6] = { 0 }; //array which determines how many card of given color appeared count[0]-blue etc
	int explode[MAX_COLORS_NUMBER] = { 0 };
	int incorrect_color[MAX_COLORS_NUMBER] = { 0 };
	int incorrect_values = 0;
	int incorrect_hand = 0;
	int incorrect_cards_number = 0;
	int* card_values_arr[6];
	for (int i = 0; i < 6; i++) { //allocate 2d array of cards values where each line is assigned to one color  
		card_values_arr[i] = (int*)malloc(sizeof(int));
	}
	for (int i = 0; i < game_info.players_number; i++) { // loop going through all players' cards and saving their values in card_values array
		loop_through(count, players_arr[i].hand, players_arr[i].cards_on_hand, card_values_arr);
		loop_through(count, players_arr[i].deck, players_arr[i].cards_on_deck, card_values_arr);
	}
	for (int i = 0; i < game_info.color_count; i++) { //reading cards' values from piles
		loop_through(count, piles_arr[i].cards, piles_arr[i].count, card_values_arr);
		check_pile(&explode[i], &incorrect_color[i], piles_arr[i]);

	}

	for (int i = 0; i < game_info.color_count; i++) { //sorting every color's values
		if (card_values_arr[i] == 0) {
			printf("error");
			exit(0);
		}
		qsort(card_values_arr[i], count[i], sizeof(int), compare);
	}
	check_colors(count, &incorrect_cards_number);
	check_values(card_values_arr, &incorrect_values, count);
	check_hands(players_arr, &incorrect_hand);
	print_errors(explode, incorrect_color, &incorrect_values, &incorrect_hand, &incorrect_cards_number, card_values_arr, count);

}

//----------------------------------------------------print current----------------------------------------------------------------------------------------------------------------

void print_current(player* players_arr, pile* piles_arr, FILE* file) { //prints current state of game 
	char cname[7][7] = { "blue","red","violet","yellow","white","black","green" };  // this array allows to convert color into string that is suitable to be written to stdout
	fprintf(file, "\nactive player = %d\n", game_info.player_number);
	fprintf(file, "players number = %d\n", game_info.players_number);
	fprintf(file, "explosion threshold = %d\n", game_info.explosion_t);
	for (int i = 0; i < game_info.players_number; i++) {
		fprintf(file, "%d player hand cards:", i + 1);
		for (int j = 0; j < players_arr[i].cards_on_hand; j++) {
			fprintf(file, " %d %s", players_arr[i].hand[j].value, cname[color(players_arr[i].hand[j].c_color)]);
		}
		fprintf(file, "\n");
		fprintf(file, "%d player dec cards:", i + 1);
		for (int j = 0; j < players_arr[i].cards_on_deck; j++) {
			fprintf(file, " %d %s", players_arr[i].deck[j].value, cname[color(players_arr[i].deck[j].c_color)]);
		}
		fprintf(file, "\n");
	}
	for (int i = 0; i < game_info.color_count; i++) {
		fprintf(file, "%d pile cards: ", i + 1);
		for (int j = 0; j < piles_arr[i].count; j++) {
			fprintf(file, " %d %s", piles_arr[i].cards[j].value, cname[color(piles_arr[i].cards[j].c_color)]);
		}
		fprintf(file, "\n");
	}
}

//---------------------------------------------------move and check if over-------------------------------------------------------------------------------------------------------

void move(player* players_arr, pile* piles_arr) { //chooses which move to make 
	if (game_info.player_number == 1) {
		moveC(players_arr, piles_arr);
	}
	else if (game_info.player_number == 2) {
		moveB(players_arr, piles_arr);
	}
	else {
		moveA(players_arr, piles_arr);
	}
}


void check_if_over(player* players_arr) { //if all the players hands are empty raise flag to end the game
	int flag = 0;
	for (int i = 0; i < game_info.players_number; i++) {
		if (players_arr[i].cards_on_hand == 0) flag++;
	}
	if (flag == game_info.players_number) game_info.end = false;
}

//-------------------------------------------end game-------------------------------------------------------------------------------------------------------------------

int** count_colors(player* players_arr) {
	int** imm = (int**)malloc(MAX_COLORS_NUMBER * sizeof(int*));
	if (imm == NULL) {
		printf("error");
		exit(0);
	}
	for (int i = 0; i < MAX_COLORS_NUMBER; i++) { //allocates array imm where each row means 1 color and each value is each player's number of given color  
		imm[i] = (int*)malloc(game_info.players_number * sizeof(int));
		if (imm[i] == NULL) {
			printf("error");
			exit(0);
		}
		for (int j = 0; j < game_info.players_number; j++) {// sets every number to 0
			imm[i][j] = 0;
		}
	}

	for (int i = 0; i < game_info.players_number; i++) { //counts colors for every player's deck
		for (int j = 0; j < players_arr[i].cards_on_deck; j++) {
			if (players_arr[i].deck[j].c_color != green)
			imm[int(players_arr[i].deck[j].c_color)][i]++;
		}
	}
	return imm;
}


int* end_game(player* players_arr) { //returns array where each slot means 1 color and its value means player immune to it 
	int** imm = count_colors(players_arr);
	int* highest = (int*)malloc(6 * sizeof(int));
	int flag = 0;
	for (int j = 0; j < game_info.color_count; j++) {
		highest[j] = 0;
		int max = 0;
		for (int i = 0; i < game_info.players_number; i++) { //seeks the highest
			if (imm[j][i] > max) {
				highest[j] = i;
				max = imm[j][i];
			}
		}
		for (int i = 0; i < game_info.players_number; i++)
		{
			if (imm[j][highest[j]] == imm[j][i]) flag++; //flag marks if 2 places with the same highest value appear
		}
		if (flag > 1) highest[j] = -1; // it means there is no player immune to this color 
		flag = 0;
	}
	return highest;

}



int* count_score(player* players_tab, int* imm) { //counts score of every player 
	int* sum = (int*)malloc(game_info.players_number * sizeof(int));
	if (sum == NULL) {
		printf("error");
		exit(0);
	}
	for (int i = 0; i < game_info.players_number; i++) {
		sum[i] = 0;
		for (int j = 0; j < players_tab[i].cards_on_deck; j++) {
			if (players_tab[i].deck[j].c_color != green)
			{
				if (imm[color(players_tab[i].deck[j].c_color)] != i) { //if player is not immune and if color isnt green counts +1 score
					sum[i]++;
				}
			}
			else //for greens counts x2 score
			{
				sum[i] += 2;
			}

		}
	}
	return sum; //returns array of scores
}


void print_results(int* imm, int* score) { 
	char cname[7][7] = { "blue","red","violet","yellow","white","black","green" };
	printf("\n\n");
	for (int i = 0; i < game_info.color_count; i++) {
		if (imm[i] > -1) printf("Na kolor %s odporny jest gracz %d\n", cname[i], imm[i] + 1);
	}
	for (int i = 0; i < game_info.players_number; i++) {
		printf("Wynik gracza %d  = %d\n", i + 1, score[i]);
	}
	printf("\n\n");
}

void end_single(player* players_arr) {
	int* immunity_arr = (int*)malloc(MAX_COLORS_NUMBER * sizeof(int)); //array indicating which player is immune to which color
	immunity_arr = end_game(players_arr);
	int* score = count_score(players_arr, immunity_arr);
	print_results(immunity_arr, score);
}

//---------------------------------------------------functions for serie of games----------------------------------------------------------------------------------------------------

int count_v(int* winners_arr, int p, int size) { //count victories of given player p 
	int count = 0;
	for (int i = 0; i < size; i++) {
		if (winners_arr[i] == p) count++;
	}
	return count;
}

void end_series(int count, player* players_arr, int* winners_arr) {//does the same as normal game ending except it also counts how many games each player won
	int* immunity_arr = (int*)malloc(MAX_COLORS_NUMBER * sizeof(int));
	immunity_arr = end_game(players_arr);
	int* score = count_score(players_arr, immunity_arr);
	//print_results(immunity_arr, score);

	int winner = 1000; //some high number, so score would be lower than it
	for (int i = 0; i < game_info.players_number; i++) {
		if (score[i] < winner) winner = i;
		else if (score[i] == score[winner]) winner = -2; // it means more than 1 players have the same highest score and this round resulted in draw
	}
	winners_arr[count] = winner;
}



//-----------------------------------------------------main functions for single game----------------------------------------------------------------------------------------------------
void load(player** players_arr, pile** piles_arr) { //operates on original values of players_arr and piles_arr, just like all the functions here
	FILE* file; 
	file = fopen("game.txt", "r");
	*players_arr = scan_players(file); //scan data from piles
	*piles_arr = scan_piles(file);
	fclose(file);
}

void gen(player** players_arr, pile** piles_arr) {
	FILE* file;
	file = fopen("game.txt", "w");
	if (file == NULL)exit(0);
	card* cards_arr = scan_info();
	shuffle(cards_arr);
	*players_arr = deal(cards_arr);
	free(cards_arr);
	*piles_arr = gen_piles();
	print_current(*players_arr, *piles_arr, file);
	fclose(file);
}

void game_loop(player** players_arr, pile** piles_arr) {
	check_corectness(*players_arr, *piles_arr); // at beggining of game check correctness of game state 
	print_current(*players_arr, *piles_arr, stdout);// print initial state 
	while (game_info.end) { //loop until game ends
		move(*players_arr, *piles_arr);
		FILE* file;
		file = fopen("game.txt", "w");
		if (file == NULL)
			exit(1);//save current state to file 
		print_current(*players_arr, *piles_arr, file);
		//print_current(*players_arr, *piles_arr, stdout);
		//printf("\n");
		fclose(file);
		check_if_over(*players_arr); 
	}
	end_single(*players_arr);
	//print_current(*players_arr, *piles_arr, stdout);
}
//---------------------------------------------main functions for serie of games---------------------------------------------------------------------------------------------------------

void series_game_loop(player** players_arr, pile** piles_arr,int** winners_arr, card* cards_arr,int r) { //r is how many rounds
	for (int i = 0; i < r; i++) {
		shuffle(cards_arr);
		*players_arr = deal(cards_arr);
		*piles_arr = gen_piles();
		//check_corectness(players_arr, piles_arr);
		//print_current(players_arr, piles_arr, stdout);
		while (game_info.end) {
			move(*players_arr, *piles_arr);
			check_if_over(*players_arr);
		}
		end_series(i, *players_arr, *winners_arr);
		//print_current(players_arr, piles_arr, stdout);
		game_info.end = true;
	}
}


void print_series_result(int* winners_arr, int r) {
	/*for (int i = 0; i < r; i++) {
	if (winners_arr[i] == -2) printf("draw\n");
	else
	printf("Winner of %d game is player number %d\n", i + 1, winners_arr[i] + 1);
}*/

	for (int i = 0; i < game_info.players_number; i++) {
		printf("player %d won %d times\n", i + 1, count_v(winners_arr, i, r));
	}
}
//-----------------------------------------------------------------main--------------------------------------------------------------------


int main() {
	player* players_arr = NULL;
	pile* piles_arr = NULL;

	char c;

	if (!scanf("%c", &c)) {
		printf("error, wrong input data");
		exit(0);
	}
	if (c == 'L') {
		load(&players_arr, &piles_arr);
		game_loop(&players_arr, &piles_arr);
	}
	else if (c == 'G') {
		gen(&players_arr, &piles_arr);
		game_loop(&players_arr, &piles_arr);
	}
	else if (c == 'S') {
		int r;
		if (!scanf("%d", &r)) { //scan how many rounds should be played
			printf("error, wrong input data");
			exit(0);
		}
		int* winners_arr = (int*)malloc(r * sizeof(int)); //allocate array containing winners of each round 
		if (winners_arr == NULL) {
			printf("error");
			exit(0);
		}
		card* cards_arr = scan_info(); // scan what values should be used in the game 
		series_game_loop(&players_arr, &piles_arr, &winners_arr, cards_arr, r);
		print_series_result(winners_arr, r);
	}
	else {
		printf("Please choose either L to load or G to generate or S");
		exit(0);
	}

	free(piles_arr);
	free(players_arr);
	return 0;
}