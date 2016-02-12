#pragma once

#include "cocos2d.h"
#include <map>

class SnakeScene : public cocos2d::Layer
{
private:
	static std::map<cocos2d::EventKeyboard::KeyCode,
		std::chrono::high_resolution_clock::time_point> keys;
public:
	//basic functions 
	static cocos2d::Scene* createScene();
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(SnakeScene);

	virtual void update(float) override;

	//objects in the game
	cocos2d::Vector<cocos2d::Sprite*> snake;
	cocos2d::Sprite* food;
	struct Level{
		cocos2d::Sprite* background;
		int tag;
		cocos2d::Vector<cocos2d::Sprite*> obstacles;
	};
	cocos2d::Sprite* background; //aka lvl1
	//screens seen in the game
	/*cocos2d::Sprite* startScreen;
	cocos2d::Sprite* endScreen;
	cocos2d::Sprite* helpScreen;*/
	cocos2d::Label* message;
	cocos2d::Label* scoreLabel;
	cocos2d::Label* highScoreLabel;

	//game variables
	int direction; //indicates direction of snake
	int count; //counter for every nth second should the snake move
	int wait; //number of when the next sequence should start
	bool start; //if a new game has started or not
	int level; //level indicator
	int score; //tracks user's current score
	int highscore; //indicates highest score
	bool eat; //indicates that user has passed title screen

	//game events
	void eatenFood();
	void restart();
	void initializeGame();
	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode code);
	void moveSnake();
	void updateScore();
	void switchLevel(int lvl);
	cocos2d::Sprite* getSnakeHead();
	cocos2d::Sprite* getSnakeBody(int tag);
};
