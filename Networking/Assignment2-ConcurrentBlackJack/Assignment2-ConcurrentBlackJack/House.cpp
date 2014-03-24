#include "House.h"

namespace Blackjack
{
	House::House() : GenericPlayer(  )
	{
	}

	House::~House()
	{
	}

	bool House::IsHitting() const
	{
		return ( GetTotal() <= 16 );
	}

	void House::FlipFirstCard()
	{
		if ( !( m_Cards.empty() ) )
			m_Cards[ 0 ]->Flip();
		else cout << "No card to flip!\n";
	}
}

