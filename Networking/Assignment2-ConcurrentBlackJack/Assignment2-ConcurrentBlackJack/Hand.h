#pragma once 

#include "Card.h"

#include <boost/interprocess/containers/vector.hpp>


namespace Blackjack
{
	
	class Hand
	{
		
	public:
		Hand();

		virtual ~Hand();

		//adds a card to the hand
		void Add( Card* pCard );

		//clears hand of all cards
		void Clear();

		//gets hand total value, intelligently treats aces as 1 or 11
		int GetTotal() const;

	protected:
		boost::interprocess::vector<Card*> m_Cards;
	};

}