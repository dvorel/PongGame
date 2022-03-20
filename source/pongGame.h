#ifndef pongHeader
#define pongHeader

//library to install: opencv 4.5.5
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <Windows.h>
#include <thread>
#include <list>

using namespace cv;
using namespace std;

//used input keys, keys can be changed here
//to add input key edit list in KeyboardInput class
#define UP VK_UP
#define DOWN VK_DOWN
#define START VK_SPACE
#define QUIT VK_SHIFT

//structs
struct Key {
	int key;
	bool state;
};
struct Position {
	int x;
	int y;
	int height;
	Scalar color;
};
//--------//

//----input interfaces----//
class InputListener {
public:
	virtual void buttonPressed(Key) = 0;
	virtual void buttonReleased(Key) = 0;
	void buttonPressed() {};
	void buttonReleased() {};
};
class Input {
public:
	virtual void addListener(InputListener* listener) = 0;
	virtual void removeListener(InputListener* listener) = 0;
	virtual void keyState(struct Key k) = 0;
	virtual vector<Key> getKeys() = 0;
};
//--------//

//keyboard input class
class KeyboardInput : public Input {
private:
	vector<Key> keys{ { UP, false }, { DOWN, false }, { START, false }, {QUIT, false} };	//list of all keys used for input (should be updated if keys are added)
	bool listen;
	thread thrKeyboard;
	list<InputListener*> listenerList;

	void updateListeners(Key);
	void keyboardRead(bool*);

public:
	void addListener(InputListener*);
	void removeListener(InputListener*);
	KeyboardInput();
	~KeyboardInput();
	void keyState(Key);
	vector<Key> getKeys();
};
//--------//

//players classes -> player position and moveing
class Players {
public:
	int score;				//player score	
	int speed;				//player move speed
	int height;				//player height	
	int minY, maxY;			//min and max y values
	bool up, down;			//key trigger bools
	bool running;			//thread while running criteria
	thread playerThread;	//thread variable
	int posX;				//x position on frame
	int posY;				//y position on frame
	Scalar color;			//player color set on object creation, default->white
};

class Player : private Players, public InputListener{
private:
	void move();	//method to run in thread, moves player depending on keyboard input

public:
	Player(class Display*,Scalar, int);		//constructor getting display object to scale sizes
	~Player();

	//input listener triggers
	void buttonPressed(struct Key);
	void buttonReleased(struct Key);

	void resetPosition(class Display*);		//calculate starting position

	//getters
	Position getProps();	//returns struct of player position, height and color
	int getScore();			//returns player score

	//setters
	void setScore(int);		//sets player score
	void setMoveSpeed(int);	//sets player move speed
};

class BotPlayer : public Players{
private:
	class Ball* ball;
	thread thrBot;
	void move();		//method to run in thread
	int makeMove();		//move bot player

public:
	BotPlayer(class Display*, class Ball*, Scalar, int);	//constructor getting display object to scale sizes
	~BotPlayer();

	void resetPosition(class Display*);		//set position to defualt value

	//getters
	Position getProps();	//returns struct of player position, height and color
	int getScore();			//returns player score

	//setters
	void setScore(int);		//sets player score
	void setMoveSpeed(int);	//sets player move speed
};
//--------//

//ball class -> sets ball properties
class Ball {
public:
	int size;				//ball size
	int x, y;				//ball position
	int xMove, yMove;		//ball move 
	int moveSpeed;			//move speed
	Scalar color;			//ball color
	class Display* display;

	Ball(class Display*, Scalar, int);
	~Ball();
	void moveBall(class PongGame*, Player*, BotPlayer*);
	void resetBall();
};
//--------//

//Display class -> displaying frame buffer on screen//
class Display {
private:
	//ova metoda je zamjena za void commit()
	void updateFrame();

public:
	int height, width;		//resolution
	//scalable graphics vars
	int boardOffset;
	int boardThickness;
	int textThickness;
	int textScale;

	//thread
	bool running;
	bool frameLocked, frameUpdating;
	thread frameThread;
	
	Mat frame1, frame2;		//2 frame buffers are used to remove flickering
	//colors
	Scalar frameColor = Scalar(0, 0, 0);
	Scalar textColor = Scalar(255, 255, 255);
	Scalar boardColor = Scalar(255, 255, 255);

	void clearWindow();		//fill frame with default color
	void drawRect(int, int, int, int, int, Scalar);
	void drawString(string, int, int, int, int, Scalar);
	void gameBoard(string, Player*, BotPlayer*, Ball*);
	void gameOver(bool);
	void gameStarting();
	Display(int, int);
	~Display();
};
//--------//

//main class -> create objects, start game
class PongGame : public InputListener {
private:
	//object pointers
	class Display* display;
	class Player* player;
	class BotPlayer* botPlayer;
	class Ball* ball;
	class KeyboardInput* keyboard;
	
	int screen;			//0->starting screen, 1->you win, 2->you lose, else->playing
	bool gameWinner;

	//game 
	bool running;
	bool playing;
	void startGame();

public:
	PongGame(int, int, int);
	void run();
	void buttonPressed(Key);
	void buttonReleased(Key);
	string scoreString();
	void updateScore(bool);
	void resetGame();
	void resetBoard();
};

#endif