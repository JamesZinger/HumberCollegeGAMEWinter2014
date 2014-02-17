#include "NumberGuessingGame.h"
#include <time.h>

#pragma region ctors and dtor

NumberGuessingGame::NumberGuessingGame()
{
	srand( time( NULL ) );

	m_MaximumNumber = DEFAULT_MAXIMUM_NUMBER;

	m_TargetNumber = rand() % m_MaximumNumber;

	m_GameFinished = false;
}

NumberGuessingGame::NumberGuessingGame( int targetNumber )
{
	m_MaximumNumber = DEFAULT_MAXIMUM_NUMBER;

	if ( targetNumber > m_MaximumNumber )
	{

		cout << "Target Number is greater than the Maximum number that can be picked.\r\n";

		cout << "Sent target number: " << targetNumber << "\r\n";

		cout << "Maximum target number: " << m_MaximumNumber << "\r\n";

		cout << "Reseting the target number to the maximum target number\r\n";

		targetNumber = m_MaximumNumber;
	}

	else if ( targetNumber < 0 )
	{
		cout << "Target Number is less than 1.\r\n";

		cout << "Sent target number: " << targetNumber << "\r\n";

		cout << "Reseting the target number to 0\r\n";

		targetNumber = 0;
	}

	m_TargetNumber = targetNumber;

	m_GameFinished = false;
}

NumberGuessingGame::~NumberGuessingGame()
{
	m_GuessedNumbers.clear();
}

#pragma endregion

bool NumberGuessingGame::GuessNumber( int GuessedNumber, string& out )
{

	if ( m_GameFinished )
	{
		cout << "Game is Finished. Cannot guess a number";
	}

	if ( GuessedNumber > m_MaximumNumber )
	{
		cout << "Guessed Number is greater than the maximum guessed number\r\n";
		cout << "Guessed Number: " << GuessedNumber << "\r\n";
		cout << "Maximum Number: " << m_MaximumNumber << "\r\n";
		
		return false;
	}

	else if ( GuessedNumber < 1 )
	{
		cout << "Guessed Number is less than 1\r\n";
		cout << "Guessed Number: " << GuessedNumber << "\r\n";

		return false;
	}

	m_GuessedNumbers.push_back( GuessedNumber );

	stringstream ss = stringstream();

	if ( GuessedNumber < m_TargetNumber )
	{
		ss << "Your number is less than my number. ";
	}

	else if ( GuessedNumber > m_TargetNumber )
	{
		ss << "Your number is greater than my number. ";
	}
	else if ( GuessedNumber == m_TargetNumber )
	{
		ss << "Congratulation! Your guess was right. ";
		m_GameFinished = true;
	}
	else
	{
		cout << "Something went terribly wrong\r\n";
		cout << "Guessed Number: " << GuessedNumber << "\r\n";
		cout << "Target Number: " << m_TargetNumber << "\r\n";
		cout << "Maximum Number: " << m_MaximumNumber << "\r\n";

		return false;
	}

	ss << "The number of guesses is: " << m_GuessedNumbers.size( ) << "\r\n";

	out = ss.str();

	return true;

}

bool NumberGuessingGame::RestartGame( void )
{
	srand( time( NULL ) );

	m_TargetNumber = rand( ) % m_MaximumNumber;

	m_GameFinished = false;

	m_GuessedNumbers.clear();

	return true;
}

#pragma region Getters

vector<int> NumberGuessingGame::GetGuessedNumbers( void )
{
	return m_GuessedNumbers;
}

int NumberGuessingGame::GetTargetNumber( void )
{
	return m_TargetNumber;
}

int NumberGuessingGame::GetMaximumNumber( void )
{
	return m_MaximumNumber;
}

bool NumberGuessingGame::isGameFinished( void )
{
	return m_GameFinished;
}

#pragma endregion