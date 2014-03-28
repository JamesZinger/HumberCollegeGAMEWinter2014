#pragma once

#include <iostream>
#include <sstream>
#include <boost/interprocess/containers/map.hpp>

using std::ostream;
using std::stringstream;

namespace Blackjack
{

	class Card
	{
	public:
		enum rank
		{
			ACE = 1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
			JACK, QUEEN, KING
		};
		enum suit { CLUBS, DIAMONDS, HEARTS, SPADES };

		//overloading << operator so can send Card object to standard output 
		friend ostream& operator<<( ostream& os, const Card& aCard );

		Card( rank r = ACE, suit s = SPADES, bool ifu = true );

		//returns the value of a card, 1 - 11
		int GetValue() const;

		//flips a card; if face up, becomes face down and vice versa
		void Flip() { m_isFaceUp = !m_isFaceUp; }

		void toString( std::stringstream ss );
		
		bool FaceUp() const { return m_isFaceUp; }

	protected:

		void FaceUp( bool val ) { m_isFaceUp = val; }

	private:
		rank m_Rank;
		suit m_Suit;
		bool m_isFaceUp;
		
		static boost::interprocess::map<rank, std::string> m_rankMap;
		static boost::interprocess::map<suit, std::string> m_suitMap;
	};

	//ostream& operator<<( ostream& os, const Card& aCard );

}