//
//  GameplayScene.cpp
//  MasmorraDados
//
//  Created by Marlon Andrade on 16/02/2015.
//
//

#include "GameplayScene.h"

#include "BackgroundLayer.h"
#include "CharacterDiceSprite.h"
#include "RoomPlacement.h"

USING_NS_CC;

#pragma mark - Public Interface

bool GameplayScene::init() {
  if (!Scene::init()) {
    return false;
  }
  
  this->_enableInteractions();
  
  auto game = Game::createWithRoomPlacedDelegate([&](Vector<RoomPlacement*> placements) {
    this->_disableInteractions();
    
    float delayTime = 0;
    int zOrder = placements.size();
    
    for (auto placement : placements) {
      auto room = placement->getRoom();
      auto position = placement->getPosition();
      
      auto roomSprite = Sprite::create(room->getImagePath());
      auto name = this->getGame()->getDungeon()->nameForPosition(position);
      roomSprite->setName(name);
      
      auto size = Director::getInstance()->getVisibleSize();
      auto origin = Director::getInstance()->getVisibleOrigin();
      
      auto deckPosition = Vec2(origin.x + size.width - TILE_DIMENSION / 2 - 20,
                               origin.y + size.height - TILE_DIMENSION / 2 - 20);
      roomSprite->setPosition(deckPosition);
      
      this->getObjectsLayer()->addChild(roomSprite, zOrder);
      
      auto spritePosition = this->_positionInScene(position);
      
      auto delay = DelayTime::create(delayTime);
      auto animationStarted = CallFunc::create([=]() {
        this->_disableInteractions();
        roomSprite->setLocalZOrder(DUNGEON_ROOM_Z_ORDER + 10);
      });
      auto easeMove = EaseBackIn::create(MoveTo::create(PLACE_ROOM_DURATION, spritePosition));
      auto animationEnded = CallFunc::create([=]() {
        this->_enableInteractions();
        roomSprite->setLocalZOrder(DUNGEON_ROOM_Z_ORDER);
      });
      
      roomSprite->runAction(Sequence::create(delay, animationStarted, easeMove,
                                             animationEnded, NULL));
      
      delayTime += PLACE_ROOM_DURATION;
      zOrder--;
    }
  });
  
  this->setGame(game);
  this->adjustInitialLayers();
  
  return true;
}

#pragma mark - Private Interface

void GameplayScene::adjustInitialLayers() {
  auto center = this->_centerOfScene();
  
  auto backgroundLayer = BackgroundLayer::create();
  this->addChild(backgroundLayer, -2);
  
  auto objectsLayer = this->_createObjectsLayer();
  this->addChild(objectsLayer, -1);
  
  auto controlsLayer = this->_createControlsLayer();
  this->addChild(controlsLayer, 1);
  
  this->getGame()->setCharacterPosition(INITIAL_POSITION);
  this->_adjustCharacterDiceSpritePosition();
}

Layer* GameplayScene::_createObjectsLayer() {
  auto objectsLayer = Layer::create();
  objectsLayer->setTag(OBJECTS_LAYER_TAG);
  
  auto initialRoom = this->getGame()->getDungeon()->getInitialRoom();
  auto initialPosition = INITIAL_POSITION;
  
  auto initialSprite = Sprite::create(initialRoom->getImagePath());
  auto name = this->getGame()->getDungeon()->nameForPosition(initialPosition);
  initialSprite->setName(name);
  initialSprite->setPosition(this->_positionInScene(INITIAL_POSITION));
  objectsLayer->addChild(initialSprite, DUNGEON_ROOM_Z_ORDER);
  
  objectsLayer->addChild(this->_createCharacterDiceSprite(), GAME_OBJECTS_Z_ORDER);
  
  return objectsLayer;
}

Layer* GameplayScene::_createControlsLayer() {
  auto controlsLayer = Layer::create();
  controlsLayer->setTag(CONTROLS_LAYER_TAG);
  
  return controlsLayer;
}

Node* GameplayScene::_createCharacterDiceSprite() {
  auto sprite = CharacterDiceSprite::create();
  sprite->setTag(CHARACTER_DICE_SPRITE_TAG);
  
  auto touchListener = EventListenerTouchOneByOne::create();
  touchListener->onTouchBegan = [&](Touch* touch, Event* event) {
    auto bounds = event->getCurrentTarget()->getBoundingBox();
    auto touchInSprite = bounds.containsPoint(touch->getLocation());
    bool containsBoot = true;
    auto canMove = this->_isInteractionEnabled() && touchInSprite && containsBoot;
    
    if (canMove) {
      Vector<Node*> visibleNodes;
      visibleNodes.pushBack(this->getObjectsLayer()->getChildByTag(CHARACTER_DICE_SPRITE_TAG));
      visibleNodes.pushBack(this->_getNodeForCharacterPosition());
      visibleNodes.pushBack(this->_getNodesForAdjacentCharacterPosition());
      this->_addOverlayWithVisibleNodes(visibleNodes);
      
      // b&w the rest of the game
      // highlight adjacent tiles
    }
    
    return canMove;
  };
  touchListener->onTouchMoved = [=](Touch* touch, Event* event) {
    sprite->setPosition(touch->getLocation());
  };
  touchListener->onTouchEnded = [=](Touch* touch, Event* event) {
    this->_removeOverlay();
    
    auto coordinate = this->getGame()->getCharacterPosition();
    auto position = this->_positionInScene(coordinate);
    auto move = MoveTo::create(RETURN_CHARACTER_DURATION, position);
    
    sprite->runAction(move);
  };
  
  auto dispatcher = Director::getInstance()->getEventDispatcher();
  dispatcher->addEventListenerWithSceneGraphPriority(touchListener, sprite);
  
  return sprite;
}

