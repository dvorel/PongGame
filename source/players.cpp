#include "pongGame.h"


//----player----//
Player::Player(Display* display,Scalar color, int speed = 10) {
	this->color = color;
	this->speed = speed;

	//set bools to default values
	this->up = false;
	this->down = false;
	this->running = true;	

	//calculate x position, player will be right
	this->posX = display->width - (display->boardOffset + (display->boardThickness * 3));
	
	//calculating player height (display height / 6)
	this->height = (display->height) / 6;
	
	//calculating starting y position from display height
	resetPosition(display);

	//calculate min and max values for y
	this->minY = display->boardOffset + display->boardThickness;
	this->maxY = display->height - minY - height;

	//start thread to move player
	this->playerThread = thread(&Player::move, this);
}

Player::~Player() {
	running = false;		//stop while loop in thread
	playerThread.join();	//wait for thread to finish
}

//player input
void Player::buttonPressed(Key key) {
	//set bool depending on button pressed
	if (key.key == UP) {
		up = true;
	}
	else if (key.key == DOWN) {
		down = true;
	}
}
void Player::buttonReleased(Key key) {
	//set bool to false depending on button released
	if (key.key == UP) {
		up = false;
	}
	else if (key.key == DOWN) {
		down = false;
	}
}

//player get methods
Position Player::getProps() {
	//returns data needed to draw player
	return Position{ posX, posY, height, color};
}
int Player::getScore() {
	//returns score
	return score;
}

//player set methods
void Player::setScore(int score) {
	//sets score
	this->score = score;
}
void Player::setMoveSpeed(int speed) {
	//sets speed
	this->speed = speed;
}

//player methods
void Player::move() {
	//running in thread to move player up and down
	while (running) {
		if (up && posY > minY) {
			posY -= speed;
		}
		if (down && posY < maxY) {
			posY += speed;
		}
		Sleep(5);	//200hz input refresh rate
	}
}
void Player::resetPosition(Display* display) {	//set default player position
	//set player position in the middle of height
	posY = (display->height / 2) - (height / 2);
}

//----bot player----//
BotPlayer::BotPlayer(Display* display, Ball* ball, Scalar color = (255, 255, 255), int speed = 10) {
	this->ball = ball;		//save ball pointer
	this->color = color;	//set player color
	this->speed = speed;	//set player speed

	//set bools to default values
	this->up = false;
	this->down = false;
	this->running = true;

	//calculate x position, 
	this->posX = display->boardOffset + (display->boardThickness * 2);
	//calculating player height (display height / 6)
	this->height = (display->height) / 6;
	//calculating starting y position from display height
	resetPosition(display);
	//save min and max values for y
	this->minY = display->boardOffset + display->boardThickness;
	this->maxY = display->height - minY;

	//start thread to move bot
	this->thrBot = thread(&BotPlayer::move, this);
}
BotPlayer::~BotPlayer() {
	running = false;	//stop bot player while
	thrBot.join();		//wait for thread to stop
}

//run in thread to adjust bot position
void BotPlayer::move() {
	int i = 0;
	//running in thread to move bot up and down
	while (running) {
		i = makeMove();
		if (i == 1) {
			posY += speed;
		}
		else if (i == 2) {
			posY -= speed;
		}
		Sleep(5);	//200hz input refresh rate
	}
}
int BotPlayer::makeMove() {		//move bot depending on ball position
	int yMid = posY + (height / 2);	//player mid position

	if (yMid < ball->y && (posY + height) <= maxY) {
		return 1;
	}
	else if (yMid > ball->y && (posY) >= minY) {
		return 2;
	}
	return 0;
}

void BotPlayer::resetPosition(Display* display) {		//set default bot position
	posY = (display->height / 2) - (height / 2);
}

//setters
void BotPlayer::setScore(int score) {
	//sets bot score
	this->score = score;
}
void BotPlayer::setMoveSpeed(int speed) {
	//sets bot move speed
	this->speed = speed;
}
//getters
Position BotPlayer::getProps() {
	//returns bot data needed to draw on screen
	return Position{ posX, posY, height, color };
}

int BotPlayer::getScore() {
	//returns bot player score
	return score;
}