#pragma once

#include <random>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>

using std::rand;
using std::vector;
using std::cout;
using std::string;
using std::stringstream;

/**
 * A number guessing game class
 * Default Setting is a game to guess a number between 1 and 100 inclusive for both.
**/
class NumberGuessingGame
{

private:
	
	const int DEFAULT_MAXIMUM_NUMBER = 100; /** The default maximum number that can be picked */

	int m_MaximumNumber;	/** The maximum number that can be guessed and picked for the target number */

	int m_TargetNumber;	/** Target number for the guessing game. */

	vector<int> m_GuessedNumbers;   /** The guessed numbers */

	bool m_GameFinished;	/** true if the target number has been guessed*/

public:

#pragma region Constructor & Destructor

	/**
	 * Default constructor.
	**/
	NumberGuessingGame();

	/**
	 * Constructor.
	 * @param	TargetNumber	The target number override.
	**/
	NumberGuessingGame( int TargetNumber );
	
	/**
	 * Destructor.
	**/
	~NumberGuessingGame();

#pragma endregion
		
	/**
	 * Guess number.
	 * @param	GuessedNumber 	The guessed number.
	 * @param [in,out]	output	The output of the function
	 * @return	ture if nothing went wrong. false if something went wrong. See console for details if something went wrong.
	**/
	bool GuessNumber( int GuessedNumber, string& output);

	/**
	 * Attempts to restart the game.
	 * @return	true if it succeeds, false if it fails.
	**/
	bool RestartGame( void );

#pragma region Getters

	/**
	 * Gets guessed numbers.
	 * @return	The guessed numbers in vector format.
	**/
	vector<int> GetGuessedNumbers( void );

	/**
	 * Gets target number.
	 * @return	The target number.
	**/
	int GetTargetNumber( void );

	/**
	 * Gets maximum number.
	 * @return	The maximum number that can be picked.
	**/
	int GetMaximumNumber( void );

	/**
	 * Query if the game is finished.
	 * @return	true if the game is finished, false if not.
	**/
	bool isGameFinished( void );

#pragma endregion

};

