#pragma once
#include <vector>
#include <SDL.h>
#include "Card.h"

class Deck
{
public:
	Deck();
	~Deck();

	void shuffle();
	void sort();

	void addCard(int cardID, Card::CardType type, Card::CardRarity rarity);
	Card* getCard(int index);
	int getCardCount() { return m_cards.size(); }

	void outputCards();

private:
	std::vector<Card*> m_cards;
};

