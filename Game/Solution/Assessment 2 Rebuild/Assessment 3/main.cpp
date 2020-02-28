#include "app.h"
int main()
{
	app Game("SFML Game", 640, 480, 32);

	if (!Game.Init())
	{
		printf("Could not initialize the game. Please restart the program or contact the game creator."); //Not necessary, but still useful. 
		return 1;
	}
	else
	{
		Game.Run();
	}
	return 0;
}