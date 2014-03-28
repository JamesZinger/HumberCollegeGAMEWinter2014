#include "Card.h"
#include <boost/assign.hpp>

using boost::interprocess::map;
using boost::assign::map_list_of;

namespace Blackjack
{
	using std::string;
	ostream& operator<<( ostream& os, const Card& aCard )
	{
		
		if ( aCard.m_isFaceUp )
			os << Card::m_rankMap.at( aCard.m_Rank ) << Card::m_suitMap.at( aCard.m_Suit );
		else
			os << "XX";

		return os;
	}

	Card::Card( rank r, suit s, bool ifu ) : m_Rank( r ), m_Suit( s ), m_isFaceUp( ifu )
	{
	}

	int Card::GetValue() const
	{
		//if a cards is face down, its value is 0
		int value = 0;
		if ( m_isFaceUp )
		{
			//value is number showing on card
			value = m_Rank;
			//value is 10 for face cards 
			if ( value > 10 )
				value = 10;
		}
		return value;
	}

	void Card::toString( stringstream ss )
	{
		if (FaceUp())
		{
		}
		else
		{
			ss << "XX"
		}
	}
	
	map<Card::rank, string> Card::m_rankMap = map_list_of
		( ACE,		"A" )
		( TWO,		"2" )
		( THREE,	"3" )
		( FOUR,		"4" )
		( FIVE,		"5" )
		( SIX,		"6" )
		( SEVEN,	"7" )
		( EIGHT,	"8" )
		( NINE,		"9" )
		( TEN,		"10")
		( JACK,		"J" )
		( QUEEN,	"Q" )
		( KING,		"K" )
		;

	map<Card::suit, string> Card::m_suitMap = map_list_of
		( CLUBS,		"c" )
		( DIAMONDS,		"d" )
		( HEARTS,		"h" )
		( SPADES,		"s" )
		;
}
