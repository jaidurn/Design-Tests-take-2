#pragma once
class Card
{
public:
	enum CardType
	{
		CARD_STAT,
		CARD_DROP,
		CARD_ABILITY,
		CARD_WORLD,
		TYPE_END
	};

	enum CardRarity
	{
		CARD_COMMON,
		CARD_UNCOMMON,
		CARD_RARE,
		CARD_UNIQUE,
		RARITY_END
	};

	Card(int entityID, CardType type, CardRarity rarity);
	~Card();

	CardType getType() { return m_type; }
	CardRarity getRarity() { return m_rarity; }
	int getEntityID() { return m_entityID; }

private:
	CardType m_type;
	CardRarity m_rarity;
	int m_entityID;
};

