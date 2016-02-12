#include "SnakeScene.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

USING_NS_CC;

Scene* SnakeScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SnakeScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SnakeScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	
	score = 0;
	highscore = 0;
	start = false;
	eat = false;
	initializeGame();

	this->scheduleUpdate();
    return true;
}

void SnakeScene::update(float delta){
	
	Node::update(delta);

	//start game now
	if (!start && isKeyPressed(EventKeyboard::KeyCode::KEY_S)){
		keys.erase(EventKeyboard::KeyCode::KEY_S);
		this->removeChild(message);
		scoreLabel->setVisible(true);
		highScoreLabel->setVisible(true);
		start = true;
	}

	else if (start){
		log("Game will start now!");

		//while the game is still on, move the snake
		assert(snake.size() > 0);
		moveSnake();
		auto head = getSnakeHead();
		auto fpos = food->getPosition();
		if (head->getPosition() == fpos){
			eatenFood();
			eat = true;
		}
		if (snake.size() >= 5){
			log("uh oh");
			//for (auto sncur : snake){
			int counter = 0;
			head = getSnakeHead();
			while (counter < snake.size()){
				log("Snake tag: %i", snake.at(counter)->getTag());
				if (snake.at(counter)->getTag() != 0 && head->getPosition() == snake.at(counter)->getPosition() && !eat){
					log("Not good");
					restart();
				}
				else log("So far so good");
				counter++;
			}
			counter = 0;
		}
	}
	
}

void SnakeScene::eatenFood(){
	int counter = 0;
	auto lastitem = getSnakeHead()->getPosition();
	while (counter != snake.size()-1){
		counter++;
	}
	lastitem = snake.at(counter)->getPosition();
	counter = 0;
	//if (head->getPosition() == food->getPosition()){
		log("FoooD!");
		moveSnake();
		auto head = getSnakeHead();
		auto fpos = food->getPosition();
		Sprite* snbody = Sprite::create("snake.png");
		snbody->setAnchorPoint(Vec2(0, 0));
		snbody->setPosition(lastitem);
		snbody->setTag(snake.size());
		this->addChild(snbody, 1);
		snake.pushBack(snbody);

		score++;
		std::stringstream ss;
		ss << "Score: " << score;
		scoreLabel->setString(ss.str().c_str());

		for (auto sncur : snake){
			do{
				//31x23
				food->setPosition((rand() % 30 + 1) * 20, (rand() % 22 + 1) * 20);

			} while (food->getPosition() == head->getPosition() && sncur->getPosition() == food->getPosition() &&
				food->getPositionX() <= 0 && food->getPositionY() <= 0 &&
				food->getPositionX() >= 620 && food->getPositionY() >= 440);
		}
		eat = true;
	//}
}

void SnakeScene::initializeGame(){
	this->removeAllChildren();
	snake = Vector<Sprite*>{};
	Sprite* head = Sprite::create("snake.png");

	head->setTag(0);

	//keyboard event for snake sprite
	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode,
		Event* event){

		if (keys.find(keyCode) == keys.end()){
			keys[keyCode] = std::chrono::high_resolution_clock::now();
		}

		Vec2 loc = event->getCurrentTarget()->getPosition();
		switch (keyCode){
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
		case EventKeyboard::KeyCode::KEY_W:{ //up
			if (direction != 0 && direction != 2 && start){
				//event->getCurrentTarget()->setPosition(loc.x, loc.y + 20);
				direction = 0;
			}
		}; break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_A:{ //left
			if (direction != 1 && direction != 3 && start){
				//event->getCurrentTarget()->setPosition(loc.x - 20, loc.y);
				direction = 1;
			}
		};  break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
		case EventKeyboard::KeyCode::KEY_S:{//down
			if (direction != 2 && direction != 0 && start){
				//event->getCurrentTarget()->setPosition(loc.x, loc.y - 20);
				direction = 2;
			}
		}; break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_D:{ //right
			if (direction != 3 && direction != 1 && start){
				//event->getCurrentTarget()->setPosition(loc.x + 20, loc.y);
				direction = 3;
			}
		}; break;
		}
	};

	eventListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event){
		keys.erase(keyCode);
	};

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, head);

	head->setAnchorPoint(Vec2(0, 0));
	head->setPosition(Point(this->getBoundingBox().getMidX(), this->getBoundingBox().getMidY()));
	this->addChild(head, 1);

	snake.pushBack(head);

	food = Sprite::create("snake_food.png");
	background = Sprite::create("gamewalls.png");
	message = Label::createWithSystemFont("'S'tart Game", "Courier New", 30);
	scoreLabel = Label::createWithSystemFont("Score: 0", "Courier New", 30);
	highScoreLabel = Label::createWithSystemFont("High Score: 0", "Courier New", 30);

	auto positionh = head->getPosition();
	srand(time(NULL));
	do{
		//31x23
		food->setPosition((rand() % 30 + 1) * 20, (rand() % 22 + 1) * 20);

	} while (food->getPosition() == head->getPosition() &&
		food->getPositionX() <= 0 && food->getPositionY() <= 0 &&
		food->getPositionX() >= 620 && food->getPositionY() >= 440);

	background->setAnchorPoint(Vec2(0, 0));
	food->setAnchorPoint(Vec2(0, 0));
	message->setAnchorPoint(Vec2(0,0));
	scoreLabel->setAnchorPoint(Vec2(0,0));
	highScoreLabel->setAnchorPoint(Vec2(0,0));

	background->setPosition(0, 0);
	message->setPosition(this->getBoundingBox().getMidX()-150, this->getBoundingBox().getMaxY()-50);
	scoreLabel->setPosition(this->getBoundingBox().getMidX()-150, this->getBoundingBox().getMaxY()-50);
	highScoreLabel->setPosition(this->getBoundingBox().getMidX()+50, this->getBoundingBox().getMaxY()-50);

	this->addChild(background, 0);
	this->addChild(food, 1);
	this->addChild(message, 0);
	this->addChild(scoreLabel, 0);
	this->addChild(highScoreLabel, 0);
	scoreLabel->setVisible(false);
	highScoreLabel->setVisible(false);

	direction = 3;
	count = 0;

	if (score > highscore){
		highscore = score;
		std::stringstream ss;
		ss << "High Score: " << highscore;
		highScoreLabel->setString(ss.str().c_str());
	}
	score = 0;
	start = false;
	eat = false;
}

