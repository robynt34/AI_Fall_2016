#include "ClueReasoner.h"
using namespace std;

int ClueReasoner::GetPlayerNum(string player)
{
	if (player == case_file)
		return num_players;
	
	for (int i = 0; i < num_players; i++)
		if (player == players[i])
			return i;
			
	cout<<"Illegal player: "<<player<<endl;
	return -1;
}
int ClueReasoner::GetCardNum(string card)
{
	for (int i = 0; i < num_cards; i++)
		if (card == cards[i])
			return i;
			
	cout<<"Illegal card: "<<card<<endl;
	return -1;
}

string ClueReasoner::QueryString(int return_code)
{
	if (return_code == kFalse)
		return "n";
	else if (return_code == kTrue)
		return "Y";
	else
		return "-";
}

void ClueReasoner::PrintNotepad()
{
	for (int i = 0; i < num_players; i++)
		cout<<"\t"<<players[i];
	cout<<"\t"<<case_file<<endl;
	
	for (int i = 0; i < num_cards; i++)
	{
		cout<<cards[i]<<"\t";
		for (int j = 0; j < num_players; j++)
			cout<<QueryString(Query(players[j], cards[i]))<<"\t";
		
		cout<<QueryString(Query(case_file, cards[i]))<<endl;
	}
}
	
void ClueReasoner::AddInitialClauses()
{
	/* The following code is given as an example to show how to create Clauses and post them to the solver. 
		SatSolver.h uses the following typedefs:
		typedef int Literal;
		typedef std::vector<Literal> Clause;
		
	That is, a Literal is defined as a positive or a negative integer
	(meaning that it is the negated form, as in -p or -q), and
	a Clause is defined as a vector of Literals (integers).
	
	The function GetPairNum(p, c) returns the literal that means that player p has card c 
	(note that p can be case_file as well). 
	See ClueReasoner.h, lines 7-29 for a definition of the arrays and
	variables that you can use in your implementation. 
	*/

	// Each card is in at least one place (including case file).
	for (int card = 0; card < num_cards; card++)	// Iterate over all cards.
	{
		Clause clause;
		for (int place = 0; place <= num_players; place++)
			clause.push_back(GetPairNum(place, card));
		
		solver->AddClause(clause);
	}
    
	// If a card is in one place, it cannot be in another place.
	// TO BE IMPLEMENTED AS AN EXERCISE
	
	// At least one card of each category is in the case file.
	// TO BE IMPLEMENTED AS AN EXERCISE

	
}

void ClueReasoner::Hand(string player, string cards[3])
{
	// GetPlayerNum returns the index of the player in the players array (not the suspects array). Remember that the
	// players array is sorted wrt the order that the players play. Also note that, player_num (not to be confused with num_players) 
	// is a private variable of the ClueReasoner class that is initialized when this function is called.
	player_num = GetPlayerNum(player);
	
	// TO BE IMPLEMENTED AS AN EXERCISE
}

void ClueReasoner::Suggest(string suggester, string card1, string card2, string card3, string refuter, string card_shown)
{
// TO BE IMPLEMENTED AS AN EXERCISE

}


void ClueReasoner::Accuse(string suggester, string card1, string card2, string card3, bool is_correct)
{
	// TO BE IMPLEMENTED AS AN EXERCISE
}
