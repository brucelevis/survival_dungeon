//
//  CharacterDiceSprite.cpp
//  MasmorraDados
//
//  Created by Marlon Andrade on 24/02/2015.
//
//

#include "CharacterDiceSprite.h"

#include "Definitions.h"
#include "Images.h"
#include "NodeNames.h"

USING_NS_CC;

#pragma mark - Public Interface

CharacterDiceSprite* CharacterDiceSprite::createWithDelegate(CharacterMoveDelegate* delegate) {
  auto character = new (std::nothrow) CharacterDiceSprite();
  if (character && character->initWithDelegate(delegate)) {
    character->autorelease();
  } else {
    CC_SAFE_DELETE(character);
  }

  return character;
}


bool CharacterDiceSprite::initWithDelegate(CharacterMoveDelegate *delegate) {
  if (!Sprite::init()) {
    return false;
  }
  
  this->setDelegate(delegate);
  this->setName(CHARACTER_DICE_SPRITE_NAME);
  this->setHitpoints(CHARACTER_INITIAL_HP);
  this->_setupTouchListener();
  
  return true;
}

void CharacterDiceSprite::setHitpoints(int hitpoints) {
  if (hitpoints > 6) hitpoints = 6;
  if (hitpoints < 1) hitpoints = 1;
  
  this->setTexture(this->_getFileNameForHitpoints(hitpoints));
}

#pragma mark - Private Interface

void CharacterDiceSprite::_setupTouchListener() {
  auto touchListener = EventListenerTouchOneByOne::create();
  touchListener->setSwallowTouches(true);
  touchListener->onTouchBegan = CC_CALLBACK_2(CharacterDiceSprite::_onTouchBegan, this);
  touchListener->onTouchMoved = CC_CALLBACK_2(CharacterDiceSprite::_onTouchMoved, this);
  touchListener->onTouchEnded = CC_CALLBACK_2(CharacterDiceSprite::_onTouchEnded, this);
  
  auto dispatcher = Director::getInstance()->getEventDispatcher();
  dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

std::string CharacterDiceSprite::_getFileNameForHitpoints(int hitpoints) {
  std::string fileName = IMG_DICE_CHAR;
  
  auto xxPos = fileName.find("xx");
  
  std::stringstream stream;
  stream << hitpoints;
  auto hp = stream.str();
  
  return fileName.replace(xxPos, 2, hp);
}

#pragma mark Touch Events

bool CharacterDiceSprite::_onTouchBegan(Touch *touch, Event *event) {
  auto target = event->getCurrentTarget();
  auto layer = target->getParent();
  
  auto bounds = target->getBoundingBox();
  auto touchLocation = layer->convertTouchToNodeSpace(touch);
  
  auto touchInSprite = bounds.containsPoint(touchLocation);
  
  auto canMove = touchInSprite && this->getDelegate()->canCharacterMove();
  
  if (canMove) {
    this->getDelegate()->characterWillMove(this);
  }
  
  return canMove;
}

void CharacterDiceSprite::_onTouchMoved(Touch *touch, Event *event) {
  auto target = event->getCurrentTarget();
  auto layer = target->getParent();
  
  auto bounds = target->getBoundingBox();
  auto touchLocation = layer->convertTouchToNodeSpace(touch);
  
  this->setPosition(touchLocation);
  
  auto dungeonLayer = layer->getParent();
  auto position = dungeonLayer->convertTouchToNodeSpace(touch);
  
  this->getDelegate()->characterIsMovingToLocation(position);
}

void CharacterDiceSprite::_onTouchEnded(Touch *touch, Event *event) {
  auto target = event->getCurrentTarget();
  auto dungeonLayer = target->getParent()->getParent();
  
  auto position = dungeonLayer->convertTouchToNodeSpace(touch);
  
  bool characterMoved = this->getDelegate()->canCharacterMoveToLocation(position);
  
  if (characterMoved) {
    this->getDelegate()->characterMovedToLocation(this, position);
  } else {
    this->getDelegate()->characterDidNotMove(this);
  }
}