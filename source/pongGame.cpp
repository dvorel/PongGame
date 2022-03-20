#include "pongGame.h"


void PongGame::startGame() {
	while (running) {
		if (screen == 0) {
			//show starting screen
			display->gameStarting();
		}
		else if (screen == 1) {
			resetBoard();		//reset all vars from previous game
			display->gameBoard(scoreString(), player, botPlayer, ball);	//draw first frame
			Sleep(1500);	//wait 1.5 second so players can prepare
			while (playing) {
				//update ball location
				ball->moveBall(this, player, botPlayer);
				//update frame buffer
				display->gameBoard(scoreString(), player, botPlayer, ball);
				Sleep(4);	//decides game speed
			}
		}
		else if (screen == 2) {
			//show you win/you loose
			//shift->quit, space->play
			display->gameOver(gameWinner);
		}
		Sleep(17);	//~60hz background refresh
	}
}

void PongGame::resetBoard() {
	//resets board when point is scored
	playing = true;
	ball->resetBall();
	player->resetPosition(display);
	botPlayer->resetPosition(display);
}

void PongGame::resetGame() {
	//resets all when winner is decided
	player->setScore(0);
	botPlayer->setScore(0);
	resetBoard();
}

string PongGame::scoreString() {
	//score int to string
	string s1 = to_string(botPlayer->getScore());
	string s2 = to_string(player->getScore());
	return s1 + " : " + s2;
}

PongGame::PongGame(int width, int height, int dificulty) {
	
	//create objects
	display = new Display(width, height);			//create display object
	ball = new Ball(display, Scalar(0,0,255), 14);					//create ball object
	player = new Player(display, Scalar(255,255,255), 10);				//create player object
	botPlayer = new BotPlayer(display, ball, Scalar(255,255,255), dificulty);	//create bot player, last number defines game dificulty (lower=easier)
	keyboard = new KeyboardInput();		//create keyboard input

	//add keyboard listeners: player and pongGame
	keyboard->addListener(this->player);	//uses keyboard input to move
	keyboard->addListener(this);			//uses keyboard input to start and quit game

	resetGame();	//set game default values
}

void PongGame::updateScore(bool winner) {
	//winner: true -> player, false -> bot
	if (winner) {	//add player 1 point
		player->setScore(player->getScore() + 1);
		if (player->getScore() > 2) {
			gameWinner = winner;
			screen = 2;
			resetGame();
		}
	}
	else {		//add bot 1 point
		botPlayer->setScore(botPlayer->getScore() + 1);
		if (botPlayer->getScore() > 2) {
			gameWinner = winner;
			screen = 2;
			resetGame();
		}
	}
	playing = false;
}

void PongGame::run() {
	screen = 0;		//show starting screen
	running = true;	//bool for while loop
	startGame();	//start while loop
}

//keyboard input methods
void PongGame::buttonPressed(Key key) {
}
void PongGame::buttonReleased(Key key) {
	if (key.key == QUIT && screen != 1) {
		running = false;
	}
	else if (key.key == START && screen!=1) {
		screen = 1;
	}
}


//start game
int main()
{
	//height, width, bot player dificulty
	PongGame* game = new PongGame(1280, 720, 3);
	game->run();
	return 0;
}