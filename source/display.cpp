#include "pongGame.h"


Display::Display(int width, int height) {
	//set window dimension (resolution)
	this->width = width;
	this->height = height;

	//initialize clear window buffers
	frame1 = Mat::zeros(height, width, CV_8UC3);
	frame2 = Mat::zeros(height, width, CV_8UC3);
	clearWindow();

	//calculate graphic sizes (scalable with resolution)
	boardOffset = width / 200;
	boardThickness = (width / height) * 8;
	textThickness = boardThickness / 3;
	textScale = height / 240;

	//default bools
	frameLocked = false;
	running = true;

	//start thread to update frame every 16ms: ~60fps
	frameThread = thread(&Display::updateFrame, this);
}

Display::~Display() {
	running = false;		//stop while loop in thread
	frameThread.join();		//wait for thread to finnish
}

//update screen method
void Display::updateFrame() {
	namedWindow("Pong", WINDOW_NORMAL);
	resizeWindow("Pong", width, height);
	//constantly update winow with new frame
	while (running) {
		frameUpdating = true;		//lock frame
		if (frameLocked) {
			imshow("Pong", frame2);
		}
		else {
			imshow("Pong", frame1);
		}
		frameUpdating = false;
		waitKey(17);	//1sec/60fps~17ms
	}
	destroyWindow("Pong");
}

//basic frame methods
void Display::clearWindow() {
	if (frameLocked) {
		frame1.setTo(frameColor);
	}
	else {
		frame2.setTo(frameColor);
	}	
}

//basic shape methods
void Display::drawRect(int x, int y, int w, int h, int thickness, Scalar color) {	//thickness -1 = full rectangle
	Point p1(x, y);
	Point p2(x + w, y + h);
	if (frameLocked) {
		rectangle(frame1, p1, p2, color, thickness);
	}
	else {
		rectangle(frame2, p1, p2, color, thickness);
	}
}
void Display::drawString(string str, int x, int y, int scale, int thickness, Scalar color) {
	Point p(x, y);
	if (frameLocked) {
		putText(frame1, str, p, FONT_HERSHEY_PLAIN, scale, color, thickness, LINE_AA);
	}
	else {
		putText(frame2, str, p, FONT_HERSHEY_PLAIN, scale, color, thickness, LINE_AA);
	}
}
//--------//

//methods used to update frame
void Display::gameBoard(string score, Player* player, BotPlayer* bot, Ball* ball) {
	while (frameUpdating) {
		//wait for imwrite to write buffer on screen
	}
	//clear framebuffer
	clearWindow();

	//get players position
	Position p = player->getProps();
	Position b = bot->getProps();
	
	//draw border
	drawRect(boardOffset, boardOffset, width - (2 * boardOffset), height - (2 * boardOffset), boardThickness, boardColor);
	
	//draw score
	Size str_size = getTextSize(score, FONT_HERSHEY_PLAIN, textScale, textThickness, 0);
	int x = (width / 2) - (str_size.width / 2);
	int y = boardOffset + (2 * boardThickness) + str_size.height;
	drawString(score, x, y, textScale, textThickness, textColor);
	
	//draw players on frame
	drawRect(p.x, p.y, boardThickness, p.height, -1, p.color);
	drawRect(b.x, b.y, boardThickness, b.height, -1, b.color);

	//draw ball
	drawRect(ball->x, ball->y, ball->size, ball->size, -1, ball->color);

	//unlock frame
	frameLocked = !frameLocked;
}

void Display::gameOver(bool win) {
	string endText;
	Size strSize;
	int x, y;

	//define text to show depending on game winner
	if (win) {
		endText = "You win!";
	}
	else {
		endText = "You loose!";
	}

	//clear locked frame buffer
	clearWindow();

	//calculate text size for centering
	strSize = getTextSize(endText, FONT_HERSHEY_PLAIN, textScale, textThickness*2, 0);
	x = (width / 2) - (strSize.width / 2);
	y = height / 4;

	//draw text to frame
	drawString(endText, x, y, textScale, textThickness * 2, textColor);

	//draw restart text to end frame
	strSize = getTextSize("SPACE -> play", FONT_HERSHEY_PLAIN, textScale, textThickness*2, 0);
	x = (width / 2) - (strSize.width / 2);
	y = height / 2;
	drawString("SPACE -> play", x, y, textScale, textThickness * 2, textColor);

	strSize = getTextSize("SHIFT->quit", FONT_HERSHEY_PLAIN, textScale, textThickness * 2, 0);
	x = (width / 2) - (strSize.width / 2);
	y = y - (strSize.height * 2);
	drawString("SHIFT->quit", x, y, textScale, textThickness * 2, textColor);

	//unlock frame buffer
	frameLocked = !frameLocked;
}

void Display::gameStarting() {
	//clear window
	clearWindow();

	//calculate text size to center it perfectly
	Size str_size = getTextSize("Press space to play!", FONT_HERSHEY_PLAIN, textScale, textThickness, 0);
	int x = (width / 2) - (str_size.width / 2);
	int y = height / 1.3;

	//draw text on locked frame buffer
	drawString("Press space to play!", x, y, textScale, textThickness * 2, textColor);

	//unlock frame buffer
	frameLocked = !frameLocked;
}