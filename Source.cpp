// Tiffany Culbertson
// July 31" 2022
// Program Name: Final Project - Blackjack Game
// Program Desc: 

#include<stdlib.h>
#include<iostream>
#include<ctime>
#include<iomanip>
#include<string>
#include<fstream>

using namespace std;


float GetBet();
void NewHand();

void AceCheck(int counter, int& cardTotal, int* HandValArr);
void PrintCards(int cardCount, bool standing);
bool IsSameCard(string card);
string Deal(int i, bool isPlayer);
string RandomCard(int i, bool isPlayer);

void Hit();
void Stand();

bool RoundOverCheck(int cases);
void GameOver();

void NewGame();
void LoadGame();
void SaveGame();
void ClearData();

const string cards[4][13] = 
{ 
	{"Ace of Spades","2 of Spades","3 of Spades","4 of Spades","5 of Spades","6 of Spades","7 of Spades","8 of Spades","9 of Spades","10 of Spades","Jack of Spades", "Queen of Spades", "King of Spades"},
	{"Ace of Diamonds","2 of Diamonds","3 of Diamonds","4 of Diamonds","5 of Diamonds","6 of Diamonds","7 of Diamonds","8 of Diamonds","9 of Diamonds","10 of Diamonds","Jack of Diamonds", "Queen of Diamonds", "King of Diamonds"},
	{"Ace of Hearts","2 of Hearts","3 of Hearts","4 of Hearts","5 of Hearts","6 of Hearts","7 of Hearts","8 of Hearts","9 of Hearts","10 of Hearts","Jack of Hearts", "Queen of Hearts", "King of Hearts"},
	{"Ace of Clubs","2 of Clubs","3 of Clubs","4 of Clubs","5 of Clubs","6 of Clubs","7 of Clubs","8 of Clubs","9 of Clubs","10 of Clubs","Jack of Clubs", "Queen of Clubs", "King of Clubs"}
};

const int values[13] = { 11,2,3,4,5,6,7,8,9,10,10,10,10 };

string dealerHand[10];
string playerHand[10];
string placeholderSuite;
string placeholderValue;
int dealerHandVal[10];
int playerHandVal[10];
int dealerTotal = 0;
int playerTotal = 0;
int wins;
int losses;
int ties;
float playerMoney;
float betAmount;

ofstream saveData;
ifstream loadData;


string Deal(int i, bool isPlayer)
{
	string randomCard = RandomCard(i, isPlayer);

	while (IsSameCard(randomCard))
	{
		randomCard = RandomCard(i, isPlayer);
	}

	if (isPlayer)
		playerTotal += playerHandVal[i];
	else
		dealerTotal += dealerHandVal[i];

	return randomCard;
}

string RandomCard(int i, bool isPlayer)
{
	int currSuite, currValue;

	currSuite = rand() % 3;
	currValue = rand() % 12;

	if (isPlayer)
		playerHandVal[i] = values[currValue];
	else
		dealerHandVal[i] = values[currValue];

	return cards[currSuite][currValue];
}

bool IsSameCard(string card)
{
	for (int i = 0; i < 10; i++)
	{
		if (dealerHand[i] == card || playerHand[i] == card)
		{
			return true;
		}
	}
	return false;
}

void AceCheck(int counter, int& cardTotal, int* HandValArr)
{
	for (int i = 0; i <= counter; i++)
	{
		if (cardTotal <= 21)
			break;

		if (HandValArr[i] == 11)
		{
			HandValArr[i] = 1;
			cardTotal -= 10;
		}
	}
}

void PrintCards(int cardCount, bool standing)
{
	cout << endl;
	cout << setw(30);
	cout << left << "PLAYER";
	cout << setw(30);
	cout << right << "DEALER";

	for (int i = 0; i <= cardCount; i++)
	{
		cout << endl;
		cout << setw(30);
		cout << left << playerHand[i];

		if (!standing && i < 1 || standing)
		{
			cout << setw(30);
			cout << right << dealerHand[i];
		}
	}

	cout << endl;
	cout << setw(7);
	cout << left << "Total: " << playerTotal;
	if (standing)
	{
		cout << setw(49);
		cout << right << "Total: " << dealerTotal;
	}
	cout << endl;
}

