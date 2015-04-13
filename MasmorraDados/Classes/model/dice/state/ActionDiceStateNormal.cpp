//
//  ActionDiceStateNormal.cpp
//  MasmorraDados
//
//  Created by Marlon Andrade on 4/13/15.
//
//

#include "ActionDiceStateNormal.h"

#include "ActionDiceSprite.h"
#include "ActionDiceStateSelected.h"

USING_NS_CC;

#pragma mark - Public Interface

bool ActionDiceStateNormal::init() {
  return true;
}

bool ActionDiceStateNormal::canChangeState() {
  return true;
}

void ActionDiceStateNormal::changeState(ActionDiceSprite* sprite) {
  auto selectedSprite = Sprite::create("images/dice/selected.png");
  selectedSprite->setPosition(Vec2(sprite->getContentSize().width / 2,
                                   sprite->getContentSize().height / 2));
  sprite->addChild(selectedSprite);
  
  sprite->setDiceState(ActionDiceStateSelected::create());
}