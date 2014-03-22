#pragma once

#include "GenericPlayer.h"

#include <iostream>

using std::cout;

namespace Blackjack
{
	class House :
		public GenericPlayer
	{
	public:
		House( const string& name = "House" );

		virtual ~House();

		//indicates whether house is hitting - will always hit on 16 or less
		virtual bool IsHitting() const;

		//flips over first card
		void FlipFirstCard();
	};
}
