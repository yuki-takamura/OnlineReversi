#ifndef ___Class_Reversi
#define ___Class_Reversi

#include "NetworkManager.h"
#include "DrawObject.h"
#include "Board.h"
#include "Stone.h"
#include "Guide.h"
#include "Player.h"
#include "Enemy.h"

enum TurnState
{
	PLAYER,
	ENEMY
};

class Reversi
{
public:
	Reversi(bool);
	void run();

	CPUType cpu = Other;

private:
	///<summary>
	///windows‚ª7ˆÈ‘O‚©‚Ç‚¤‚©
	///</summary>
	bool isVersion7;
	bool canNotPut[2];
	int stoneCounter[2];

	Guide guide;
	Board board;
	Stone stone[VERTICAL][HORIZONTAL];
	Player player[2];
	Enemy enemy;

	NetworkManager networkManager;

	TurnState currentTurn;

	void initialize();
	void update();
	void draw();
};
#endif