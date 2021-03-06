#include "Hand.h"

namespace Blackjack
{
	using boost::interprocess::vector;

	Hand::Hand()
	{
		m_Cards.reserve( 7 );
	}

	Hand::~Hand()  //don't use the keyword virtual outside of class definition
	{
		Clear();
	}

	void Hand::Add( Card* pCard )
	{
		m_Cards.push_back( pCard );
	}

	void Hand::Clear()
	{
		//iterate through vector, freeing all memory on the heap
		for ( auto iter = m_Cards.begin(); iter != m_Cards.end(); ++iter )
		{
			delete *iter;
			*iter = 0;
		}
		//clear vector of pointers
		m_Cards.clear();
	}

	int Hand::GetTotal() const
	{
		//if no cards in hand, return 0
		if ( m_Cards.empty() )
			return 0;

		//if a first card has value of 0, then card is face down; return 0
		if ( m_Cards[ 0 ]->GetValue() == 0 )
			return 0;

		//add up card values, treat each Ace as 1
		int total = 0;
		for ( auto iter = m_Cards.begin(); iter != m_Cards.end(); ++iter )
			total += ( *iter )->GetValue();

		//determine if hand contains an Ace
		bool containsAce = false;
		for ( auto iter = m_Cards.begin(); iter != m_Cards.end(); ++iter )
			if ( ( *iter )->GetValue() == Card::ACE )
				containsAce = true;

		//if hand contains Ace and total is low enough, treat Ace as 11
		if ( containsAce && total <= 11 )
			//add only 10 since we've already added 1 for the Ace
			total += 10;

		return total;
	}


}
