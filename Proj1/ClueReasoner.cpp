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
	for (int card = 0; card < num_cards; card++)
	{
		// One clause for all places because the card is here OR there OR etc.
		Clause clause;
		for (int place = 0; place <= num_players; place++)
			clause.push_back(GetPairNum(place, card));
		solver->AddClause(clause);

		// If a card is in one place, it cannot be in another place.
		// TO BE IMPLEMENTED AS AN EXERCISE

		// For each card, enter the possibility of where teh card is and what that means
		for (int place = 0; place <= num_players - 1; place++)
		{
			for (int j = place + 1; j <= num_players; j++)
			{
				// Separate clause for each possibility because it is a unique clause to consider
				Clause clause;

				// The card is either in not in place or not in j
				clause.push_back(-GetPairNum(place, card));
				clause.push_back(-GetPairNum(j, card));

				solver->AddClause(clause);
			}
		}
	}

	// At least one card of each category is in the case file.
	// TO BE IMPLEMENTED AS AN EXERCISE

	// At least one player in the case file
	Clause playerClause;
	for (int outerPlayer = 0; outerPlayer < num_players; outerPlayer++)
	{
		Clause clause;
		for (int innerPlayer = 0; innerPlayer < num_players; innerPlayer++)
		{
			// only one can be in case file
			if (outerPlayer == innerPlayer)
			{
				clause.push_back(GetPairNum(case_file, players[innerPlayer]));
			}
			else
			{
				clause.push_back(-GetPairNum(case_file, players[outerPlayer]));
			}
		}
		solver->AddClause(clause);
	
		// At least one is in case file
		playerClause.push_back(GetPairNum(case_file, players[outerPlayer]));
	}
	solver->AddClause(playerClause);
	
	// At least one weapon in the case file
	// Unique weapon
	Clause weaponClause;
	for (int outerWeapon = 0; outerWeapon < num_weapons; outerWeapon++)
	{
		Clause clause;
		for (int innerWeapon = 0; innerWeapon < num_weapons; innerWeapon++)
		{
			if (outerWeapon == innerWeapon)
			{
				clause.push_back(GetPairNum(case_file, weapons[innerWeapon]));
			}
			else
			{
				clause.push_back(-GetPairNum(case_file, weapons[innerWeapon]));
			}
		}
		solver->AddClause(clause);

		// At least one weapon can be in the case file
		weaponClause.push_back(GetPairNum(case_file, weapons[outerWeapon]));
	}
	solver->AddClause(weaponClause);

	// At least one room in the case file
	Clause roomClause;
	for (int outerRoom = 0; outerRoom < num_rooms; outerRoom++)
	{
		Clause clause;
		for (int innerRoom = 0; innerRoom < num_rooms; innerRoom++)
		{
			if (outerRoom == innerRoom)
			{
				clause.push_back(GetPairNum(case_file, rooms[innerRoom]));
			}
			else
			{
				clause.push_back(-GetPairNum(case_file, rooms[innerRoom]));
			}
		}
		solver->AddClause(clause);

		// At least one room can be in the case file
		roomClause.push_back(GetPairNum(case_file, rooms[outerRoom]));
	}
	solver->AddClause(roomClause);
}

void ClueReasoner::Hand(string player, string cards[3])
{
	// GetPlayerNum returns the index of the player in the players array (not the suspects array). Remember that the
	// players array is sorted wrt the order that the players play. Also note that, player_num (not to be confused with num_players) 
	// is a private variable of the ClueReasoner class that is initialized when this function is called.
	player_num = GetPlayerNum(player);
	
	// TO BE IMPLEMENTED AS AN EXERCISE

	// Record our player has the cards FOR SURE
	for(int c = 0; c < 3; c++)
	{
		Clause hasCard;
		hasCard.push_back(GetPairNum(player, cards[c]));
		solver->AddClause(hasCard);
	}
}

void ClueReasoner::Suggest(string suggester, string card1, string card2, string card3, string refuter, string card_shown)
{
	// Note that in the Java implementation, the refuter and the card_shown can be NULL. 
	// In this C++ implementation, NULL is translated to be the empty string "".
	// To check if refuter is NULL or card_shown is NULL, you should use if(refuter == "") or if(card_shown == ""), respectively.
	
	// TO BE IMPLEMENTED AS AN EXERCISE

	// No player (excluding suggester) is able to show a card
	int sugNum = GetPlayerNum(suggester);
	if (refuter == "" && card_shown == "")
	{
		// For every player, write a clause that they do not have any of the three cards
		for (int p = 0; p < num_players; p++)
		{
			if (p != sugNum)
			{
				// Each is its own clause because we know FOR SURE that these other players do not have these
				Clause noCard1, noCard2, noCard3;
				noCard1.push_back(-GetPairNum(players[p], card1));
				noCard2.push_back(-GetPairNum(players[p], card2));
				noCard3.push_back(-GetPairNum(players[p], card3));
				solver->AddClause(noCard1);
				solver->AddClause(noCard2);
				solver->AddClause(noCard3);
			}
		}
	}
	else if (refuter != "")
	{
		// if SOMEONE has refuted, we have to say that all the players between the refuter and suggester have non eof the three cards
		// Say all players between current player and refuter don't have the card shown
		// Need to iterate in order but have to wrap around to smaller number potentially (if we cross from last to first)
		int refNum = GetPlayerNum(refuter);
		int iterator = (sugNum + 1) % num_players;
		while(iterator != sugNum && iterator != refNum)
		{
				Clause noCard1, noCard2, noCard3;
				noCard1.push_back(-GetPairNum(players[iterator], card1));
				noCard2.push_back(-GetPairNum(players[iterator], card2));
				noCard3.push_back(-GetPairNum(players[iterator], card3));
				solver->AddClause(noCard1);
				solver->AddClause(noCard2);
				solver->AddClause(noCard3);
				iterator++;
				iterator %= num_players;
		}

		if(card_shown != "")
		{
			// Some player shows some card to ME to refute the suggestion (I know the card and the player)
			// Add that the refuter has card_shown FOR SURE
			// Our initial clauses will know that a card can only be in one spot
			Clause hasCard;
			hasCard.push_back(GetPairNum(refuter, card_shown));
			solver->AddClause(hasCard);
		}
		else if (card_shown == "")
		{
			// I know the refuter but I don't know the card shown
			// Add clause that refuter has 1 OR 2 OR 3 
			// Don't need to add clauses that 1 or 2 or 3 is not in case file because we added that if a card is in one spot it is not in another
			Clause refHas;
			refHas.push_back(GetPairNum(refuter, card1));
			refHas.push_back(GetPairNum(refuter, card2));
			refHas.push_back(GetPairNum(refuter, card3));		
			solver->AddClause(refHas);
		}
	}
}


void ClueReasoner::Accuse(string suggester, string card1, string card2, string card3, bool is_correct)
{
	// TO BE IMPLEMENTED AS AN EXERCISE
}
