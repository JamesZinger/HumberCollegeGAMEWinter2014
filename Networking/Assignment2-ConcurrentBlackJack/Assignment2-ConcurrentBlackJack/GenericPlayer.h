#pragma once

#include "Hand.h"

#include <boost/interprocess/containers/string.hpp>
#include <iostream>

using std::cout;

namespace Blackjack
{
	
	class GenericPlayer : public Hand
	{
		friend ostream& operator<<( ostream& os, const GenericPlayer& aGenericPlayer );

	public:
		GenericPlayer();

		virtual ~GenericPlayer();

		//indicates whether or not generic player wants to keep hitting
		virtual bool IsHitting() const = 0;

		//returns whether generic player has busted - has a total greater than 21
		bool IsBusted() const;

		//announces that the generic player busts
		void Bust() const;

		const boost::interprocess::string& Name( ) { return m_Name; }

	protected:
		void Name( boost::interprocess::string val ) { m_Name = val; };

	private:
		boost::interprocess::string m_Name;

	};

}