#include "Card.h"



Card::Card(int entityID, CardType type, CardRarity rarity)
	:m_entityID(entityID), m_type(type), m_rarity(rarity)
{
}


Card::~Card()
{
}