void NewHand()
{
	cout << "\nHow much would you like to bet? You currently have $" << playerMoney << ".\n" << endl;
	betAmount = GetBet();

	while (betAmount > playerMoney || betAmount == 0)
	{
		cout << "Not a valid entry." << endl;
		cout << "\nHow much would you like to bet? You currently have $" << playerMoney << ".\n" << endl;
		betAmount = GetBet();
	}

	playerMoney -= betAmount;

	srand(time(0));

	int count = 2;
	for (int i = 0; i < count; i++)
	{
		string placeholderCard;

		placeholderCard = Deal(i, true);
		playerHand[i] = placeholderCard;

		placeholderCard = Deal(i, false);
		dealerHand[i] = placeholderCard;
	}

	PrintCards(count - 1, false);

	AceCheck(1, playerTotal, playerHandVal);

	if (!RoundOverCheck(2))
		Hit();
}

float GetBet()
{
	float bet;
	cin >> bet;

	if (bet < 0)
	{
		bet = abs(bet);
		cout << "Negative number! Changing to positive." << endl;
	}
	return bet;
}

void Hit()
{
	string placeholderCard;
	int choice = 0;
	int counter = 1;

	while (choice != 2)
	{
		cout << "\nEnter 1 to Hit. Enter 2 to Stand.\n" << endl;
		cin >> choice;

		while (choice != 1 && choice != 2)
		{
			cout << "Invalid Option. Enter 1 to Hit. Enter 2 to Stand.\n" << endl;
			cin >> choice;
		}

		if (choice == 2)
			break;

		counter++;
		placeholderCard = Deal(counter, true);
		playerHand[counter] = placeholderCard;

		AceCheck(counter, playerTotal, playerHandVal);

		PrintCards(counter, false);

		if (RoundOverCheck(2))
			return;
	}

	Stand();
}

void Stand()
{
	string placeholderCard;
	int counter = 1;
	float winnings = betAmount * 2;

	while (dealerTotal <= 16)
	{
		counter++;
		placeholderCard = Deal(counter, false);
		dealerHand[counter] = placeholderCard;
		AceCheck(counter, dealerTotal, dealerHandVal);
	}

	PrintCards(counter, true);

	RoundOverCheck(1);
}

bool RoundOverCheck(int cases)
{
	float winnings = betAmount * 2;

	switch (cases)
	{
	case 1:
		if (dealerTotal > 21)
		{
			playerMoney += winnings;
			wins++;
			cout << setw(30);
			cout << right << "\nBUST!" << endl;
			cout << "\nPlayer wins!" << endl;
			cout << fixed;
			cout << "You received $" << setprecision(2) << winnings << "!\nNew total is $" << setprecision(2) << playerMoney << endl;
			return true;
		}
		else if (dealerTotal == 21 && playerTotal != 21)
		{
			losses++;
			cout << "\nDealer wins!" << endl;
			cout << fixed;
			cout << "You lost your bet of $" << setprecision(2) << betAmount << ".\nNew total is $" << setprecision(2) << playerMoney << endl;
			return true;
		}
		else if (dealerTotal > playerTotal)
		{
			losses++;
			cout << "\nDealer wins!" << endl;
			cout << fixed;
			cout << "You lost your bet of $" << setprecision(2) << betAmount << ".\nNew total is $" << setprecision(2) << playerMoney << endl;
			return true;
		}
		else if (dealerTotal == playerTotal)
		{
			winnings /= 2;
			playerMoney += winnings;
			ties++;
			
			cout << "\nTie!" << endl;
			cout << fixed;
			cout << "You received your bet of $" << setprecision(2) << winnings << " back!\nNew total is $" << setprecision(2) << playerMoney << endl;
			return true;
		}
		else
		{
			playerMoney += winnings;
			wins++;

			cout << "\nPlayer wins!" << endl;
			cout << fixed;
			cout << "You received $" << setprecision(2) << winnings << "!\nNew total is $" << setprecision(2) << playerMoney << endl;
			return true;
		}
		break;
	case 2:
		if (playerTotal > 21)
		{
			losses++;
			cout << setw(30);
			cout << left << "\nBUST!" << endl;
			cout << "Dealer wins!" << endl;
			cout << fixed;
			cout << "You lost your bet of $" << setprecision(2) << betAmount << ".\nNew total is $" << setprecision(2) << playerMoney << endl;
			return true;
		}
		else if (playerTotal == 21)
		{
			playerMoney += winnings;
			wins++;
			cout << "\nPlayer wins!" << endl;
			cout << fixed;
			cout << "You received $" << setprecision(2) << winnings << "!\nNew total is $" << setprecision(2) << playerMoney << endl;
			return true;
		}
		break;
	}

	return false;
}

