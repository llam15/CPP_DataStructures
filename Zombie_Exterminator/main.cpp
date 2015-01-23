#include "History.h"
#include "Game.h"
#include "Arena.h"
#include "Player.h"
#include "Zombie.h"

#include <cstdlib>
#include <ctime>
using namespace std;

int main()
{
    // Initialize the random number generator
    srand(static_cast<unsigned int>(time(0)));
    
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(20, 20, 20);
    
    // Play the game
    g.play();
}