//
//  PlayerSkillsLayer.cpp
//  MasmorraDados
//
//  Created by Marlon Andrade on 4/27/15.
//
//

#include "PlayerSkillsLayer.h"

#include "ActionDiceDragData.h"
#include "Definitions.h"

USING_NS_CC;

#pragma mark - Public Interface

bool PlayerSkillsLayer::init() {
  if (!Layer::init()) {
    return false;
  }
  
  this->_setupFreeBootSymbol();
  this->_setupDockableDice();
  this->_setupEventHandlers();
  
  return true;
}

Vector<Node*> PlayerSkillsLayer::getDockableNodes() {
  return this->_getDockContainer()->getChildren();
}

#pragma mark - Private Interface

void PlayerSkillsLayer::_setupFreeBootSymbol() {
  auto sprite = Sprite::create(IMG_DICE_ACTION_FREE_BOOT);
  sprite->setName(FREE_BOOT_SPRITE_NAME);
  
  auto size = sprite->getContentSize() / 2;
  auto x = DOCK_MARGIN + size.width;
  auto y = this->getContentSize().height - (DOCK_MARGIN + size.height);
  
  sprite->setPosition(Vec2(x, y));
  
  this->addChild(sprite);
}

void PlayerSkillsLayer::_setupDockableDice() {
  auto dockContainer = Node::create();
  dockContainer->setName(DOCK_CONTAINER_NODE_NAME);
  
  auto bootSprite = this->getChildByName(FREE_BOOT_SPRITE_NAME);
  
  this->addChild(dockContainer);
  
  int dockableSpaces = 6;
  
  auto height = 0;
  auto width = 0;
  auto marginBetweenSpaces = 1;
  
  for (int i = 0; i < dockableSpaces; i++) {
    auto sprite = Sprite::create(IMG_DICE_ACTION_DOCK);
    
    width = sprite->getContentSize().width;
    height = sprite->getContentSize().height;
    
    auto x = 0.f;
    auto y = height * i + (i * marginBetweenSpaces);
    
    sprite->setPosition(Vec2(x, y));
    
    dockContainer->addChild(sprite);
  }
  
  auto bootSize = bootSprite->getContentSize();
  
  auto dockX = DOCK_MARGIN + bootSize.width / 2;
  auto dockY = this->getContentSize().height -
      (DOCK_MARGIN + (dockableSpaces - 0.5) * height +
       bootSize.height + dockableSpaces * marginBetweenSpaces);
  
  auto totalWidth = width;
  auto totalHeight = height * dockableSpaces + (dockableSpaces - 1) * marginBetweenSpaces;
  
  dockContainer->setPosition(Vec2(dockX, dockY));
  dockContainer->setContentSize(Size(totalWidth, totalHeight));
}

void PlayerSkillsLayer::_setupEventHandlers() {
  auto dispatcher = Director::getInstance()->getEventDispatcher();
  
  dispatcher->addCustomEventListener(
      EVT_ACTION_DICE_DRAG_STARTED,
      CC_CALLBACK_1(PlayerSkillsLayer::_handleActionDiceDragStarted, this)
  );
  dispatcher->addCustomEventListener(
      EVT_ACTION_DICE_DRAG_MOVED,
      CC_CALLBACK_1(PlayerSkillsLayer::_handleActionDiceDragMoved, this)
  );
  dispatcher->addCustomEventListener(
      EVT_ACTION_DICE_DRAG_ENDED,
      CC_CALLBACK_1(PlayerSkillsLayer::_handleActionDiceDragEnded, this)
  );
}

Node* PlayerSkillsLayer::_getDockContainer() {
  return this->getChildByName(DOCK_CONTAINER_NODE_NAME);
}

void PlayerSkillsLayer::_addOverlay() {
  auto overlayLayer = LayerColor::create(Color4B(0, 0, 0, 0));
  overlayLayer->setName(OVERLAY_LAYER_NAME);
  
  for (auto node : this->getChildren()) {
    auto newZOrder = node->getLocalZOrder() + OVERLAY_Z_ORDER;
    node->setLocalZOrder(newZOrder);
  }
  
  this->addChild(overlayLayer, OVERLAY_Z_ORDER - 1);
  
  auto fadeIn = FadeTo::create(OVERLAY_DURATION, OVERLAY_OPACITY);
  overlayLayer->runAction(fadeIn);
}

void PlayerSkillsLayer::_removeOverlay() {
  auto overlayLayer = this->getChildByName(OVERLAY_LAYER_NAME);
  
  auto fadeOut = FadeOut::create(OVERLAY_DURATION);
  auto changeLayer = CallFunc::create([=]() {
    for (auto node : this->getChildren()) {
      auto oldZOrder = node->getLocalZOrder() - OVERLAY_Z_ORDER;
      node->setLocalZOrder(oldZOrder);
    }
  });
  auto removeSelf = RemoveSelf::create();
  
  overlayLayer->runAction(Sequence::create(fadeOut, changeLayer, removeSelf, NULL));
}

#pragma mark - Event Handlers

void PlayerSkillsLayer::_handleActionDiceDragStarted(EventCustom* event) {
  log("drag started");
  
  auto dockableNodes = this->getDockableNodes();
  this->_addOverlay();
  
  auto data = (ActionDiceDragData*) event->getUserData();
  auto sprite = data->getSprite();
  sprite->runAction(ScaleTo::create(0.2, 0.58));
}

void PlayerSkillsLayer::_handleActionDiceDragMoved(EventCustom* event) {
  auto data = (ActionDiceDragData*) event->getUserData();
  
  auto touchLocation = this->convertTouchToNodeSpace(data->getTouch());
  data->getSprite()->setPosition(touchLocation);
  
  auto dockableContainer = this->getChildByName(DOCK_CONTAINER_NODE_NAME);
  auto dockableLocation = dockableContainer->convertTouchToNodeSpaceAR(data->getTouch());
  
  for (auto node : this->getDockableNodes()) {
    Color3B color = Color3B::WHITE;
    
    if (node->getBoundingBox().containsPoint(dockableLocation)) {
      color = Color3B(170, 255, 170);
    }
    
    node->setColor(color);
  }
}

void PlayerSkillsLayer::_handleActionDiceDragEnded(EventCustom* event) {
  auto data = (ActionDiceDragData*) event->getUserData();
  auto sprite = data->getSprite();
  auto touchLocation = this->convertTouchToNodeSpace(data->getTouch());
  
  auto dockableContainer = this->getChildByName(DOCK_CONTAINER_NODE_NAME);
  auto dockableLocation = dockableContainer->convertTouchToNodeSpaceAR(data->getTouch());
  
  auto position = Vec2::ZERO;
  
  bool moved = false;
  for (auto node : this->getDockableNodes()) {
    if (node->getBoundingBox().containsPoint(dockableLocation)) {
      moved = true;
      position = node->getPosition() + dockableContainer->getPosition();
      break;
    }
  }
  
  log("drag ended");
  if (moved) {
    auto move = MoveTo::create(0.1, position);
    sprite->runAction(move);
    
    log("animate move");
  } else {
    auto move = MoveTo::create(0.2, sprite->getOriginalPosition());
    auto scale = ScaleTo::create(0.2, 1);
    
    sprite->runAction(Spawn::create(move, scale, NULL));
    log("animate back");
  }
  
  this->_removeOverlay();
}