void SnakeScene::restart(){
	this->removeAllChildren();
	initializeGame();
}

bool SnakeScene::isKeyPressed(EventKeyboard::KeyCode code){
	if (keys.find(code) != keys.end()){
		return true;
	}
	return false;
}

void SnakeScene::moveSnake(){
	auto head = getSnakeHead();
	auto loc = head->getPosition();
	//log("Snake head tag: %i", head->getTag());
	if (start){
		//log("The game has started. good.");
		//for (auto snbody : snake){
			if (head->getPositionX() >= 620 || head->getPositionY() >= 460 ||
				head->getPositionX() <= 0 || head->getPositionY() <= 0){
				log("Not good");
				restart();
				//break;
			}
			else log("All good");


		//}
		while (count >= 12){
			switch (direction){
			case 0:{ //up
				log("Going up!");
				for (auto snbody : snake){
					if (snbody->getTag() == 0){
						log("imma head");
						loc = head->getPosition();
						snbody->setPosition(loc.x, loc.y + 20);
					}
					else{
						log("imma body");
						auto curloc = snbody->getPosition();
						snbody->setPosition(loc);
						loc = curloc;
					}//end else
				}//end for loop
			}; break;
			case 1:{ //left
				log("Going left!");
				for (auto snbody : snake){
					if (snbody->getTag() == 0){
						log("imma head");
						loc = head->getPosition();
						snbody->setPosition(loc.x - 20, loc.y);
					}
					else{
						log("imma body");
						auto curloc = snbody->getPosition();
						snbody->setPosition(loc);
						loc = curloc;
					}
				}
			}; break;
			case 2:{ //down
				log("Going down!");
				for (auto snbody : snake){
					if (snbody->getTag() == 0){
						log("imma head");
						loc = head->getPosition();
						snbody->setPosition(loc.x, loc.y - 20);
					}
					else{
						log("imma body");
						auto curloc = snbody->getPosition();
						snbody->setPosition(loc);
						loc = curloc;
					}
				}
			}; break;
			case 3:{ //right
				log("Going right!");
				for (auto snbody : snake){
					if (snbody->getTag() == 0){
						log("imma head");
						loc = head->getPosition();
						snbody->setPosition(loc.x + 20, loc.y);
					}
					else{
						log("imma body");
						auto curloc = snbody->getPosition();
						snbody->setPosition(loc);
						loc = curloc;
					}
				}
			}; break;
			}
			if (count == 12)
				count = 0;
		}
		if (count<12)
			count++;
	}//end if
	eat = false;
}

cocos2d::Sprite* SnakeScene::getSnakeHead(){
	for (auto sncur : snake){
		if (sncur->getTag() == 0){
			return sncur;
		}
	}
}

cocos2d::Sprite* SnakeScene::getSnakeBody(int tag){
	for (auto sncur : snake){
		if (sncur->getTag() == tag){
			return sncur;
		}
	}
}
std::map<cocos2d::EventKeyboard::KeyCode,
	std::chrono::high_resolution_clock::time_point> SnakeScene::keys;