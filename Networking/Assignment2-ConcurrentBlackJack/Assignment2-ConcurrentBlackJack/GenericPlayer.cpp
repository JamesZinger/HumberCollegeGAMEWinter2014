#include "GenericPlayer.h"

namespace Blackjack
{
	using boost::interprocess::vector;

	ostream& operator<<( ostream& os, const GenericPlayer& aGenericPlayer )
	{
		//os << aGenericPlayer.m_Name << ":\t";

		vector<Card*>::const_iterator pCard;
		if ( !aGenericPlayer.m_Cards.empty() )
		{
			for ( pCard = aGenericPlayer.m_Cards.begin();
				  pCard != aGenericPlayer.m_Cards.end(); ++pCard )
				  os << *( *pCard ) << "\t";

			if ( aGenericPlayer.GetTotal() != 0 )
				cout << "(" << aGenericPlayer.GetTotal() << ")";
		}
		else
		{
			os << "<empty>";
		}

		return os;
	}

	GenericPlayer::GenericPlayer(  )
	{
	}

	GenericPlayer::~GenericPlayer()
	{
	}

	bool GenericPlayer::IsBusted() const
	{
		return ( GetTotal() > 21 );
	}

	void GenericPlayer::Bust() const
	{
		//cout << m_Name << " busts.\n";
	}

}