Layer* GameplayScene::getObjectsLayer() {
  return (Layer*) this->getChildByTag(OBJECTS_LAYER_TAG);
}

Layer* GameplayScene::getControlsLayer() {
  return (Layer*) this->getChildByTag(CONTROLS_LAYER_TAG);
}

Vec2 GameplayScene::_positionInScene(Vec2 gameCoordinate) {
  auto centerPosition = INITIAL_POSITION;
  auto centerOfScene = this->_centerOfScene();
  
  auto offsetX = gameCoordinate.x - centerPosition.x;
  auto offsetY = gameCoordinate.y - centerPosition.y;
  
  return Vec2(centerOfScene.x + offsetX * TILE_DIMENSION,
              centerOfScene.y + offsetY * TILE_DIMENSION);
}

void GameplayScene::_adjustCharacterDiceSpritePosition() {
  auto sprite = this->getObjectsLayer()->getChildByTag(CHARACTER_DICE_SPRITE_TAG);
  auto characterPosition = this->getGame()->getCharacterPosition();
  sprite->setPosition(this->_positionInScene(characterPosition));
}

void GameplayScene::_addOverlayWithVisibleNodes(Vector<Node *> visibleNodes) {
  auto bwLayer = LayerColor::create(Color4B(0, 0, 0, 0));
  bwLayer->setTag(BW_OVERLAY_LAYER_TAG);
  this->addChild(bwLayer, BW_OVERLAY_Z_ORDER);
  
  auto fadeIn = FadeTo::create(BW_OVERLAY_DURATION, 100);
  bwLayer->runAction(fadeIn);
  
  for (auto visibleNode : visibleNodes) {
    visibleNode->retain();
    visibleNode->removeFromParent();
    bwLayer->addChild(visibleNode);
    visibleNode->release();
  }
}

void GameplayScene::_removeOverlay() {
  this->_disableInteractions();
  
  auto bwLayer = this->getChildByTag(BW_OVERLAY_LAYER_TAG);
  
  auto fadeOut = FadeOut::create(BW_OVERLAY_DURATION);
  auto delayToFinishCharacterAnimation = DelayTime::create(RETURN_CHARACTER_DURATION);
  auto changeLayer = CallFunc::create([=]() {
    auto children = bwLayer->getChildren();
    for (auto node : children) {
      node->retain();
      node->removeFromParent();
      this->getObjectsLayer()->addChild(node);
      node->release();
    }
  });
  auto removeSelf = RemoveSelf::create();
  auto animationEnded = CallFunc::create([=]() {
    this->_enableInteractions();
  });
  
  bwLayer->runAction(Sequence::create(fadeOut, delayToFinishCharacterAnimation,
                                      changeLayer, removeSelf, animationEnded, NULL));
}

Vec2 GameplayScene::_centerOfScene() {
  Vec2 visibleOrigin = Director::getInstance()->getVisibleOrigin();
  Size visibleSize = Director::getInstance()->getVisibleSize();
  
  return Vec2(visibleSize.width / 2 + visibleOrigin.x,
              visibleSize.height / 2 + visibleOrigin.y);
}

Node* GameplayScene::_getNodeForCharacterPosition() {
  auto position = this->getGame()->getCharacterPosition();
  auto name = this->getGame()->getDungeon()->nameForPosition(position);
  return this->getObjectsLayer()->getChildByName(name);
}

Vector<Node*> GameplayScene::_getNodesForAdjacentCharacterPosition() {
  Vector<Node*> nodes;
  
  auto position = this->getGame()->getCharacterPosition();
  auto adjacentPositions = this->getGame()->getDungeon()->adjacentPositionsTo(position);
  for (auto adjacentPosition : adjacentPositions) {
    auto name = this->getGame()->getDungeon()->nameForPosition(adjacentPosition);
    nodes.pushBack(this->getObjectsLayer()->getChildByName(name));
  }
  
  return nodes;
}

bool GameplayScene::_isInteractionEnabled() {
  return _userInteractionEnabled;
}

void GameplayScene::_disableInteractions() {
  _userInteractionEnabled = false;
}

void GameplayScene::_enableInteractions() {
  _userInteractionEnabled = true;
}