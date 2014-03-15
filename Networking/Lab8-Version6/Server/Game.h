#pragma once
class Game
{
public:
	
	bool virtual Start() = 0;
	bool virtual End() = 0;
	bool virtual Restart() = 0;
};

