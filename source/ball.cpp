#include "pongGame.h"


Ball::Ball(Display* display, Scalar color, int speed) {
	this->display = display;				//save display pointer for future use
	this->moveSpeed = speed;				//set default move speed
	this->size = display->boardThickness;	//define ball size
	this->color = color;					//set ball color

	resetBall();					//set default position and move direction
}

void Ball::resetBall() {	//set ball starting position and direction
	//generate random number to randomly pick x direction
	int randN = int(rand() % 100);

	//starting coordinates
	x = ((display->width) / 2)-size/2;
	y = ((display->height) / 2)+size/2;

	//random moving direction with small angle
	yMove = (randN >= 50) ? -1 : 1;;
	xMove = (randN <= 50) ? -(moveSpeed - abs(yMove)) : (moveSpeed - abs(yMove));
}

void Ball::moveBall(PongGame* g, Player* player, BotPlayer* bot) {
	//calculate next x,y
	int nextX = x - xMove;
	int nextY = y - yMove;
	
	//get min/max y
	int minY = min(nextY, y);
	int maxY = max(nextY, y);

	//get player and bot player positions
	Position p = player->getProps();
	Position b = bot->getProps();

	//check for collision with player
	if (x<=p.x && nextX>=p.x && minY>= p.y && maxY <=(p.y+p.height)) {
		x = p.x - size;
		y = (minY + maxY) / 2;

		if (nextY < (p.y + (p.height / 3))) {
			yMove = (yMove<=-moveSpeed/3||yMove>=moveSpeed/3) ? abs(yMove) : abs(yMove)+2;
			xMove = (moveSpeed - abs(yMove));
		}
		else if (nextY < (p.y + (p.height / 3) * 2)) {
			xMove = (moveSpeed - abs(yMove));
		}
		else {
			yMove = (yMove <= -moveSpeed/3 || yMove >= moveSpeed/3) ? -abs(yMove) : -abs(yMove)-2;
			xMove = (moveSpeed - abs(yMove));
		}
	}
	//check for collision with bot player
	else if (x>=b.x && nextX<=b.x && minY >= b.y && maxY <= (b.y+b.height)) {
		x = bot->posX + size;
		y = (minY + maxY) / 2;

		if (nextY < (b.y + (b.height / 3))) {
			yMove = (yMove <= -moveSpeed/3 || yMove >= moveSpeed/3) ? -abs(yMove) : -abs(yMove) - 2;
			xMove = -(moveSpeed - abs(yMove));
		}
		else if (nextY < (b.y + (b.height / 3) * 2)) {
			xMove = -(moveSpeed - abs(yMove));
		}
		else {
			yMove = (yMove <= -moveSpeed/3 || yMove >= moveSpeed/3) ? abs(yMove) : abs(yMove) + 2;
			xMove = -(moveSpeed - abs(yMove));
		}
	}
	//check for collision with walls
	else {
		//left and right wall->decide round winner
		if (nextX < 0) {	//collision with left wall -> player gets point
			g->updateScore(true);
			x = 0;
		}
		else if (nextX > display->width) {	//collision with right wall -> bot gets point
			g->updateScore(false);
			x = display->width;
		}
		else {
			x = nextX;
		}

		//top and bottom wall->bounce
		if (nextY < (display->boardOffset + display->boardOffset)) {
			//bounce from top
			y = display->boardOffset + display->boardOffset;
			yMove = -yMove;
		}
		else if (nextY > (display->height - display->boardOffset- display->boardThickness)) {
			//bounce from bottom
			y = display->height - display->boardOffset - display->boardThickness;
			yMove = -yMove;
		}
		else {
			y = nextY;
		}
	}
}