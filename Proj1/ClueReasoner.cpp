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
				// Separate clause for each possibility of where teh card is
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
	
		// Only one player can be in the case file
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

		// Only one weapon can be in the case file
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

		// Only one room can be in the case file
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

	// Record our player has the cards
	for(int c = 0; c < 3; c++)
	{
		Clause hasCard;
		hasCard.push_back(GetPairNum(player, cards[c]));
		solver->AddClause(hasCard);
	}
}

void ClueReasoner::Suggest(string suggester, string card1, string card2, string card3, string refuter, string card_shown)
{
#if 1
	// Note that in the Java implementation, the refuter and the card_shown can be NULL. 
	// In this C++ implementation, NULL is translated to be the empty string "".
	// To check if refuter is NULL or card_shown is NULL, you should use if(refuter == "") or if(card_shown == ""), respectively.
	
	// TODO: Should the suggester be included in the first 2 cases?
// TO BE IMPLEMENTED AS AN EXERCISE
	// Case refuter and card shown are not empty (I make a suggestion that someone refutes)
	if (refuter != "" && card_shown != "")
	{
		// Add that the refuter has card_shown
		// Add that the case file does not have the card shown
		Clause refuterHasCardShown, notInCaseFile;
		refuterHasCardShown.push_back(GetPairNum(refuter, card_shown));
		notInCaseFile.push_back(-GetPairNum(case_file, card_shown));
		solver->AddClause(refuterHasCardShown);
		solver->AddClause(notInCaseFile);
		// Add that all players between current player and refuter don't have the card shown
		int refuterNum = GetPlayerNum(refuter);
		int suggesterNum = GetPlayerNum(suggester);
		for (int p = 0; p < num_players; p++)
		{
			if (p != refuterNum && p != player_num)
			{
				Clause playerDoesNotHaveCardShown;
				playerDoesNotHaveCardShown.push_back(-GetPairNum(players[p], card_shown));
				solver->AddClause(playerDoesNotHaveCardShown);
			}
		}

		for (int p = (suggesterNum + 1) % num_players;
		p != refuterNum && p != suggesterNum;
		p =  (p + 1) % num_players)
		{
			if (p != player_num)
			{
				Clause playerDoesNotHaveCard1, playerDoesNotHaveCard2, playerDoesNotHaveCard3;
				playerDoesNotHaveCard1.push_back(-GetPairNum(players[p], card1));
				playerDoesNotHaveCard2.push_back(-GetPairNum(players[p], card2));
				playerDoesNotHaveCard3.push_back(-GetPairNum(players[p], card3));
				solver->AddClause(playerDoesNotHaveCard1);
				solver->AddClause(playerDoesNotHaveCard2);
				solver->AddClause(playerDoesNotHaveCard3);
			}
		}
	}
	// Case refuter is valid, card shown is empty (Someone else makes a suggestion that someone else refutes).
	else if (refuter != "" && card_shown == "")
	{
		// Add that the refuter has one of these cards.
		Clause refuterHasACard, aCardNotInCaseFile;
		refuterHasACard.push_back(GetPairNum(refuter, card1));
		refuterHasACard.push_back(GetPairNum(refuter, card2));
		refuterHasACard.push_back(GetPairNum(refuter, card3));
		aCardNotInCaseFile.push_back(-GetPairNum(case_file, card1));
		aCardNotInCaseFile.push_back(-GetPairNum(case_file, card2));
		aCardNotInCaseFile.push_back(-GetPairNum(case_file, card3));
		solver->AddClause(refuterHasACard);
		solver->AddClause(aCardNotInCaseFile);
		// Add that players between suggester and refuter don't have card shown.
		int refuterNum = GetPlayerNum(refuter);
		int suggesterNum = GetPlayerNum(suggester);
		for (int p = (suggesterNum + 1) % num_players;
		p != refuterNum && p != suggesterNum;
		p =  (p + 1) % num_players)
		{
			if (p != player_num)
			{
				Clause playerDoesNotHaveCard1, playerDoesNotHaveCard2, playerDoesNotHaveCard3;
				playerDoesNotHaveCard1.push_back(-GetPairNum(players[p], card1));
				playerDoesNotHaveCard2.push_back(-GetPairNum(players[p], card2));
				playerDoesNotHaveCard3.push_back(-GetPairNum(players[p], card3));
				solver->AddClause(playerDoesNotHaveCard1);
				solver->AddClause(playerDoesNotHaveCard2);
				solver->AddClause(playerDoesNotHaveCard3);
			}
		}
	}
	// Case refuter and card shown are both empty (Nobody was able to refute the suggestion)
	else if (refuter == "" && card_shown == "")
	{
		// Add that all players except the suggester don't have any of the suggested cards.
		int suggesterNum = GetPlayerNum(suggester);
		for (int p = 0; p < num_players; p++)
		{
			if (p != suggesterNum)
			{
				Clause playerDoesNotHaveCard1, playerDoesNotHaveCard2, playerDoesNotHaveCard3;
				playerDoesNotHaveCard1.push_back(-GetPairNum(players[p], card1));
				playerDoesNotHaveCard2.push_back(-GetPairNum(players[p], card2));
				playerDoesNotHaveCard3.push_back(-GetPairNum(players[p], card3));
				solver->AddClause(playerDoesNotHaveCard1);
				solver->AddClause(playerDoesNotHaveCard2);
				solver->AddClause(playerDoesNotHaveCard3);
			}
		}
	}
	else
	{
		cout << "No case handled. " << "Suggester: " << suggester << ", card_shown: " << card_shown << endl;
		cout << endl;
	}
	#endif
}


void ClueReasoner::Accuse(string suggester, string card1, string card2, string card3, bool is_correct)
{
	// TO BE IMPLEMENTED AS AN EXERCISE
}