void GameOver()
{
	int menu;
	if (playerMoney <= 0)
	{
		//Resets values if you lose so you can't keep playing on the same save file after you lose
		NewGame();

		cout << "Game Over!" << endl;
		cout << "Would you like to start a new game?\n1. Yes\n2. End Game\n" << endl;
		cin >> menu;

		while (menu != 1 && menu != 2)
		{
			cout << "Invalid option!\n" << endl;
			cout << "Would you like to start a new game?\n1. Yes\n2. End Game\n" << endl;
			cin >> menu;
		}

		if (menu == 2)
			exit(0);

	}
}

void NewGame()
{
	playerMoney = 1000;
	wins = 0;
	losses = 0;
	ties = 0;
	ClearData();
	SaveGame();
}


void SaveGame()
{
	saveData.open("savefile.txt");

	if (saveData.is_open())
	{
		saveData << playerMoney << " " <<  wins << " " << losses << " " << ties;
	}

	saveData.close();
}

void LoadGame()
{
	loadData.open("savefile.txt");

	if (loadData.is_open())
	{
		loadData >> playerMoney >> wins >> losses >> ties;
	}

	string winsText = wins != 1 ? "games" : "game";
	string lossText = losses != 1 ? "games" : "game";
	string tieText = ties != 1 ? "games" : "game";

	cout << "You've won " << wins << " " << winsText << " and lost " << losses << " " << lossText << " and tied " << ties << " " << tieText << "." << endl;

	loadData.close();
}

void ClearData()
{
	dealerTotal = 0;
	playerTotal = 0;
	betAmount = 0;
}

int main()
{
	int menu;
	cout << "Welcome to Blackjack!" << endl;
	cout << "1. Start a New Game\n2. Load a Previously Saved Game\n3. Exit\n" << endl;
	cin >> menu;

	while (menu != 1 && menu != 2 && menu != 3)
	{
		cout << "Invalid option!\n" << endl;
		cout << "1. Start a New Game\n2. Load a Previously Saved Game\n3. Exit\n" << endl;
		cin >> menu;
	}

	if (menu == 2)
	{
		LoadGame();
		menu = 1;
	}
	else if (menu == 1)
	{
		NewGame();
	}
		
	while (menu == 1)
	{
		NewHand();
		ClearData();
		SaveGame();

		if (playerMoney > 0)
		{
			cout << "\nWould you like to play another hand?\n1. Yes\n2. End Game\n" << endl;
			cin >> menu;

			while (menu != 1 && menu != 2)
			{
				cout << "\nInvalid Option! Would you like to play another hand?\n1. Yes\n2. End Game\n" << endl;
				cin >> menu;
			}

			if (menu == 2)
			{
				exit(0);
			}
		}

		GameOver();
	}
}
