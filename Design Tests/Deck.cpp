#include "Deck.h"
#include <iostream>
#include <string>

Deck::Deck()
{
}


Deck::~Deck()
{
	for (unsigned int i = 0; i < m_cards.size(); i++)
	{
		delete m_cards[i];
	}
}

void Deck::shuffle()
{
	srand(SDL_GetTicks());

	for (unsigned int i = 0; i < m_cards.size(); i++)
	{
		int cardPlace = rand() % m_cards.size();

		while (cardPlace == i)
		{
			cardPlace = rand() % m_cards.size();
		}

		Card *holder = NULL;

		holder = m_cards[i];

		m_cards[i] = m_cards[cardPlace];
		m_cards[cardPlace] = holder;
	}
}

void Deck::sort()
{
	Card *holder = NULL;

	for (unsigned int i = 0; i < m_cards.size(); i++)
	{
		for (unsigned int j = 0; j < m_cards.size() - i; j++)
		{
			if (j != m_cards.size() - 1)
			{
				if (m_cards[j + 1]->getType() < m_cards[j]->getType())
				{
					holder = m_cards[j + 1];

					m_cards[j + 1] = m_cards[j];
					m_cards[j] = holder;
				}
				else if (m_cards[j + 1]->getType() == m_cards[j]->getType())
				{
					if (m_cards[j + 1]->getRarity() < m_cards[j]->getRarity())
					{
						holder = m_cards[j + 1];
						
						m_cards[j + 1] = m_cards[j];
						m_cards[j] = holder;
					}
				}
			}
		}
	}
}

void Deck::addCard(int cardID, Card::CardType type, Card::CardRarity rarity)
{
	Card *card = new Card(cardID, type, rarity);

	m_cards.push_back(card);

	sort();
}

Card* Deck::getCard(int index)
{
	Card *card = NULL;

	if (0 <= index && index < (int)m_cards.size())
	{
		card = m_cards[index];
	}

	return card;
}

void Deck::outputCards()
{
	for (unsigned int i = 0; i < m_cards.size(); i++)
	{
		std::string cardString = std::to_string(i) + ": ";

		if (m_cards[i])
		{
			cardString += " Type: ";

			switch (m_cards[i]->getType())
			{
			case Card::CARD_ABILITY:
			{
				cardString += "Ability. ";
				break;
			}
			case Card::CARD_DROP:
			{
				cardString += "Drop. ";
				break;
			}
			case Card::CARD_STAT:
			{
				cardString += "Stat. ";
				break;
			}
			case Card::CARD_WORLD:
			{
				cardString += "World. ";
				break;
			}
			}

			cardString += "Rarity: ";
			
			switch (m_cards[i]->getRarity())
			{
			case Card::CARD_COMMON:
			{
				cardString += "Common. \n";
				break;
			}
			case Card::CARD_UNCOMMON:
			{
				cardString += "Uncommon.\n";
				break;
			}
			case Card::CARD_RARE:
			{
				cardString += "Rare.\n";
				break;
			}
			case Card::CARD_UNIQUE:
			{
				cardString += "Unique.\n";
				break;
			}
			}

			std::cout << cardString;
		}
	}
}